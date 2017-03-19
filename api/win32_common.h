#ifndef __CACTUS_WIN32_COMMON_H__
#define __CACTUS_WIN32_COMMON_H__

typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
typedef uint8_t u_int8_t;
typedef uint16_t u_int16_t;
typedef uint32_t u_int32_t;
typedef uint64_t u_int64_t;


#ifdef _MSC_VER

#if _MSC_VER >= 1400

    #ifndef _CRT_SECURE_NO_DEPRECATE
        #define _CRT_SECURE_NO_DEPRECATE
    #endif

    #ifndef _CRT_SECURE_NO_WARNINGS
        #define _CRT_SECURE_NO_WARNINGS
    #endif

#endif

	// ×Ö·û´®
#if _MSC_VER < 1900
	#define snprintf		_snprintf
	#define snwprintf		_snwprintf
#endif

	#define strcasecmp		_stricmp
	#define strncasecmp		_strnicmp
	#define wcscasecmp		_wcsicmp
	#define wcsncasecmp		_wcsnicmp
	#define stristr			StrStrIA
	#define wcsistr			StrStrIW
	#define strdate         _strdate
    #define strtime         _strtime

    #define getpid _getpid
    #define getcmd  _getcmd

	#include <Shlwapi.h>
	#pragma comment(lib, "shlwapi.lib")
#endif



#include <Windows.h>
#include <string>
#include <vector>
#include <set>



typedef char				char8;
typedef wchar_t				char16;
typedef std::wstring		string16;
typedef std::string			string8;
typedef std::string			ustring;

#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif



std::string& trim(std::string& s);
std::string trim(const std::string& str);


const std::string  sEmpty = "";
const std::wstring sWEmpty = L"";
const std::wstring PathDelimiters = L"\\/";
const char16 PathDelim  = L'\\';
const char16 DirDelimiter = L'\\';
const char16 DriveDelim = L':';
const char16 PathSep    = L';';



template<typename To, typename From>
inline To convert_to(const From& from)
{
    //enum { not_supported = 1 / (1 == 0) }; // static_assert(false)
}
/*! @brief µ¥×Ö½Ú×Ö·û´®×ª¿í×Ö½Ú×Ö·û´®
 *
 * @param[in] from µ¥×Ö½Ú×Ö·û´®
 * @return ·µ»Ø¿í×Ö½Ú×Ö·û´®
 */
template<> inline string16 convert_to(const std::string& from)
{
    int len = static_cast<int>(from.size() + 1);
    std::vector<char16> wideBuf(len);
    int ret = MultiByteToWideChar(CP_ACP, 0, from.c_str(), len, &wideBuf[0], len);

    if (ret == 0)
        return sWEmpty;
    else
        return string16(&wideBuf[0]);
}

//template<> inline string16 convert_to(const int& from)
//{
//    return format_string(L"%d", from);
//}
//
//template<> inline string16 convert_to(const long& from)
//{
//    return format_string(L"%ld", from);
//}
//
//template<> inline string16 convert_to(const unsigned int& from)
//{
//    return format_string(L"%u", from);
//}
//
//template<> inline string16 convert_to(const unsigned long& from)
//{
//    return format_string(L"%lu", from);
//}
//
//template<> inline string16 convert_to(const float& from)
//{
//    return format_string(L"%.16g", from);
//}
//
//template<> inline string16 convert_to(const double& from)
//{
//    return format_string(L"%.16g", from);
//}

template<> inline string16 convert_to(const char* const& from)
{
    int len = static_cast<int>(strlen(from) + 1);
    std::vector<char16> wideBuf(len);
    int ret = MultiByteToWideChar(CP_ACP, 0, from, len, &wideBuf[0], len);

    if (ret == 0)
        return sWEmpty;
    else
        return string16(&wideBuf[0]);
}


