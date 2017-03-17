

#include "win32_common.h"
#include <vector>
#include <fstream>




std::string& trim(std::string& s)
{
	if (s.empty()) {
		return s;
	}

	std::string::iterator c;

	// Erase whitespace before the string
	for (c = s.begin(); c != s.end() && iswspace(*c++););

	s.erase(s.begin(), --c);

	// Erase whitespace after the string
	for (c = s.end(); c != s.begin() && iswspace(*--c););

	s.erase(++c, s.end());
	return s;
}

std::string trim(const std::string& str)
{
	std::string Result = str;
	Result.erase(0, Result.find_first_not_of(" /t/n/r")).erase(Result.find_last_not_of(" /t/n/r") + 1);
	return Result;
}

string8 hex_encode(const void* bytes, size_t size)
{
	static const char kHexChars[] = "0123456789ABCDEF";
	// Each input byte creates two output hex characters.
	string8 ret(size * 2, '\0');

	for (size_t i = 0; i < size; ++i) {
		char b = reinterpret_cast<const char*>(bytes)[i];
		ret[(i * 2)] = kHexChars[(b >> 4) & 0xf];
		ret[(i * 2) + 1] = kHexChars[b & 0xf];
	}

	return ret;
}


string16 formatBytesW(int64_t aBytes)
{
	char16 buf[64];

	if (aBytes < 1024) {
		_snwprintf(buf, _countof(buf), L"%d %s", (int)(aBytes & 0xffffffff), L"B");
	} else if (aBytes < 1048576) {
		_snwprintf(buf, _countof(buf), L"%.02f %s", (double)aBytes / (1024.0), L"KB");
	} else if (aBytes < 1073741824) {
		_snwprintf(buf, _countof(buf), L"%.02f %s", (double)aBytes / (1048576.0), L"MB");
	} else if (aBytes < (int64_t)1099511627776) {
		_snwprintf(buf, _countof(buf), L"%.02f %s", (double)aBytes / (1073741824.0), L"GB");
	} else if (aBytes < (int64_t)1125899906842624) {
		_snwprintf(buf, _countof(buf), L"%.03f %s", (double)aBytes / (1099511627776.0), L"TB");
	} else if (aBytes < (int64_t)1152921504606846976) {
		_snwprintf(buf, _countof(buf), L"%.03f %s", (double)aBytes / (1125899906842624.0), L"PB");
	} else {
		_snwprintf(buf, _countof(buf), L"%.03f %s", (double)aBytes / (1152921504606846976.0), L"EB"); //TODO Crash beta-16
	}

	return buf;
}

bool StartsWithPathDelimiter(const string16& path)
{
	return (path.find_first_of(PathDelimiters) == 0);
}

bool EndsWithPathDelimiter(const string16& path)
{
	return (path.find_last_of(PathDelimiters) + 1 == path.length());
}

string16 IncludeTrailingPathDelimiter(const string16& S)
{
	string16 Result = S;

	if (!EndsWithPathDelimiter(Result))
		Result = Result + PathDelim;

	return Result;
}

string16 ExcludeTrailingPathDelimiter(const string16& S)
{
	string16 Result = S;

	while (EndsWithPathDelimiter(Result))
		Result.erase(Result.length() - 1);

	return Result;
}

extern "C" IMAGE_DOS_HEADER __ImageBase;
string16 get_app_filename()
{
	char16 filename[MAX_PATH + 10] = {0};
	GetModuleFileNameW((HMODULE)&__ImageBase, filename, MAX_PATH);
	return string16(filename);
}

string16 get_app_dir()
{
	return extract_filepath(get_app_filename());
}


string16 get_cwd()
{
	char16 buffer[MAX_PATH + 1];
	::GetCurrentDirectory(MAX_PATH, buffer);

	if (wcslen(buffer) > 0)
		return string16(buffer);
	else
		return sWEmpty;
}

string16 get_current_working_dir()
{
	char16 Buffer[MAX_PATH + 1] = {0};
	_wgetcwd(Buffer, MAX_PATH);
	return string16(Buffer);
}

string16 get_system_drv()
{
	static string16 sysdrv;

	if (!sysdrv.empty())
		return sysdrv;

	std::vector<char16> buffer(MAX_PATH + 1);
	GetSystemDirectory(&buffer[0], MAX_PATH);
	sysdrv = string16(&buffer[0]);
	sysdrv = sysdrv.erase(sysdrv.find_first_of(L"\\/") + 1);
	return sysdrv;
}


string16 extract_filepath(const string16& filename)
{
	string16 Result = filename;
	return Result.erase(Result.find_last_of(PathDelimiters) + 1);
}

string16 extract_filename(const string16& filename)
{
	string16 Result = filename;
	return Result.erase(0, Result.find_last_of(PathDelimiters) + 1);
}

string16 extract_fileext(const string16& filename)
{
	string16 Result = filename;
	return Result.erase(0, Result.find_last_of(L'.') + 1);
}

