/*

Description:
	This program runs in background with administrator privileges and keeps stopping windows update services
	
How to install:
	Follow README instructions
	
Enjoy =)

*/

#include <iostream>
#include <windows.h>

#define TTS 5000	//Time to sleep: 5 seconds

using namespace std;


BOOL IsRunAsAdministrator();		//Check if program is running as administrator
void ElevateNow();					//Ask you to run program with administrator privileges

int main(){
	
	//Run process in background
	HWND window;
	AllocConsole();
	window = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(window,0);
	
	//Check if it's running as administrator, if not it asks you
	if(!IsRunAsAdministrator()){
		ElevateNow();
		exit(0);
	}
	
	while(1){
		
		//Stop windows update
		system("net stop wuauserv");
		system("sc config \"wuauserv\" start= disabled");
		system("net stop bits");
		system("sc config \"bits\" start= disabled");
		
		//Kill processes if they aren't already dead
		system("taskkill /F /FI \"SERVICES eq wuauserv\"");
		system("taskkill /F /FI \"SERVICES eq bits\"");
		
		//Delete SoftwareDistribution folder (the one containing windows update files)
		system("del /q \"C:\\Windows\\SoftwareDistribution\\*\"");
		system("FOR /D %p IN (\"C:\\Windows\\SoftwareDistribution\\*.*\") DO rmdir \"%p\" /s /q");
		
		//Kill MusNotifyIcon.exe process (the shitty icon that says you don't have updates and bla bla bla...)
		system("taskkill /f /im MusNotifyIcon.exe");
		
		//Sleep for 5 seconds
		Sleep(TTS);
		
	}
	
	return 0;
}

void ElevateNow() {
	BOOL bAlreadyRunningAsAdministrator = FALSE;
	try
	{
		bAlreadyRunningAsAdministrator = IsRunAsAdministrator();
	}
	catch(int ex){}
	
	if(!bAlreadyRunningAsAdministrator) {
		char szPath[MAX_PATH];
		if (GetModuleFileName(NULL, szPath, ARRAYSIZE(szPath))) {
			
			SHELLEXECUTEINFO sei = { sizeof(sei) };
			
			sei.lpVerb = "runas";
			sei.lpFile = szPath;
			sei.hwnd = NULL;
			sei.nShow = SW_NORMAL;

			if (!ShellExecuteEx(&sei)) {
				DWORD dwError = GetLastError();
				if (dwError == ERROR_CANCELLED)
					CreateThread(0,0,(LPTHREAD_START_ROUTINE)ElevateNow,0,0,0);
	       	}
		}

	}
	else
	{
	///Code
	}
}

BOOL IsRunAsAdministrator() {
    BOOL fIsRunAsAdmin = FALSE;
    DWORD dwError = ERROR_SUCCESS;
    PSID pAdministratorsGroup = NULL;

    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(
        &NtAuthority, 
        2, 
        SECURITY_BUILTIN_DOMAIN_RID, 
        DOMAIN_ALIAS_RID_ADMINS, 
        0, 0, 0, 0, 0, 0, 
        &pAdministratorsGroup))
    {
        dwError = GetLastError();
        goto Cleanup;
    }

    if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin)) {
        dwError = GetLastError();
        goto Cleanup;
    }

Cleanup:

    if (pAdministratorsGroup) {
        FreeSid(pAdministratorsGroup);
        pAdministratorsGroup = NULL;
    }

    if (ERROR_SUCCESS != dwError) {
        throw dwError;
    }

    return fIsRunAsAdmin;
}
