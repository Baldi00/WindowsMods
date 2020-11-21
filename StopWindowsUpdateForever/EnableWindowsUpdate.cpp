/*

Description:
	This program kills the process "StopWindowsUpdateForever.exe" and starts windows update services
	
Enjoy =)

*/

#include <iostream>
#include <windows.h>

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
	
	//Stop "StopWindowsUpdateForever.exe" if exists
	system("taskkill /IM StopWindowsUpdateForever.exe /F");
	
	//Start windows update
	system("sc config \"wuauserv\" start= auto");
	system("net start wuauserv");
	system("sc config \"bits\" start= auto");
	system("net start bits");
		
	
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