template<> inline std::string convert_to(const char16* const& from)
{
    int len = static_cast<int>(wcslen(from));
    int required_size = WideCharToMultiByte(CP_ACP, 0, from, (DWORD)len, NULL, 0, NULL, NULL);

    if (required_size == 0)
        return sEmpty;

    std::vector<char> buf(++required_size);
    int ret = WideCharToMultiByte(CP_ACP, 0, from, len, &buf[0], required_size, NULL, NULL);

    if (ret == 0)
        return sEmpty;
    else
        return std::string(&buf[0]);
}

/*! @brief Ë«×Ö½Ú×Ö·û´®×ªµ¥×Ö½Ú×Ö·û´®
 *
 * @param[in] from Ë«×Ö½Ú×Ö·û´®
 * @return ·µ»Øµ¥×Ö½Ú×Ö·û´®
 */
template<> inline std::string convert_to(const string16& from)
{
    int len = static_cast<int>(from.size());
    int required_size = WideCharToMultiByte(CP_ACP, 0, from.c_str(), (DWORD)len, NULL, 0, NULL, NULL);

    if (required_size == 0)
        return sEmpty;

    std::vector<char> buf(++required_size);
    int ret = WideCharToMultiByte(CP_ACP, 0, from.c_str(), len, &buf[0], required_size, NULL, NULL);

    if (ret == 0)
        return sEmpty;
    else
        return std::string(&buf[0]);
}





string8 hex_encode(const void* bytes, size_t size);
string16 formatBytesW(int64_t aBytes);


// ÎÄ¼þÂ·¾¶
string16 get_app_filename();
string16 get_app_dir();
string16 get_cwd();
string16 get_current_working_dir();
string16 get_system_drv();			// »ñÈ¡ÏµÍ³ÅÌÅÌ·û
string16 extract_filepath(const string16& filename);
string16 extract_filename(const string16& filename);
string16 extract_fileext(const string16& filename);
string16 expand_filename(const string16& filename);
inline string16 get_full_pathname(const string16& filename)
{
	return expand_filename(filename);
}
string16 complete_path_with_modulepath(const string16& filename);
string16 get_parent_directory(const string16& path);
string16 IncludeTrailingPathDelimiter(const string16& S);
inline string16 path_add_backslash(const string16& path)
{
	return IncludeTrailingPathDelimiter(path);
}
string16 ExcludeTrailingPathDelimiter(const string16& S);
inline string16 path_remove_backslash(const string16& S)
{
	return ExcludeTrailingPathDelimiter(S);
}
void file_unix_to_dos_path(char* lpPath);

bool file_get_contents(const string8& filename, std::vector<char>& buffer);
bool file_get_contents(const string16& filename, std::vector<char>& buffer);
bool file_get_contents(const string16& filename, std::vector<string16>& list);
bool file_get_contents(const string16& filename, std::set<string16>& list);
BOOL get_file_large_size(LPCWSTR lpFilePath, LARGE_INTEGER& liFileSize);
void hex_dump_to_file(FILE* dmpf, const char* name, BYTE* addr, size_t size);


inline FILE* OpenFile(const string16& filename, const char* mode)
{
	string16 w_mode = convert_to<string16>(mode);
	FILE* file;

	if (_wfopen_s(&file, filename.c_str(), w_mode.c_str()) != 0) {
		return NULL;
	}

	return file;
}

inline FILE* OpenFile(const string8& filename, const char* mode)
{
	FILE* file;

	if (fopen_s(&file, filename.c_str(), mode) != 0) {
		return NULL;
	}

	return file;
}

bool get_file_str_content(const string16& path, std::string* contents);


// windows ´íÎóÂë
char* windows_format_error_string(unsigned long dwError, char* pszBuf, int iSize);

// ½ø³Ì´¦Àí
BOOL TerminateProcessById(DWORD dwProcessID);


namespace base64 {
std::string r_base64_encode(const char* from, size_t length);
std::string r_base64_decode(const char* from, size_t length);


inline std::string r_base64_encode(std::string const& from)
{
    return r_base64_encode(from.c_str(), from.length());
}
inline std::string r_base64_decode(std::string const& from)
{
    return r_base64_decode(from.c_str(), from.length());
}
}

#endif // __CACTUS_WIN32_COMMON_H__
