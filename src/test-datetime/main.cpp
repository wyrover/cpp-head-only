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

#define RUBY_CRITICAL(statements) do {statements;} while (0)
#define RUBY_CRITICAL if (0) {} else /* just remark */

/* winsock error map */
#include <errno.h>

#ifndef EWOULDBLOCK
# define EWOULDBLOCK		WSAEWOULDBLOCK
#endif
#ifndef EINPROGRESS
# define EINPROGRESS		WSAEINPROGRESS
#endif
#ifndef EALREADY
# define EALREADY		WSAEALREADY
#endif
#ifndef ENOTSOCK
# define ENOTSOCK		WSAENOTSOCK
#endif
#ifndef EDESTADDRREQ
# define EDESTADDRREQ		WSAEDESTADDRREQ
#endif
#ifndef EMSGSIZE
# define EMSGSIZE		WSAEMSGSIZE
#endif
#ifndef EPROTOTYPE
# define EPROTOTYPE		WSAEPROTOTYPE
#endif
#ifndef ENOPROTOOPT
# define ENOPROTOOPT		WSAENOPROTOOPT
#endif
#ifndef EPROTONOSUPPORT
# define EPROTONOSUPPORT	WSAEPROTONOSUPPORT
#endif
#ifndef ESOCKTNOSUPPORT
# define ESOCKTNOSUPPORT	WSAESOCKTNOSUPPORT
#endif
#ifndef EOPNOTSUPP
# define EOPNOTSUPP		WSAEOPNOTSUPP
#endif
#ifndef EPFNOSUPPORT
# define EPFNOSUPPORT		WSAEPFNOSUPPORT
#endif
#ifndef EAFNOSUPPORT
# define EAFNOSUPPORT		WSAEAFNOSUPPORT
#endif
#ifndef EADDRINUSE
# define EADDRINUSE		WSAEADDRINUSE
#endif
#ifndef EADDRNOTAVAIL
# define EADDRNOTAVAIL		WSAEADDRNOTAVAIL
#endif
#ifndef ENETDOWN
# define ENETDOWN		WSAENETDOWN
#endif
#ifndef ENETUNREACH
# define ENETUNREACH		WSAENETUNREACH
#endif
#ifndef ENETRESET
# define ENETRESET		WSAENETRESET
#endif
#ifndef ECONNABORTED
# define ECONNABORTED		WSAECONNABORTED
#endif
#ifndef ECONNRESET
# define ECONNRESET		WSAECONNRESET
#endif
#ifndef ENOBUFS
# define ENOBUFS		WSAENOBUFS
#endif
#ifndef EISCONN
# define EISCONN		WSAEISCONN
#endif
#ifndef ENOTCONN
# define ENOTCONN		WSAENOTCONN
#endif
#ifndef ESHUTDOWN
# define ESHUTDOWN		WSAESHUTDOWN
#endif
#ifndef ETOOMANYREFS
# define ETOOMANYREFS		WSAETOOMANYREFS
#endif
#ifndef ETIMEDOUT
# define ETIMEDOUT		WSAETIMEDOUT
#endif
#ifndef ECONNREFUSED
# define ECONNREFUSED		WSAECONNREFUSED
#endif
#ifndef ELOOP
# define ELOOP			WSAELOOP
#endif
/*#define ENAMETOOLONG	WSAENAMETOOLONG*/
#ifndef EHOSTDOWN
# define EHOSTDOWN		WSAEHOSTDOWN
#endif
#ifndef EHOSTUNREACH
# define EHOSTUNREACH		WSAEHOSTUNREACH
#endif
/*#define ENOTEMPTY	WSAENOTEMPTY*/
#ifndef EPROCLIM
# define EPROCLIM		WSAEPROCLIM
#endif
#ifndef EUSERS
# define EUSERS			WSAEUSERS
#endif
#ifndef EDQUOT
# define EDQUOT			WSAEDQUOT
#endif
#ifndef ESTALE
# define ESTALE			WSAESTALE
#endif
#ifndef EREMOTE
# define EREMOTE		WSAEREMOTE
#endif



