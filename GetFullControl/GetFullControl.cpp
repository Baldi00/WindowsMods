#include <iostream>
#include <windows.h>
#include <Lmcons.h>

using namespace std;

BOOL IsRunAsAdministrator();		//Check if program is running as administrator
void ElevateNow();					//Ask you to run program with administrator privileges

string params = "";

int main(int argc, char* argv[]){
	
	//Coping parameters for elevation
	for(int i=1; i<argc; i++){
		string param = string(argv[i]);
		params = params + "\"" + param + "\" ";
	}
	
	//Check if it's running as administrator, if not it asks you
	if(!IsRunAsAdministrator()){
		ElevateNow();
		exit(0);
	}
	
	string cmd;
	char username[UNLEN+1];
	DWORD username_len = UNLEN+1;
	
	//Get username
	GetUserName(username, &username_len);
	
	string usernameS = string(username);
	
	for(int i=1; i<argc; i++){
		string folder = string(argv[i]);
		cmd = "echo TAKING OWNERSHIP on \"" + folder + "\":";
		system(cmd.c_str());
		cmd = "takeown /f \"" + folder + "\" /r";
		system(cmd.c_str());
		
		system("echo. & echo. & echo.");
		
		cmd = "echo GRANTING ALL PERMISSIONS on \"" + folder + "\": & echo.";
		system(cmd.c_str());
		cmd = "icacls \"" + folder + "\" /grant " + usernameS + ":F /t";
		system(cmd.c_str());
		
		system("echo. & echo. & echo.");
	}
	
	system("pause");
	
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
			sei.lpParameters = params.c_str();
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
