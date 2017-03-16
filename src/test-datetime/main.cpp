#include <SDKDDKVer.h>
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <process.h>
#include <time.h>
#include <errno.h>
#include <vector>

#include <api/datetime.h>

char *
rb_w32_ugetenv(const char *name)
{
    return w32_getenv(name, CP_UTF8);
}


int main()
{
#ifdef DEBUG
    _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_LEAK_CHECK_DF);
#endif
    
    return 0;
}