string16 expand_filename(const string16& filename)
{
	char16* FName;
	char16 Buffer[MAX_PATH + 1] = {0};
	::GetFullPathNameW(filename.c_str(), MAX_PATH,  Buffer, &FName);

	if ((FName != NULL) && (FName != 0))
		return string16(Buffer);
	else
		return sWEmpty;
}

string16 change_fileext(const string16& filename, const string16& extension)
{
	string16 Result = filename;
	return Result.erase(Result.find_last_of(L'.') + 1) + extension;
}

string16 change_filepath(const string16& filename, const string16& Path)
{
	string16 Result = Path;
	return IncludeTrailingPathDelimiter(Result) + extract_filename(filename);
}

string16 extract_short_pathname(const string16& filename)
{
	char16 Buffer[MAX_PATH + 1] = {0};
	GetShortPathNameW(filename.c_str(), Buffer, MAX_PATH);
	return string16(Buffer);
}

string16 complete_path_with_modulepath(const string16& filename)
{
	char16 szPath[MAX_PATH] = {0};
	::GetModuleFileName((HMODULE)&__ImageBase, szPath, MAX_PATH);
	PathAppend(szPath, L"..\\");
	PathAppend(szPath, filename.c_str());
	string16 data = szPath;
	return data;
}

string16 get_parent_directory(const string16& path)
{
	string16::size_type pos = path.find_last_of(L'\\');

	if (pos != string16::npos) {
		string16 sPath = path.substr(0, pos);
		return sPath;
	}

	return path;
}


void file_unix_to_dos_path(char* lpPath)
{
	LPSTR p;	

	if (!lpPath)
		return;

	for (p = lpPath; *p; p++) {
		if (*p == '/')
			*p = '\\';
	}	
}

bool file_get_contents(const string8& filename, std::vector<char>& buffer)
{
	std::ifstream file(filename.c_str(), std::ios_base::binary);

	if (file) {
		file.seekg(0, std::ios_base::end);
		std::streamsize size = file.tellg();

		if (size > 0) {
			file.seekg(0, std::ios_base::beg);
			buffer.resize(static_cast<std::size_t>(size));
			file.read(&buffer[0], size);
		}

		buffer.push_back('\0');
		return true;
	} else {
		return false;
	}
}

bool file_get_contents(const string16& filename, std::vector<char>& buffer)
{
	std::ifstream file(filename.c_str(), std::ios_base::binary);

	if (file) {
		file.seekg(0, std::ios_base::end);
		std::streamsize size = file.tellg();

		if (size > 0) {
			file.seekg(0, std::ios_base::beg);
			buffer.resize(static_cast<std::size_t>(size));
			file.read(&buffer[0], size);
		}

		buffer.push_back('\0');
		return true;
	} else {
		return false;
	}
}


bool file_get_contents(const string16& filename, std::vector<string16>& list)
{
	std::fstream fin(filename.c_str());

	if (!fin)
		return false;

	std::string s;

	while (getline(fin, s)) {
		if (!s.empty()) {
			list.push_back(convert_to<string16>(trim(s)));
		}
	}

	return true;
}

bool file_get_contents(const string16& filename, std::set<string16>& list)
{
	std::fstream fin(filename.c_str());

	if (!fin)
		return false;

	std::string s;

	while (getline(fin, s)) {
		if (!s.empty()) {
			list.insert(convert_to<string16>(trim(s)));
		}
	}

	return true;
}


BOOL get_file_large_size(LPCWSTR lpFilePath, LARGE_INTEGER& liFileSize)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	LARGE_INTEGER larFileSize;

	if (!lpFilePath) {
		liFileSize.LowPart = 0xFFFFFFFE;
		return FALSE;
	}

	larFileSize.LowPart = 0;
	larFileSize.HighPart = 0;
	hFile = ::CreateFile(
		lpFilePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile) {
		liFileSize.LowPart = ::GetLastError();
		return FALSE;
	}

	if (::GetFileSizeEx(hFile, &larFileSize)) {
		liFileSize.LowPart = larFileSize.LowPart;
		liFileSize.HighPart = larFileSize.HighPart;
	} else {
		liFileSize.LowPart = ::GetLastError();
	}

	::CloseHandle(hFile);
	hFile = NULL;
	return TRUE;
}


void hex_dump_to_file(FILE* dmpf, const char* name, BYTE* addr, size_t size)
{
	if (!size)
		return;

	//assert(addr);
	fprintf(dmpf, "Hex dump of %s:\n", name);

	for (unsigned i = 0; i < size; i++) {
		if ((i % 16) == 0) {
			fprintf(dmpf, "\n    %04X: ", i);
		}

		fprintf(dmpf, "%02X ", *addr++);
	}

	fprintf(dmpf, "\n\n");
}

bool get_file_str_content(const string16& path, std::string* contents)
{
	FILE* file = OpenFile(path, "rb");

	if (!file) {
		return false;
	}

	char buf[1 << 16];
	size_t len;

	while ((len = fread(buf, 1, sizeof(buf), file)) > 0) {
		contents->append(buf, len);
	}

	if (file) {
		fclose(file);
		file = NULL;
	}
	
	return true;
}