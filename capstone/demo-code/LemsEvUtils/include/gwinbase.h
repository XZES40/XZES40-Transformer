/*
 * 'C' Source Library - Common Utilities - Supported by GNU and Windows VC7.1
 *
 * Copyright (c) 2008 - This is open source software in the pulbic domain.
 *
 * Developed by: Steven J. Hathaway
 *
 * For:  State of Oregon - Oregon State Police / CJIS-LEDS
 */

/*
 * Define basic windows type definitions for GNU applications
 */

#ifndef GNU_WINBASE_H
#define GNU_WINBASE_H
#if defined(__GNUC__)

#define VOID				void

#define CONST				const
#define NEAR
#define FAR
#define UNALIGNED

typedef int					BOOL;
typedef char				CHAR;
typedef unsigned char		UCHAR;
typedef unsigned char		BYTE;
typedef short int			SHORT;
typedef unsigned short int	USHORT;
typedef int					INT;
typedef unsigned int		UINT;
typedef long int			LONG;
typedef unsigned long int	ULONG;
typedef double				DOUBLE;

/* Items from Microsoft <windef.h> */

typedef ULONG *PULONG;
typedef USHORT *PUSHORT;
typedef UCHAR *PUCHAR;
typedef char *PSZ;
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#undef far
#undef near
#undef pascal

#define far
#define near
#define pascal

#undef PASCAL
#define PASCAL pascal

#undef FAR
#undef  NEAR
#define FAR                 far
#define NEAR                near

#ifndef CDECL
#define CDECL
#endif

typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
typedef BOOL near           *PBOOL;
typedef BOOL far            *LPBOOL;
typedef BYTE near           *PBYTE;
typedef BYTE far            *LPBYTE;
typedef int near            *PINT;
typedef int far             *LPINT;
typedef WORD near           *PWORD;
typedef WORD far            *LPWORD;
typedef long far            *LPLONG;
typedef DWORD near          *PDWORD;
typedef DWORD far           *LPDWORD;
typedef void far            *LPVOID;
typedef CONST void far      *LPCVOID;
typedef unsigned int        *PUINT;


/* Wide Character Support */

#include <wchar.h>
typedef wchar_t				WCHAR;
typedef char				TCHAR, *PTCHAR;
typedef BYTE				TBYTE, *PTBYTE;

/* Items from Microsoft <winnt.h> */

typedef WCHAR *PWCHAR;
typedef WCHAR *LPWCH, *PWCH;
typedef CONST WCHAR *LPCWCH, *PCWCH;
typedef WCHAR *NWPSTR;
typedef WCHAR *LPWSTR, *PWSTR;
typedef WCHAR UNALIGNED *LPUWSTR, *PUWSTR;

typedef CONST WCHAR *LPCWSTR, *PCWSTR;
typedef CONST WCHAR UNALIGNED *LPCUWSTR, *PCUWSTR;

typedef CHAR *PCHAR;
typedef CHAR *LPCH, *PCH;

typedef CONST CHAR *LPCCH, *PCCH;
typedef CHAR *NPSTR;
typedef CHAR *LPSTR, *PSTR;
typedef CONST CHAR *LPCSTR, *PCSTR;

typedef LPSTR LPTCH, PTCH;
typedef LPSTR PTSTR, LPTSTR, PUTSTR, LPUTSTR;
typedef LPCSTR PCTSTR, LPCTSTR, PCUTSTR, LPCUTSTR;

typedef SHORT *PSHORT;
typedef LONG *PLONG;

typedef void	*HANDLE;
typedef HANDLE	*PHANDLE;

typedef BYTE   FCHAR;
typedef WORD   FSHORT;
typedef DWORD  FLONG;

#ifdef __cplusplus
    #define EXTERN_C    extern "C"
#else
    #define EXTERN_C    extern
#endif

typedef BYTE  BOOLEAN;           
typedef BOOLEAN *PBOOLEAN;

/* Min and Max Values */

#define MINCHAR     0x80        
#define MAXCHAR     0x7f        
#define MINSHORT    0x8000      
#define MAXSHORT    0x7fff      
#define MINLONG     0x80000000  
#define MAXLONG     0x7fffffff  
#define MAXBYTE     0xff        
#define MAXWORD     0xffff      
#define MAXDWORD    0xffffffff

#endif /*__GNUC__*/
#endif /*GNU_WINBASE_H*/



