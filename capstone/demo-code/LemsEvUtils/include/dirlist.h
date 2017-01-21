/*
 * 'C' Source Library - Common Utilities - Supported by GNU and Windows VC7.1
 *
 * Copyright (c) 2008 - This is open source software in the pulbic domain.
 *
 * Developed by: Steven J. Hathaway
 *
 * For:  State of Oregon - Oregon State Police / CJIS-LEDS
 */

#ifndef	OR_DIRLIST_H
#define OR_DIRLIST_H

#include "platform.h"

/*
 * This (dirlist) library creates a set of standard interface definitions that
 * that are compatible between Microsoft VC7.1 and GNU/Linux systems.  The 
 * implementation uses a DIRHANDLE structure similar to Microsoft but exposes
 * an underlying structure content type.  The function names are similar to
 * those used by Microsoft for directory searches.
 *
 * The directory search information is stored in the structure referenced by
 * the DIRHANDLE.  The DIRHANDLE structure is allocated and initialized by a
 * call to FindFirstDirEntry() and deallocated by a call to CloseFindDir().
 *
 * This library is not compatible with Microsoft UNICODE which allows network
 * node addressing and strings of upto 32767 wide characters.  Thus the 
 * restriction on Microsoft systems is 260 characters, the maximum device:path
 * string allowed by Microsoft file systems.  On GNU systems, the path length
 * is expanded to support UNIX capable file systems.
 *
 * "platform.h" defines MAXPATHLEN to indicate the actual maximum path length.
 * For GNU systems MAXPATHLEN = PATH_MAX.  For Windows MAXPATHLEN = MAX_PATH.
 */

#if defined(__GNUC__)
#include "gwinbase.h"
#endif

#if defined(_WIN32)
#if defined(UNICODE)
#error Microsoft UNICODE directive is is unsupported. TCHAR type becomes wide.
#endif /* UNICODE */

/* DIRHANDLE_STRUCT is the common structure used by the routines in this library
 * that provide a common interface that can be supported on a wide number of
 * operating system platforms. There are two forms of this structure.  One is
 * supported by the Microsoft Visual C and Studio .NET platforms.  The other is
 * supported by the GNU C/C++ compilers.
 *
 * DIRHANDLE_STRUCT is the directory traversal and control structure. With
 * UNIX like systems, the (DIR *) and (struct dirent) are incorporated into
 * the structure.  With Microsoft WIN32 systems, the HANDLE and WIN32_FIND_DATA
 * are incorporated into the structure.
 */

typedef struct _dir_handle				/* the FindFirstDirEntry Control */
{	HANDLE				d_open_dir;		/* the WIN32 directory stream */
	char				*d_base_dir;	/* the directory name */
	char				*d_find_text;	/* the filename match string */
	char				*d_full_path;	/* the directory plus filename */
	WIN32_FIND_DATA		d_dirent;		/* the WIN32 directory info */
}	DIRHANDLE_STRUCT,	*DIRHANDLE;		/* the FindFirstDirEntry handle */
#endif /*_WIN32*/

#if defined(__GNUC__)
#include <dirent.h>

typedef struct _dir_handle				/* the FindFirstDirEntry Control */
{	DIR	*				d_open_dir;		/* the POSIX directory stream */
	char				*d_base_dir;	/* the directory name */
	char				*d_find_text;	/* the filename match string */
	char				*d_full_path;	/* the directory plus filename */
	struct dirent		d_dirent;		/* the POSIX directory info */
}	DIRHANDLE_STRUCT,	*DIRHANDLE;		/* the FindFirstDirEntry handle */
#endif /*__GNUC__*/

/* The following routines extract in a system-independent mannner, the
 * information that is found in a DIRHANDLE_STRUCT as returned by
 * FindFirstDirEntry, modified by FindNextDirEntry, and released by
 * CloseFindDir.
 *
 * TRUE = 1, FALSE = 0;
 *
 * Microsoft only knows of two directory entry types {File and Directory}
 * supported by FAT12, FAT16, FAT32, and NTFS file systems.
 * UNIX like systems have more file types inclding File, Directory, Link,
 * FIFO Pipe, Socket, Block Device, and Character Device.
 */

