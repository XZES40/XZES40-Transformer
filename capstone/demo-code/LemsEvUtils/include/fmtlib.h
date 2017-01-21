
/* Modification Histoy
 *
 *    Onsite.2     May 27 2008             hathaway
 * Add: TrimSpaces()
 *
 *    Onsite.1     May 25 2008             hathaway
 * Original Release: FindExprList()
 */


#ifndef OR_FMTLIB_H
#define OR_FMTLIB_H

/* FindExprList() and FindExprTable()
 *
 * Search a haystack message, and return a pointer to the first match
 * of several expression strings.
 */

char * FindExprList(char *haystack, const int count, ...);
char * FindExprTable(char *haystack, const int count, const char * var[]);

/* TrimSpaces()
 * Edits a string-buffer in place and returns the supplied pointer.
 * Leading and trailing spaces are removed.  All interior white space is preserved.
 */

char * TrimSpaces(char *txtBuf);

#endif