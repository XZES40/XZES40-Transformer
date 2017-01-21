/*
 * 'C' Source Library - Common Utilities - Supported by GNU and Windows VC7.1
 *
 * Copyright (c) 2008 - This is open source software in the pulbic domain.
 *
 * Developed by: Steven J. Hathaway
 *
 * For:  State of Oregon - Oregon State Police / CJIS-LEDS
 */

#ifndef OR_MSGLOG_PLATFORM_H
#define OR_MSGLOG_PLATFORM_H

#if defined(_WIN32) && defined(__GNUC__)
 #error Conflicting WIN32/GNUC compilation... Aborting
#endif


/*
 * GNU programming may need some standard WINDOWS data type names.
 * The "gwinbase.h" supplies these for __GNUC__ compilation.
 */

#if defined(__GNUC__)
 #include "gwinbase.h"
#endif

/*
 * These are useful in creating and referencing shared dynamic libraries.
 */

#if defined(__GNUC__)
 #define PLATFORM_IMPORT
 #define PLATFORM_EXPORT
#endif

#if defined(_WIN32)
 #define PLATFORM_IMPORT __declspec(dllimport)
 #define PLATFORM_EXPORT __declspec(dllexport)
#endif

#define BEGIN_C_CODE #if defined(__cplusplus) extern "C" { #endif

#define END_C_CODE #if defined(__cplusplus) } #endif

/*
 * These definitions are useful for scanning directories and
 * creating filename paths.
 */

#if defined(__GNUC__)
#include <unistd.h>
#include <linux/limits.h>
#define MAXPATHLEN PATH_MAX
#define DIRSLASH "/"
#endif

#if defined(_WIN32)
/*
 * <limits.h>	Imported if _POSIX_ is defined
 *		NAME_MAX		255
 * <io.h>		
 *	The second set of low-level I/O functions are included
 *	only if both _POSIX_ and __STDC__ are undefined
 *		_open		open
 *		_close		close
 *		_read		read
 *		_write		write
 *		_lseek		lseek
 */

/*
 * Be sure to include <winsock2.h> before <windows.h>
 * thus preventing the inclusion of older <winsock.h>
 */
#include <winsock2.h>
#include <windows.h>
#include <limits.h>
#include <io.h>
/* NAME_MAX is defined when compiled with _POSIX_ */
/* But _POSIX_ and __STDC__ break other stuff */
#ifndef NAME_MAX
  #define NAME_MAX 255
#endif
#define MAXPATHLEN MAX_PATH
#define DIRSLASH "\\"
#endif	/* _WIN32 */

/* NOTES:  File I/O Offset defined in <sys/types>
 *
 *  Microsoft uses "_off_t" as the file offset value type.
 *  GNU open source uses "off_t" as the effective offset value type.
 *
 *   __GNUC__  if defined(__USE_FILE_OFFSET64)
 *     typedef __off64_t off_t;
 *   else
 *     typedef __off_t off_t;
 *
 *   _WIN32 defines only _off_t as a long integer in the crt.
 *   And "off_t" is defined only if "__STDC__" is undefined.
 *   The maximum file offset is therefore restricted to 2 GB.
 *
 *   __GNUC__ does not define _off_t.
 */

#include <sys/types.h>
#if (defined(_WIN32) && !defined(off_t))
 typedef _off_t off_t;
#endif

/* MORE USEFUL DEFINITIONS */
#ifndef TRUE
#define TRUE			1
#endif
#ifndef FALSE
#define FALSE			0
#endif
#ifndef RETURN_SUCCESS
#define RETURN_SUCCESS		0
#endif
#ifndef RETURN_FAILURE
#define RETURN_FAILURE		1
#endif
/* Note: Microsoft uses #define INVALID_HANDLE_VALUE ((void *)(-1)) */
#if defined(__GNUC__)
 #ifndef INVALID_HANDLE_VALUE
 #define INVALID_HANDLE_VALUE	NULL
 #endif
#endif

#endif /* OR_MSGLOG_PLATFORM_H */



