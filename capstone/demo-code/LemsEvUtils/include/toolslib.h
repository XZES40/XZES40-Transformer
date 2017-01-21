/**
 * Tools Library:  toolslib.h
 *
 * This is the combined header file for 
 *
 *     btcomp.c			btcomp.h
 *     findexpr.c		fmtlib.h
 *     trimsp.c			fmtlib.h
 *     debuglog.c		debuglog.h
 *     charzero.c		charzero.h
 *     fixcjisids.c		fixcjisids.h
 */

#ifndef LEMS_LOG_TOOLS_COMBINED_H
#define LEMS_LOG_TOOLS_COMBINED_H

#include <time.h>
#include <stdarg.h>

/* TrimSpaces()
 * Edits a string-buffer in place and returns the supplied pointer.
 * Leading and trailing spaces are removed.  All interior white space is preserved.
 */

char * TrimSpaces(char * theString);

/* RemoveSpaces()
 * Edits a null-terminated string-buffer in place and returns the supplied pointer.
 * All space 0x20 characters are removed from the string.
 */

char * RemoveSpaces(char * ptr);

/* TrimInnerSpaces()
 * Edits a null-terminated string-buffer in place and returns the supplied pointer.
 * Leading and trailing 0x20 space characters are removed from the string.
 * Adjacent 0x20 characters are reduced to one space character.
 */

char * TrimInnerSpaces(char * ptr);


/************************************/
/**  Compare (a, b)                **/
/**  +1 = (a > b)                  **/
/**   0 = (a = b)                  **/
/**  -1 = (a < b)                  **/
/**                                **/
/************************************/

/* Integer Comparison */

int intcomp(int a, int b);

/* Time Structure (tm) Comparison */

int tmcomp (const struct tm *tm1, const struct tm * tm2);

/* FindExprList() and FindExprTable()
 *
 * Search a haystack message, and return a pointer to the first match
 * of several expression strings.  NULL is returned if no match is found.
 */

char * FindExprList(char *haystack, int count, ...);

char * FindExprTable(char *haystack, int count, const char * var[]);

/* ZeroToOh
 *
 * Edit buffer in-place, converting digits Zero to letters Oh
 * And returns a pointer to the buffer.
 *
 * theBuf = buffer to edit
 * theLimit = maximum characters in buffer to edit
 * endToken = token character to terminate the edit
 */

char * ZeroToOh(char * theBuf, int theLimit, char endToken);

/* OhToZero
 *
 * Edit buffer in-place, converting letters Oh to digits Zero
 * And returns a pointer to the buffer.
 *
 * theBuf = buffer to edit
 * theLimit = maximum characters in buffer to edit
 * endToken = token character to terminate the edit
 */

char * OhToZero(char * theBuf, int theLimit, char endToken);


/**
 * FixSidFormat
 *
 * The sidState gives a default state code if sidNumber is not 10 characters.
 * A sidNumber of 10 characters is assumed to have a state code prefix.
 * 
 * A sidNumber of 8 characters will have the sidState applied if found. Otherwise
 * the default "OR" state prefix will be applied.
 *
 * The state prefix will be ZeroToOh converted (MO, OR, OH, OK, IO, CO)
 * The sid number will be OhToZero converted - digits after state prefix.
 *
 * The function is not thread safe.  The result string is preallocated as static storage.
 *
 */

char * FixSidFormat(const unsigned char * sidState, const unsigned char * sidNumber);


/**
 * FixOriFormat
 *
 * Parameter theOri is assumed to be 9 characters.  The first 2 characters are ZeroToOh
 * converted.  The remaining characters are OhToZero converted.
 *
 * The function is not thread safe.  The result string is preallocated as static storage.
 */

char * FixOriFormat(const unsigned char * theOri);


void initDEBUGLOG(const char * LogFilePath);

void DEBUGLOG(const char * function, const char * message);

char * getDEBUGLOG(void);


#endif /* LEMS_LOG_TOOLS_COMBINED_H */

