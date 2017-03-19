

#include <SDKDDKVer.h>
#include <api/win32_common.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <WinIoCtl.h>

#define _Field_size_bytes_part_(s,c)

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
#ifdef MIDL_PASS
    [size_is(MaximumLength / 2), length_is((Length) / 2)] PUSHORT Buffer;
#else
    _Field_size_bytes_part_(MaximumLength, Length) PWCH Buffer;
#endif
} UNICODE_STRING, *PUNICODE_STRING;
typedef const UNICODE_STRING* PCUNICODE_STRING;

typedef struct _CURDIR
{
    UNICODE_STRING DosPath;
    HANDLE Handle;
} CURDIR, *PCURDIR;

#define _ANONYMOUS_UNION

/* There is no API for creating junctions, so we must do it the hard way */
static BOOL CreateJunction(LPCTSTR LinkName, LPCTSTR TargetName)
{
    /* Structure for reparse point daya when ReparseTag is one of
     * the tags defined by Microsoft. Copied from MinGW winnt.h */
    typedef struct _REPARSE_DATA_BUFFER
    {
        DWORD  ReparseTag;
        WORD   ReparseDataLength;
        WORD   Reserved;
        _ANONYMOUS_UNION union
        {
            struct
            {
                WORD   SubstituteNameOffset;
                WORD   SubstituteNameLength;
                WORD   PrintNameOffset;
                WORD   PrintNameLength;
                ULONG  Flags;
                WCHAR PathBuffer[1];
            } SymbolicLinkReparseBuffer;
            struct
            {
                WORD   SubstituteNameOffset;
                WORD   SubstituteNameLength;
                WORD   PrintNameOffset;
                WORD   PrintNameLength;
                WCHAR PathBuffer[1];
            } MountPointReparseBuffer;
            struct
            {
                BYTE   DataBuffer[1];
            } GenericReparseBuffer;
        } DUMMYUNIONNAME;
    } REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;

    HMODULE hNTDLL = GetModuleHandle(_T("NTDLL"));
    BOOLEAN (WINAPI *RtlDosPathNameToNtPathName_U)(PCWSTR, PUNICODE_STRING, PCWSTR *, CURDIR *)
        = (BOOLEAN (WINAPI *)(PCWSTR, PUNICODE_STRING, PCWSTR *, CURDIR *))GetProcAddress(hNTDLL, "RtlDosPathNameToNtPathName_U");
    VOID (WINAPI *RtlFreeUnicodeString)(PUNICODE_STRING)
        = (VOID (WINAPI *)(PUNICODE_STRING))GetProcAddress(hNTDLL, "RtlFreeUnicodeString");

    TCHAR TargetFullPath[MAX_PATH];
#ifdef UNICODE
    #define TargetFullPathW TargetFullPath
#else
    WCHAR TargetFullPathW[MAX_PATH];
#endif
    UNICODE_STRING TargetNTPath;
    HANDLE hJunction;

    /* The data for this kind of reparse point has two strings:
     * The first ("SubstituteName") is the full target path in NT format,
     * the second ("PrintName") is the full target path in Win32 format.
     * Both of these must be wide-character strings. */
    if (!RtlDosPathNameToNtPathName_U ||
        !RtlFreeUnicodeString ||
        !GetFullPathName(TargetName, MAX_PATH, TargetFullPath, NULL) ||
#ifndef UNICODE
        !MultiByteToWideChar(CP_ACP, 0, TargetFullPath, -1, TargetFullPathW, -1) ||
#endif
        !RtlDosPathNameToNtPathName_U(TargetFullPathW, &TargetNTPath, NULL, NULL))
    {
        return FALSE;
    }

    /* We have both the names we need, so time to create the junction.
     * Start with an empty directory */
    if (!CreateDirectory(LinkName, NULL))
    {
        RtlFreeUnicodeString(&TargetNTPath);
        return FALSE;
    }

    /* Open the directory we just created */
    hJunction = CreateFile(LinkName, GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (hJunction != INVALID_HANDLE_VALUE)
    {
        /* Allocate a buffer large enough to hold both strings, including trailing NULs */
        SIZE_T TargetLen = wcslen(TargetFullPathW) * sizeof(WCHAR);
        DWORD DataSize = (DWORD)(FIELD_OFFSET(REPARSE_DATA_BUFFER, MountPointReparseBuffer.PathBuffer)
                          + TargetNTPath.Length + sizeof(WCHAR)
                          + TargetLen           + sizeof(WCHAR));
        PREPARSE_DATA_BUFFER Data = (PREPARSE_DATA_BUFFER)_alloca(DataSize);

        /* Fill it out and use it to turn the directory into a reparse point */
        Data->ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
        Data->ReparseDataLength = (WORD)(DataSize - FIELD_OFFSET(REPARSE_DATA_BUFFER, MountPointReparseBuffer));
        Data->Reserved = 0;
        Data->MountPointReparseBuffer.SubstituteNameOffset = 0;
        Data->MountPointReparseBuffer.SubstituteNameLength = TargetNTPath.Length;
        wcscpy(Data->MountPointReparseBuffer.PathBuffer,
               TargetNTPath.Buffer);
        Data->MountPointReparseBuffer.PrintNameOffset = TargetNTPath.Length + sizeof(WCHAR);
        Data->MountPointReparseBuffer.PrintNameLength = (USHORT)TargetLen;
        wcscpy((WCHAR *)((BYTE *)Data->MountPointReparseBuffer.PathBuffer
                         + Data->MountPointReparseBuffer.PrintNameOffset),
                        TargetFullPathW);
        if (DeviceIoControl(hJunction, FSCTL_SET_REPARSE_POINT,
                            Data, DataSize, NULL, 0, &DataSize, NULL))
        {
            /* Success */
            CloseHandle(hJunction);
            RtlFreeUnicodeString(&TargetNTPath);
            return TRUE;
        }
        CloseHandle(hJunction);
    }
    RemoveDirectory(LinkName);
    RtlFreeUnicodeString(&TargetNTPath);
    return FALSE;
}

int main()
{
#ifdef DEBUG
    _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_LEAK_CHECK_DF);
#endif

    CreateJunction(L"d:\\111", L"d:\\222");

    system("pause");
    
    return 0;
}