/* errno mapping */
static struct {
    DWORD winerr;
    int err;
} errmap[] = {
    {	ERROR_INVALID_FUNCTION,		EINVAL		},
    {	ERROR_FILE_NOT_FOUND,		ENOENT		},
    {	ERROR_PATH_NOT_FOUND,		ENOENT		},
    {	ERROR_TOO_MANY_OPEN_FILES,	EMFILE		},
    {	ERROR_ACCESS_DENIED,		EACCES		},
    {	ERROR_INVALID_HANDLE,		EBADF		},
    {	ERROR_ARENA_TRASHED,		ENOMEM		},
    {	ERROR_NOT_ENOUGH_MEMORY,	ENOMEM		},
    {	ERROR_INVALID_BLOCK,		ENOMEM		},
    {	ERROR_BAD_ENVIRONMENT,		E2BIG		},
    {	ERROR_BAD_FORMAT,		ENOEXEC		},
    {	ERROR_INVALID_ACCESS,		EINVAL		},
    {	ERROR_INVALID_DATA,		EINVAL		},
    {	ERROR_INVALID_DRIVE,		ENOENT		},
    {	ERROR_CURRENT_DIRECTORY,	EACCES		},
    {	ERROR_NOT_SAME_DEVICE,		EXDEV		},
    {	ERROR_NO_MORE_FILES,		ENOENT		},
    {	ERROR_WRITE_PROTECT,		EROFS		},
    {	ERROR_BAD_UNIT,			ENODEV		},
    {	ERROR_NOT_READY,		ENXIO		},
    {	ERROR_BAD_COMMAND,		EACCES		},
    {	ERROR_CRC,			EACCES		},
    {	ERROR_BAD_LENGTH,		EACCES		},
    {	ERROR_SEEK,			EIO		},
    {	ERROR_NOT_DOS_DISK,		EACCES		},
    {	ERROR_SECTOR_NOT_FOUND,		EACCES		},
    {	ERROR_OUT_OF_PAPER,		EACCES		},
    {	ERROR_WRITE_FAULT,		EIO		},
    {	ERROR_READ_FAULT,		EIO		},
    {	ERROR_GEN_FAILURE,		EACCES		},
    {	ERROR_LOCK_VIOLATION,		EACCES		},
    {	ERROR_SHARING_VIOLATION,	EACCES		},
    {	ERROR_WRONG_DISK,		EACCES		},
    {	ERROR_SHARING_BUFFER_EXCEEDED,	EACCES		},
    {	ERROR_BAD_NETPATH,		ENOENT		},
    {	ERROR_NETWORK_ACCESS_DENIED,	EACCES		},
    {	ERROR_BAD_NET_NAME,		ENOENT		},
    {	ERROR_FILE_EXISTS,		EEXIST		},
    {	ERROR_CANNOT_MAKE,		EACCES		},
    {	ERROR_FAIL_I24,			EACCES		},
    {	ERROR_INVALID_PARAMETER,	EINVAL		},
    {	ERROR_NO_PROC_SLOTS,		EAGAIN		},
    {	ERROR_DRIVE_LOCKED,		EACCES		},
    {	ERROR_BROKEN_PIPE,		EPIPE		},
    {	ERROR_DISK_FULL,		ENOSPC		},
    {	ERROR_INVALID_TARGET_HANDLE,	EBADF		},
    {	ERROR_INVALID_HANDLE,		EINVAL		},
    {	ERROR_WAIT_NO_CHILDREN,		ECHILD		},
    {	ERROR_CHILD_NOT_COMPLETE,	ECHILD		},
    {	ERROR_DIRECT_ACCESS_HANDLE,	EBADF		},
    {	ERROR_NEGATIVE_SEEK,		EINVAL		},
    {	ERROR_SEEK_ON_DEVICE,		EACCES		},
    {	ERROR_DIR_NOT_EMPTY,		ENOTEMPTY	},
    {	ERROR_DIRECTORY,		ENOTDIR		},
    {	ERROR_NOT_LOCKED,		EACCES		},
    {	ERROR_BAD_PATHNAME,		ENOENT		},
    {	ERROR_MAX_THRDS_REACHED,	EAGAIN		},
    {	ERROR_LOCK_FAILED,		EACCES		},
    {	ERROR_ALREADY_EXISTS,		EEXIST		},
    {	ERROR_INVALID_STARTING_CODESEG,	ENOEXEC		},
    {	ERROR_INVALID_STACKSEG,		ENOEXEC		},
    {	ERROR_INVALID_MODULETYPE,	ENOEXEC		},
    {	ERROR_INVALID_EXE_SIGNATURE,	ENOEXEC		},
    {	ERROR_EXE_MARKED_INVALID,	ENOEXEC		},
    {	ERROR_BAD_EXE_FORMAT,		ENOEXEC		},
    {	ERROR_ITERATED_DATA_EXCEEDS_64k,ENOEXEC		},
    {	ERROR_INVALID_MINALLOCSIZE,	ENOEXEC		},
    {	ERROR_DYNLINK_FROM_INVALID_RING,ENOEXEC		},
    {	ERROR_IOPL_NOT_ENABLED,		ENOEXEC		},
    {	ERROR_INVALID_SEGDPL,		ENOEXEC		},
    {	ERROR_AUTODATASEG_EXCEEDS_64k,	ENOEXEC		},
    {	ERROR_RING2SEG_MUST_BE_MOVABLE,	ENOEXEC		},
    {	ERROR_RELOC_CHAIN_XEEDS_SEGLIM,	ENOEXEC		},
    {	ERROR_INFLOOP_IN_RELOC_CHAIN,	ENOEXEC		},
    {	ERROR_FILENAME_EXCED_RANGE,	ENOENT		},
    {	ERROR_NESTING_NOT_ALLOWED,	EAGAIN		},
#ifndef ERROR_PIPE_LOCAL
#define ERROR_PIPE_LOCAL	229L
#endif
    {	ERROR_PIPE_LOCAL,		EPIPE		},
    {	ERROR_BAD_PIPE,			EPIPE		},
    {	ERROR_PIPE_BUSY,		EAGAIN		},
    {	ERROR_NO_DATA,			EPIPE		},
    {	ERROR_PIPE_NOT_CONNECTED,	EPIPE		},
    {	ERROR_OPERATION_ABORTED,	EINTR		},
    {	ERROR_NOT_ENOUGH_QUOTA,		ENOMEM		},
    {	ERROR_MOD_NOT_FOUND,		ENOENT		},
    {	ERROR_PRIVILEGE_NOT_HELD,	EACCES,		},
    {	ERROR_CANT_RESOLVE_FILENAME,	ELOOP,		},
    {	WSAEINTR,			EINTR		},
    {	WSAEBADF,			EBADF		},
    {	WSAEACCES,			EACCES		},
    {	WSAEFAULT,			EFAULT		},
    {	WSAEINVAL,			EINVAL		},
    {	WSAEMFILE,			EMFILE		},
    {	WSAEWOULDBLOCK,			EWOULDBLOCK	},
    {	WSAEINPROGRESS,			EINPROGRESS	},
    {	WSAEALREADY,			EALREADY	},
    {	WSAENOTSOCK,			ENOTSOCK	},
    {	WSAEDESTADDRREQ,		EDESTADDRREQ	},
    {	WSAEMSGSIZE,			EMSGSIZE	},
    {	WSAEPROTOTYPE,			EPROTOTYPE	},
    {	WSAENOPROTOOPT,			ENOPROTOOPT	},
    {	WSAEPROTONOSUPPORT,		EPROTONOSUPPORT	},
    {	WSAESOCKTNOSUPPORT,		ESOCKTNOSUPPORT	},
    {	WSAEOPNOTSUPP,			EOPNOTSUPP	},
    {	WSAEPFNOSUPPORT,		EPFNOSUPPORT	},
    {	WSAEAFNOSUPPORT,		EAFNOSUPPORT	},
    {	WSAEADDRINUSE,			EADDRINUSE	},
    {	WSAEADDRNOTAVAIL,		EADDRNOTAVAIL	},
    {	WSAENETDOWN,			ENETDOWN	},
    {	WSAENETUNREACH,			ENETUNREACH	},
    {	WSAENETRESET,			ENETRESET	},
    {	WSAECONNABORTED,		ECONNABORTED	},
    {	WSAECONNRESET,			ECONNRESET	},
    {	WSAENOBUFS,			ENOBUFS		},
    {	WSAEISCONN,			EISCONN		},
    {	WSAENOTCONN,			ENOTCONN	},
    {	WSAESHUTDOWN,			ESHUTDOWN	},
    {	WSAETOOMANYREFS,		ETOOMANYREFS	},
    {	WSAETIMEDOUT,			ETIMEDOUT	},
    {	WSAECONNREFUSED,		ECONNREFUSED	},
    {	WSAELOOP,			ELOOP		},
    {	WSAENAMETOOLONG,		ENAMETOOLONG	},
    {	WSAEHOSTDOWN,			EHOSTDOWN	},
    {	WSAEHOSTUNREACH,		EHOSTUNREACH	},
    {	WSAEPROCLIM,			EPROCLIM	},
    {	WSAENOTEMPTY,			ENOTEMPTY	},
    {	WSAEUSERS,			EUSERS		},
    {	WSAEDQUOT,			EDQUOT		},
    {	WSAESTALE,			ESTALE		},
    {	WSAEREMOTE,			EREMOTE		},
};

