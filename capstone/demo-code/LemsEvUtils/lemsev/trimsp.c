
/* Modification Histoy
 *
 *    Onsite.2     Jul 31 2012             Hathaway
 * Add recognition of tab chars as whitespace.
 *
 *    Onsite.1     May 27 2008             Hathaway
 * Original Release: TrimSpaces()
 *
 */

#include <string.h>

/* TrimSpaces()
 * Edits a string-buffer in place and returns the supplied pointer.
 * Leading and trailing spaces are removed.  All interior white space is preserved.
 */

char * TrimSpaces(char * theString)
{
	register char * cp;
	register char * ep;
	register char * np;
	int	n;

	if (!theString)
		return theString;

	if (!(n = (int)strlen(theString)))
		return theString;

	cp = theString;
	for (ep = cp + n - 1; ep >= cp && (*ep == ' ' || *ep == '\t'); ep--)
		*ep = '\0';

	for (np = cp; (*np == ' ' || *np == '\t'); np++);

	while (np <= ep)
	{
		*cp = *np;
		cp++;
		np++;
	}

	while (cp <= ep)
	{
		*cp = '\0';
		cp++;
	}

	return theString;
}


/* RemoveSpaces()
 * Edits a null-terminated string-buffer in place and returns the supplied pointer.
 * All space 0x20 characters are removed from the string.
 */

char * RemoveSpaces(char * ptr)
{
	register char * p1;
	register char * p2;
	if (!ptr)
		return ptr;
	for (p1 = p2 = ptr; *p1; p1++)
	{
		if (*p1 == '\t') *p1 = ' ';
		if (*p1 == ' ')
			continue;
		*p2 = *p1;
		p2++;
	}
	*p2 = '\0';
	return ptr;
}


/* TrimInnerSpaces()
 * Edits a null-terminated string-buffer in place and returns the supplied pointer.
 * Leading and trailing 0x20 space characters are removed from the string.
 * Adjacent 0x20 characters are reduced to one space character.
 */

char * TrimInnerSpaces(char * ptr)
{
	register char *p1;
	register char *p2;
	register char *p3;
	
	if (!ptr)
		return ptr;

	for (p1 = p2 = p3 = ptr; *p1 ; p1++)
	{
		if (*p1 == '\t') *p1 = ' ';
		if (*p1 == ' ')
		{
			if (p1 == p3)
			{
				p2 = ++p3;
				continue;
			}
			if (p1[1] == ' ' || p1[1] == '\t' || p1[1] == '\0')
				continue;
		}
		*p2 = *p1;
		p2++;
	}
	*p2 = '\0';

	for (p1 = ptr; *p3; p1++, p3++)
		*p1 = *p3;
	*p1 = '\0';

	return ptr;
}