/*
 * 'C' Source Library - Common Utilities - Supported by GNU and Windows VC7.1
 *
 * Copyright (c) 2008 - This is open source software in the pulbic domain.
 *
 * Developed by: Steven J. Hathaway
 *
 * For:  State of Oregon - Oregon State Police / CJIS-LEDS
 */

/************************************/
/**  Compare (a, b)                **/
/**  +1 = (a > b)                  **/
/**   0 = (a = b)                  **/
/**  -1 = (a < b)                  **/
/**                                **/
/************************************/

/*
 * This is a utility library of standard comparison functions.
 */

#include <time.h>

/* Integer Comparison */

int intcomp(int a, int b)
{
	if (a == b)
		return 0;
	if (a > b)
		return 1;
	else
		return -1;
}

/* Time Structure (tm) Comparison */

int tmcomp (const struct tm *tm1, const struct tm * tm2)
{
	int rtn;
	if (rtn = intcomp(tm1->tm_year, tm2->tm_year))
		return rtn;
	if (rtn = intcomp(tm1->tm_mon, tm2->tm_mon))
		return rtn;
	if (rtn = intcomp(tm1->tm_mday, tm2->tm_mday))
		return rtn;
	if (rtn = intcomp(tm1->tm_hour, tm2->tm_hour))
		return rtn;
	if (rtn = intcomp(tm1->tm_min, tm2->tm_min))
		return rtn;
	return intcomp(tm1->tm_sec, tm2->tm_sec);
}
