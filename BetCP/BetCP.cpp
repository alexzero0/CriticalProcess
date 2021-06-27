// BetCP.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>
using namespace std;

BOOL setPrivileges(LPCTSTR szPrivName)
{
	TOKEN_PRIVILEGES tp = { 0 };
	HANDLE Handle = 0;
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &Handle))
	{
		cout << "Eror OpenProcessToken : " << GetLastError() << endl;
	}
	if (!LookupPrivilegeValue(NULL, szPrivName, &tp.Privileges[0].Luid))
	{
		cout << "Eror LookupPrivilegeValue : " << GetLastError() << endl;
	}
	if (!AdjustTokenPrivileges(Handle, FALSE, &tp, sizeof(tp), NULL, NULL))
	{
		cout << "Eror AdjustTokenPrivileges : " << GetLastError() << endl;
		CloseHandle(Handle);
		return 0;//1?
	}
	CloseHandle(Handle);
	return 1;//0?
}

typedef NTSTATUS (NTAPI *pRtlSetProcessIsCritical)(BOOLEAN bNewValue, BOOLEAN *pbOldValue, BOOLEAN CheckFlag);

//BOOL set_proc_critical(int Flag1,int Flag2)
//{
//	/*HANDLE Hand = 0;
//	Hand = LoadLibrary("ntdll.dll");
//	if (Hand == NULL)
//	{
//		cout << "Eror LoadLibrary <--- " << endl;
//		return FALSE;
//	}*/
//
//	pRtlSetProcessIsCritical SetCritproccess;
//	SetCritproccess = (pRtlSetProcessIsCritical)GetProcAddress(GetModuleHandle("ntdll.dll"), "RtlSetProcessIsCritical");
//
//
//	if (SetCritproccess == NULL)
//	{
//		cout << "Eror adress!!! RtlSetProcessIsCritical " << endl;
//		return FALSE;
//	}
//	if (SetCritproccess(Flag1, 0, Flag2))//TRUE//FALSE
//	{
//		cout << "RtlSetProcessIsCritical !" << endl;
//		return TRUE;
//	}
//	else return FALSE;
//}

//typedef NTSTATUS(__stdcall *RtlAdjustPrivilege)(DWORD, BOOL, INT, PBOOL);

//BOOL DoRtlAdjustPrivilege()
//{
//	RtlAdjustPrivilege Priv = (RtlAdjustPrivilege)GetProcAddress(LoadLibrary("ntdll.dll"), "RtlAdjustPrivilege");
//	if (!Priv)
//	{
//		return FALSE;
//	}
//	BOOL pBL= FALSE;
//	Priv(20, TRUE, FALSE, &pBL);
//}


int main()
{
	system("pause");

	if(!setPrivileges(SE_DEBUG_NAME)){ cout << "err setPrivileges!" << endl; }

	pRtlSetProcessIsCritical SetCritproccess;
	SetCritproccess = (pRtlSetProcessIsCritical)GetProcAddress(GetModuleHandle("ntdll.dll"), "RtlSetProcessIsCritical");
	if (!SetCritproccess) 
	{ 
		cout << "Eror adress!!! (RtlSetProcessIsCritical) " << endl; 
		system("pause");
		return 0;
	}
	BOOLEAN dwOldValue;
	SetCritproccess(1, &dwOldValue, 0);
	cout << " set_proc_critical ON !" << endl;
	//if (!set_proc_critical(TRUE, FALSE)) { cout << "err set_proc_critical ON !" << endl; }
	
	system("pause");
	SetCritproccess(dwOldValue, &dwOldValue, 0);
	cout << "err set_proc_critical OFF !" << endl;
	//if(!set_proc_critical(FALSE, FALSE)){ cout << "err set_proc_critical OFF !" << endl; }
	FreeLibrary(GetModuleHandle("ntdll.dll"));
	system("pause");

    return 0;
}

