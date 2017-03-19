

#include <SDKDDKVer.h>
#include <api/win32_common.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

void winhttp_send(const wchar_t* host, const wchar_t* url, int port, const std::string& data)
{
    BOOL  retval = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    HINTERNET hSession = NULL;
    HINTERNET hConnect = NULL;
    HINTERNET hRequest = NULL;
    hSession = WinHttpOpen(L"WinHTTP Example/1.0",
                           WINHTTP_ACCESS_TYPE_NO_PROXY,
                           WINHTTP_NO_PROXY_NAME,
                           WINHTTP_NO_PROXY_BYPASS, 0);

    if (!hSession)
        goto Exit0;

    
    hConnect = WinHttpConnect(hSession, host, port, 0);

    if (!hConnect)
        goto Exit0;

    
    hRequest = WinHttpOpenRequest(hConnect, L"POST", url,
                                      NULL, WINHTTP_NO_REFERER,
                                      WINHTTP_DEFAULT_ACCEPT_TYPES,
                                      NULL);

    if (!hRequest)
        goto Exit0;

    
    retval = WinHttpSendRequest(hRequest,
                                    WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                    (LPVOID)data.c_str(), data.length(), data.length(), 0);


    if (!retval)
        goto Exit0;

   
    retval = WinHttpReceiveResponse(hRequest, NULL);
    if (!retval)
        goto Exit0;

    
    do {
        // Check for available data.
        dwSize = 0;

        if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
            printf("Error %u in WinHttpQueryDataAvailable.\n",  GetLastError());

        // Allocate space for the buffer.
        pszOutBuffer = new char[dwSize + 1];

        if (!pszOutBuffer) {
            printf("Out of memory\n");
            dwSize = 0;
            goto Exit0;
        } else {
            // Read the data.
            ZeroMemory(pszOutBuffer, dwSize + 1);

            if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
                                 dwSize, &dwDownloaded))
                printf("Error %u in WinHttpReadData.\n", GetLastError());
            else
                printf("%s", pszOutBuffer);

            // Free the memory allocated to the buffer.
            delete[] pszOutBuffer;
        }
    } while (dwSize > 0);
    

    // Report any errors.
    if (!retval) 
        printf("Error %d has occurred.\n", GetLastError());
Exit0:

    // Close any open handles.
    if (hRequest) {
        WinHttpCloseHandle(hRequest);
        hRequest = NULL;
    }

    if (hConnect) {
        WinHttpCloseHandle(hConnect);
        hConnect = NULL;
    }

    if (hSession) {
        WinHttpCloseHandle(hSession);
        hSession = NULL;
    }
}


int main()
{
#if defined(WIN32) && defined(_DEBUG)
    int flags = _crtDbgFlag       | 
        _CRTDBG_ALLOC_MEM_DF      |
        _CRTDBG_DELAY_FREE_MEM_DF |
        _CRTDBG_CHECK_ALWAYS_DF;

    _CrtSetDbgFlag(flags);
    //AllocConsole();
    //freopen("CONOUT$", "w", stdout);
#endif 

    winhttp_send(L"192.168.1.195", L"www/check.cgi", 80, "your data to send to service ");

#if defined(WIN32) && defined(_DEBUG)
    _CrtDumpMemoryLeaks();
#endif

    system("pause");
    return 0;
}