/* License: Ruby's */
int
rb_w32_map_errno(DWORD winerr)
{
    int i;

    if (winerr == 0) {
        return 0;
    }

    for (i = 0; i < (int)(sizeof(errmap) / sizeof(*errmap)); i++) {
        if (errmap[i].winerr == winerr) {
            return errmap[i].err;
        }
    }

    if (winerr >= WSABASEERR) {
        return winerr;
    }
    return EINVAL;
}

#define map_errno rb_w32_map_errno


/* License: Ruby's */
char *
rb_w32_wstr_to_mbstr(UINT cp, const WCHAR *wstr, int clen, long *plen)
{
    char *ptr;
    int len = WideCharToMultiByte(cp, 0, wstr, clen, NULL, 0, NULL, NULL);
    if (!(ptr = (char*)malloc(len))) return 0;
    WideCharToMultiByte(cp, 0, wstr, clen, ptr, len, NULL, NULL);
    if (plen) {
        /* exclude NUL only if NUL-terminated string */
        if (clen == -1) --len;
        *plen = len;
    }
    return ptr;
}

/* License: Ruby's */
WCHAR *
rb_w32_mbstr_to_wstr(UINT cp, const char *str, int clen, long *plen)
{
    WCHAR *ptr;
    int len = MultiByteToWideChar(cp, 0, str, clen, NULL, 0);
    if (!(ptr = (WCHAR*)malloc(sizeof(WCHAR) * len))) return 0;
    MultiByteToWideChar(cp, 0, str, clen, ptr, len);
    if (plen) {
        /* exclude NUL only if NUL-terminated string */
        if (clen == -1) --len;
        *plen = len;
    }
    return ptr;
}

