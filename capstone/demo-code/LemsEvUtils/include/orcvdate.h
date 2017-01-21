/*
 * File: orcvdate.h
 *---------------------------------------------------------------------
 *
 *	Copyright (c) 2005-2009 Oregon State Police
 *	Law Enforcement Data System, Salem, Oregon
 *
 *		This code is in the public domain.
 *
 * $Log:   G:/PVCS/Projects/LEMS2003/archives/mp/source/orcvdate.c-arch $
 *
 *    Rev 2.0   Sep 18 2009             Hathaway - Oregon CJIS-LEDS
 * Add more output formatting options to output month names with mixed case
 * (initial cap:  Mon or Month). Add the ability to format the message
 * by recognizing TODAY, TOMORROW, and YESTERDAY as valid dates based on
 * the current system time clock.  Handle periods between m.d.y components.
 * Handle comma between day and year "September 9, 2009".
 *
 *    Rev 1.9   Aug 14 2009             Hathaway - Oregon CJIS-LEDS
 * To accommodate end-of-year records and timezone/date skew, advance
 * the floating window algorithm by 2 days and 2 hours.
 *
 *    Rev 1.8   Apr 17 2009             Hathaway - Oregon CJIS-LEDS
 * Add output formats of DD MONTH CCYY.
 *
 *    Rev 1.7   Apr 04 2009             Hathaway - Oregon CJIS-LEDS
 * Add output formats that contain slashes instead of hyphens.
 *
 *    Rev 1.5   October 2007            Hathaway - Oregon CJIS-LEDS
 * Incorporate "ccwin" argument. Make callable as a custom XSLT XPATH
 * function using the Apache XALAN library. "lx:cvdate(srcdate,fmt,ccwin)"
 * Allow the .h file to be used with C++ compilers.
 *
 *    Rev 1.4   Jun 2005                hathaway
 * Add more input formats "DD-Month-CCYY" "CCYY-Month-DD"
 * Function cvdate() should probably become a utility under the
 * /lems2k/ut/source directory.
 *
 */

#if !defined(ORCVDATE_HEADER_GUARD_20071019)
#define ORCVDATE_HEADER_GUARD_20071019

/*
   Output Date Formats:

	"YR2"  YY
	"YR4"  CCYY
	"FD6"  YYMMDD
	"FD8"  CCYYMMDD
	"FH6"  YY-MM-DD
	"FH8"  CCYY-MM-DD
	"ND6"  MMDDYY
	"ND8"  MMDDCCYY
	"NH6"  MM-DD-YY
	"NH8"  MM-DD-CCYY
	"FS6"  YY/MM/DD
	"FS8"  CCYY/MM/DD
	"NS6"  MM/DD/YY
	"NS8"  MM/DD/CCYY
	"MYR"  MON CCYY
	"MXR"  MONTH CCYY
	"MDY"  MON DD CCYY
	"MDX"  MONTH DD, CCYY
	"DM6"  DD MON YY
	"DM8"  DD MON CCYY
	"DX6"  DD MONTH YY
	"DX8"  DD MONTH CCYY 
	"IDT"  CCYY-MM-DDT00:00:00

	"LYR"  Mon CCYY
	"LXR"  Month CCYY
	"LDY"  Mon DD CCYY
	"LDX"  Month DD, CCYY
	"DL6"  DD Mon YY
	"DL8"  DD Mon CCYY
	"XL6"  DD Month YY
	"XL8"  DD Month CCYY
	"DH6"  DD-MM-YY
	"DH8"  DD-MM-CCYY
	"DS6"  DD/MM/YY
	"DS8"  DD/MM/CCYY
*/

#if defined(__cplusplus)
extern "C"
{
#endif

/* The ORcvdate formatted string is statically allocated in global memory space
 * with no requirement to deallocate.  This function is not "thread safe" because
 * additional calls to this function will overwrite the static return buffer.
 */

//char * ORcvdate( char *indate, int inlen, char *fmt, int ccwin, char * tzone );
const char * ORcvdate(const char *indate, size_t inlen, const char *fmt, int ccwin);

#if defined(__cplusplus)
}
#endif

#endif // ORCVDATE_HEADER_GUARD_20071019

