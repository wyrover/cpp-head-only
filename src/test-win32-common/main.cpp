
#include "stdafx.h"
#include <SDKDDKVer.h>
#include <api/win32_common.h>
#include <iostream>
#include <stdio.h>




int main()
{
#ifdef DEBUG
    _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_LEAK_CHECK_DF);
#endif

	if (stristr("test123", "123")) {
		printf("123");
	}

	std::cout << trim(" adsfasdf ").c_str() << std::endl;
	std::cout << hex_encode("A", 1).c_str() << std::endl;
	std::wcout << get_app_filename().c_str() << std::endl;
	std::wcout << get_cwd() << std::endl;
	std::wcout << extract_filepath(get_cwd()) << std::endl;
	std::wcout << extract_filename(get_app_filename()) << std::endl;
	std::wcout << get_full_pathname(extract_filename(get_app_filename())) << std::endl;
	std::wcout << complete_path_with_modulepath(L"test.txt") << std::endl;
	std::wcout << get_system_drv() << std::endl;

    /*const char *codeset = 0;
    char cp[SIZEOF_CP_NAME];
    UINT codepage = GetConsoleCP();
    if (!codepage) codepage = GetACP();
    CP_FORMAT(cp, codepage);
    
    std::cout << cp << std::endl;*/

    system("pause");
    
    return 0;
}