UINT rb_w32_filecp(void);

#define filecp rb_w32_filecp
#define mbstr_to_wstr rb_w32_mbstr_to_wstr
#define wstr_to_mbstr rb_w32_wstr_to_mbstr
#define acp_to_wstr(str, plen) mbstr_to_wstr(CP_ACP, str, -1, plen)
#define wstr_to_acp(str, plen) wstr_to_mbstr(CP_ACP, str, -1, plen)
#define filecp_to_wstr(str, plen) mbstr_to_wstr(filecp(), str, -1, plen)
#define wstr_to_filecp(str, plen) wstr_to_mbstr(filecp(), str, -1, plen)
#define utf8_to_wstr(str, plen) mbstr_to_wstr(CP_UTF8, str, -1, plen)
#define wstr_to_utf8(str, plen) wstr_to_mbstr(CP_UTF8, str, -1, plen)


static int
wlink(const WCHAR *from, const WCHAR *to)
{
    if (!CreateHardLinkW(to, from, NULL)) {
        errno = map_errno(GetLastError());
        return -1;
    }

    return 0;
}



static int
wunlink(const WCHAR *path)
{
    int ret = 0;
    const DWORD SYMLINKD = FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY;
    RUBY_CRITICAL {
        const DWORD attr = GetFileAttributesW(path);
        if (attr == (DWORD)-1) {
        }
        else if ((attr & SYMLINKD) == SYMLINKD) {
            ret = RemoveDirectoryW(path);
        }
        else {
            if (attr & FILE_ATTRIBUTE_READONLY) {
                SetFileAttributesW(path, attr & ~FILE_ATTRIBUTE_READONLY);
            }
            ret = DeleteFileW(path);
        }
        if (!ret) {
            errno = map_errno(GetLastError());
            ret = -1;
            if (attr != (DWORD)-1 && (attr & FILE_ATTRIBUTE_READONLY)) {
                SetFileAttributesW(path, attr);
            }
        }
    }
    return ret;
}

int
rb_w32_uunlink(const char *path)
{
    WCHAR *wpath;
    int ret;

    if (!(wpath = utf8_to_wstr(path, NULL)))
        return -1;
    ret = wunlink(wpath);
    free(wpath);
    return ret;
}



int main()
{
#ifdef DEBUG
    _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_LEAK_CHECK_DF);
#endif
    
    return 0;
}