int DirentIsFile(const DIRHANDLE dirInfo);	/* returns TRUE or FALSE */

int DirentIsDirectory(const DIRHANDLE dirInfo);	/* returns TRUE or FALSE */

int DirentIsLink(const DIRHANDLE dirInfo);	/* returns TRUE or FALSE */

int DirentIsFIFO(const DIRHANDLE dirInfo);	/* returns TRUE or FALSE */

int DirentIsSocket(const DIRHANDLE dirInfo);	/* returns TRUE or FALSE */

int DirentIsBlockDev(const DIRHANDLE dirInfo);	/* returns TRUE or FALSE */

int DirentIsCharDev(const DIRHANDLE dirInfo);	/* returns TRUE or FALSE */

/* DirentGetFileName returns the filename portion of the directory entry */

char * DirentGetFileName(const DIRHANDLE dirInfo);	/* allocated in DIRHANDLE_STRUCT */

/* DirentGetFullPath returns the full pathname associated with the directory entry */

char * DirentGetFullPath(const DIRHANDLE dirInfo);	/* returns static alloc string */
/**= We should localize the full pathname to the DIRHANDLE_STRUCT, especially
 * if we are considering making a dynamic shared library of these routines.
 * This change will help make the library thread-safe.
 */

/*
 * FindFirstDirEntry, FindNextDirEntry, CloseFindDir
 *
 * These functions perform functions equivalent to Microsoft Windows
 * FindFirstFile, FindNextFile, and FindClose but use only one structure type
 * so that memory allocation can be well managed.
 *
 * These functions may fail if lookup_filename or its path expansion
 * cannot fit in MAXPATHLEN.  For GNU the value is 4096 chars. For
 * WIN32 the value is 260 chars.  The Microsoft convention for long
 * path names with \\.\ or \\?\ prefixes is not supported -- these require
 * the Microsoft UNICODE compilation and implement filenames and
 * directory names as strings of wchar.  The length for Microsoft UNICODE
 * strings is increased to 32767 wide characters.
 */

/* DIRHANDLE_STRUCT, *DIRHANDLE
 *
 * Even though they have characteristics of a Microsoft HANDLE, the DIRHANDLE pointer
 * is incompatible with Microsoft CloseHandle and FindClose functions.
 */

/* FindFirstDirEntry
 *
 * The argument is a directory and filename to lookup.  The filename part may
 * have the '*' and '?' wildcards.  The string is limited to MAXPATHLEN and
 * the filename components are limited to NAME_MAX (255).
 * MAXPATHLEN = 260 (Microsoft)
 * MAXPATHLEN = 4096 (GNU)
 *
 * A successful return is a pointer to a DIRHANDLE_STRUCT directory control structure.
 * _WIN32   compilation returns INVALID_HANDLE_VALUE on error = ((void *)(-1)).
 * __GNUC__ complation returns NULL on error = ((void *)(0)).
 */

DIRHANDLE FindFirstDirEntry(const char * lookup_filename);

/* FindNextDirEntry
 *
 * Advances the directory stream to the next lookup_filename match as specified by
 * FindFirstDirEntry.
 *
 * Returns TRUE(1) if the next directory entry is found.  Otherwise FALSE(0)
 * is returned.
 */

int FindNextDirEntry(DIRHANDLE dirHandle);

/* CloseFindDir
 *
 * Closes the directory stream and deallocates the directory access control
 * structure referenced by DIRHANDLE.  The directory access control structure is
 * initially allocated by a successful return from FindFirstDirEntry.
 *
 * Returns zero(0) if successful.
 */

int CloseFindDir(DIRHANDLE dirHandle);

#endif		/* OR_DIRLIST_H */

