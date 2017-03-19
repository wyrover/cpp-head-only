

#include <SDKDDKVer.h>
#include <api/win32_common.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <shellapi.h>

HINSTANCE hApplet = 0;

typedef BOOL (WINAPI *PDEVMGREXEC)(HWND hWndParent, HINSTANCE hInst, PVOID Unknown, int nCmdShow);

static BOOL
LaunchDeviceManager(HWND hWndParent)
{
    /* Hack for ROS to start our devmgmt until we have MMC */
#ifdef __REACTOS__
    return ((INT_PTR)ShellExecuteW(NULL, L"open", L"devmgmt.exe", NULL, NULL, SW_SHOWNORMAL) > 32);
#else
    HMODULE hDll;
    PDEVMGREXEC DevMgrExec;
    BOOL Ret;

    hDll = LoadLibrary(_TEXT("devmgr.dll"));
    if(!hDll)
        return FALSE;

    DevMgrExec = (PDEVMGREXEC)GetProcAddress(hDll, "DeviceManager_ExecuteW");
    if(!DevMgrExec)
    {
        FreeLibrary(hDll);
        return FALSE;
    }

    /* Run the Device Manager */
    Ret = DevMgrExec(hWndParent, hApplet, NULL /* ??? */, SW_SHOW);
    FreeLibrary(hDll);
    return Ret;
#endif /* __REACTOS__ */
}

static VOID
LaunchHardwareWizard(HWND hWndParent)
{
    SHELLEXECUTEINFO shInputDll;

    memset(&shInputDll, 0x0, sizeof(SHELLEXECUTEINFO));

    shInputDll.cbSize = sizeof(shInputDll);
    shInputDll.hwnd = hWndParent;
    shInputDll.lpVerb = _T("open");
    shInputDll.lpFile = _T("rundll32.exe");
    shInputDll.lpParameters = _T("shell32.dll,Control_RunDLL hdwwiz.cpl");

    if (ShellExecuteEx(&shInputDll) == 0)
    {
        MessageBox(NULL,
            _T("Can't start hdwwiz.cpl"),
            NULL,
            MB_OK | MB_ICONERROR);
    }
}


int main()
{
#ifdef DEBUG
    _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_LEAK_CHECK_DF);
#endif

    hApplet = GetModuleHandle(NULL);

    LaunchDeviceManager(NULL);

    //LaunchHardwareWizard(NULL);

    system("pause");
    
    return 0;
}

