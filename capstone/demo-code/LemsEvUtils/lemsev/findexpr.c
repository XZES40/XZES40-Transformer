
/* Modification Histoy
 *
 *    Onsite.2     May 27 2008             hathaway
 * Add: TrimSpaces()
 *
 *    Onsite.1     May 25 2008             hathaway
 * Original Release: FindExprList()
 */

#include <stdlib.h>
	// defines malloc() and free()
#include <stdarg.h>
	// defines variable argument list macros va_list, va_start, va_arg, va_end
#include <string.h>
	// defines strstr() string search function

/* FindExprList()
 * Search a haystack message, and return a pointer to the first match
 * of several expression strings.  NULL is returned if no match is found.
 */

char * FindExprList(char *haystack, const int count, ...)
{
	va_list	ap;
	int		i;
	const char	*p1;
	char	*rtn;
	char	*p2;
	char	*in_string;		// for runtime search performance

	rtn = NULL;

	if (!(haystack) || (count < 1))
	{
		return rtn;
	}

	i = (int) strlen(haystack);
	if ((in_string=malloc(i + 1)) == NULL)
		return rtn;		// cannot allocate haystack copy

	memcpy(in_string,haystack,i);
	in_string[i] = '\0';

	va_start(ap, count);
	for (i=0; i<count; i++)
	{	
		p1=va_arg(ap, char *);
		p2=strstr(in_string, p1);
		if (p2)
		{
			*p2 = '\0';			// limit next search
			if (!rtn)
				rtn = p2;
			else
				rtn = (rtn < p2) ? rtn : p2;
		}
	}
	va_end(ap);
	if (rtn)
		rtn = (char *)(haystack + (rtn - in_string));
	free(in_string);
	return rtn;
}

/* FindExprTable()
 * Search a haystack message, and return a pointer to the first match
 * of several expression strings.  NULL is returned if no match is found.
 */

char * FindExprTable(char *haystack, const int count, const char * val[])
{
	int		i;
	char	*rtn;
	char	*p2;
	char	*in_string;		// for runtime search performance

	rtn = NULL;

	if (!(haystack) || (count < 1))
	{
		return rtn;
	}

	i = (int) strlen(haystack);
	if ((in_string=malloc(i + 1)) == NULL)
		return rtn;		// cannot allocate haystack copy

	memcpy(in_string,haystack,i);
	in_string[i] = '\0';

	for (i=0; i<count && val[i]; i++)
	{	
		p2=strstr(in_string, val[i]);
		if (p2)
		{
			*p2 = '\0';			// limit next search
			if (!rtn)
				rtn = p2;
			else
				rtn = (rtn < p2) ? rtn : p2;
		}
	}
	if (rtn)
		rtn = (char *)(haystack + (rtn - in_string));
	free(in_string);
	return rtn;
}
