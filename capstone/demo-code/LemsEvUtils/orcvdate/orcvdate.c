/*
 * File: orcvdate.c
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
 *    Rev 1.6   -- removed -- experiments for XML based timezone analysis
 *
 *    Rev 1.5   October 2007            Hathaway - Oregon CJIS-LEDS
 *
 * Incorporate "ccwin" argument. Make callable as a custom XSLT XPATH
 * function using the Apache XALAN library. "lx:cvdate(srcdate,fmt,ccwin)"
 * Add recognition of XML timezone delimiters.
 *
 *    Rev 1.4   Jun 2005                hathaway
 *
 * Add more input formats "DD-Month-CCYY" "CCYY-Month-DD"
 * Function cvdate() should probably become a utility under the
 * /lems2k/ut/source directory.
 *
 *    Rev 1.3	April 2005		Hathaway - Oregon CJIS-LEDS
 *
 * Major rewrite of date parsing rules - incorporated as cvdate().
 * Allow as input most any format that resembles a date.
 * Use a 10-year floating year window to determine "YY" century.
 * Rewrite convertDate() to call cvdate(), preserving old operations.
 * The cvdate() allo0ws more output format options, and allows
 * many non-standard and hyphenated date conventions for input.
 *
 *    Rev 1.2	Dec 10 2003 19:28:26	lems
 *
 * Preserve the convertDate() to process date-ranges and the
 * library of validation algorithms.  Preserve the operation of
 * getType() to support these and other field reformat functions.
 * No changes are made to the include file timeconv.h.
 *
 *
 * ISSUES:
 * 1>	Parsing errors keep the input field unchanged and return a
 *	pointer to the input field.  This preserves ad-hoc text identifiers
 *	that are required of some data systems.
 *
 * 2>	Zeros for month and day are allowed.  Some databases
 *	interpret day "00" to render the entire month, and interpret
 *  month "00" to render an entire year.  NCIC may complain with an
 *  EquipmentFailure message on such dates.  This NCIC message is a
 *  date format fault instead of an actual equipment failure.
 *
 * 3>	There is no attempt to recompute a valid date if the day-of-month
 *  is invalid for the specified month. The formatted date will still
 *	return the invalid day-of-month in the output.
 *
 * 4>	Input dates can accommodate hyphens, spaces, commas, periods and
 * slashes as date component delimiters.  Months can be rendered as numbers
 * and as text strings which match on the first three characters.  Month
 * names can be upper-case or initial-cap or lower-case.
 *
 * 5>	A 3-digit year is allowed, because some systems render
 *	year "000" as 1900, and "100" as 2000 and "200" as 2100.  There are
 *  still some parsing issues where this assumption fails.
 *
 * 6>	A Day-Month-Year input sequence for date strings is supported
 *	only if Month is text and Year is 4-digits.  The output sequence
 *	is always independent of the input sequence.
 *
 * 7>	A YYMMDD input date is recognized only if YY > 12.  Otherwise the 
 *	six character date is recognized as MMDDYY on input.
 *
 * 8>	MMDDCCYY input is recognized if MM <= 12.
 *		CCYYMMDD input is recognized if CC > 12.
 *
 * 9>	Special date strings of YESTERDAY, TODAY, and TOMORROW obtain the
 *	current date from the system clock and then format the appropriate
 *	computed system date.
*/

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Note:  The DATEWINDOW is defined to support the code requirements of the
 * UNISYS LEMS message switch baseline code.  For our use of the library in
 * other products, we supply an explicit year-window parameter.
 */

#define DATEWINDOW 10


/*------- ORcvdate() -- DESIGN NOTES --------------

   The ORcvdate() allows input to look like most anything that
   resembles a date.  The following input strings are accepted:

      TODAY
	  TOMORROW
	  YESTERDAY

      YY, xYY, CCYY,
      MMDDYY, YYMMDD (YY>12), xYYMMDD (7char),
      MMDDCCYY, CCYYMMDD (CC>12),
      M.-D.-YY, M.-D.-xYY, M.-D.-CCYY,
      YY-M.-D. (YY>12), xYY-M.-D. (xYY>12), CCYY-M.-D.,
      month-YY, month-xYY, month-CCYY, D.-month-CCYY,
      month-D.-YY, month-D.-xYY, month-D.-CCYY

      In cases of xYY, CC = 19 + x
      In cases of M., MM = MM or M
      In cases of D., DD = DD or D
 
      Parsing of '-' matches any of {hyphen slash period comma space}

      Parsing of 'month' matches any of
          Jan JAN jan January JANUARY january
          Feb FEB feb February FEBRUARY february
          Mar MAR mar March MARCH march
          Apr APR apr April APRIL april
          May MAY may May MAY may
          Jun JUN jun June JUNE june
          Jul JUL jul July JULY july
          Aug AUG aug August AUGUST august
          Sep SEP sep September SEPTEMBER september
          Oct OCT oct October OCTOBER october
          Nov NOV nov November NOVEMBER november
          Dec DEC dec December DECEMBER december
      Only the first three characters of 'month' are examined.

   If output format code is unknown, the input string is returned.

   Implement a 10-year floating century window, replacing the
	50/50 window, and other complex computation requirements.
	Most license expirations and record expirations occur
	within 10 years.  Most CJIS databases do not recognize
	persons older than 89 years.  If dates greater than
	10 years future or less than 89 years past are required,
	then the incoming date must specify a century. 

   Century Determination: YY using DATEWINDOW 10
	If (base-century + YY) is greater than 10 years future,
	then (base-century -100) is used.  This is a true 10-year
	floating window to handle registration and record expirations.
	Base-century = today + 2Days + 2 Hours + DATEWINDOW years.

   NCIC Faults:
	NCIC processes reports Equipment Failure when receiving DOB/
	with month and/or day set to zero when DOB/ is the only
	numeric search field.  This is not a true Equipment Failure.
	NCIC is aware of the issue.  It is up to the user to fix the
	date and reenter the command.

   We will not attempt to force MM or DD of zeros to value "01".
	We allow the database systems to report the error and have
	the users resubmit their command with fixed dates.

	Some database systems use DD = "00" to imply the entire month.
	They will also interpret MM = "00" to imply the entire year.

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

----------------------------------------------------
*/


const struct monthStrings {
	char	StdMonth[4];
	char	LongMonth[10];
	char	SearchMonth[10];
	char	StdMonthIc[4];
	char	LongMonthIc[10];
} MonthStrings[13] = {{"   ","UNKNOWN","   ","   ","   "},
	{"JAN","JANUARY","JanJANjan","Jan","January"},
	{"FEB","FEBRUARY","FebFEBfeb","Feb","February"},
	{"MAR","MARCH","MarMARmar","Mar","March"},
	{"APR","APRIL","AprAPRapr","Apr","April"},
	{"MAY","MAY","MayMAYmay","May","May"},
	{"JUN","JUNE","JunJUNjun","Jun","June"},
	{"JUL","JULY","JulJULjul","Jul","July"},
	{"AUG","AUGUST","AugAUGaug","Aug","August"},
	{"SEP","SEPTEMBER","SepSEPsep","Sep","September"},
	{"OCT","OCTOBER","OctOCToct","Oct","October"},
	{"NOV","NOVEMBER","NovNOVnov","Nov","November"},
	{"DEC","DECEMBER","DecDECdec","Dec","December"}};

/*
 * ORcvdate :: The Conversion Function
 *
 * Note - The static allocation of return string is not thread safe.
 *
*/

//char * ORcvdate( char *indate, int inlen, char *fmt, int ccwin, char * tzone )
const char * ORcvdate(const char *indate, size_t inlen, const char *fmt, int ccwin)
{
  struct tm tm_str;
  struct tm *tm_ptr;
  struct tm tm_today;
  struct tm tm_yesterday;
  struct tm tm_tomorrow;
  char itemp[20];
  static char odate[20];		// Static Allocation Return String
  time_t epoc_time;
  int t1, t2, t3, t4, i, imonth, len;
  char cc[3] = "00";
  char yy[3] = "00";
  char mm[3] = "00";
  char dd[3] = "00";
  char *p1 = NULL;
  char *p2 = NULL;
  char *p3 = NULL;

/* validate function input */

	if (indate == NULL)
		return indate;

    if (fmt == NULL || (strlen(fmt) != 3) || inlen < 2 || inlen >17 )
	return indate;

    if (!strstr("YR2 YR4 FD6 FD8 FH6 FH8 ND6 ND8 NH6 NH8 FS6 FS8 NS6 NS8 "
				"MYR MXR MDY MDX DM6 DM8 DX6 DX8 IDT"
				"LYR LXR LDY LDX DL6 DL8 XL6 XL8 DH6 DH8 DS6 DS8",fmt))
	return indate;	/* Unimplemented Format */

/* find date values for YESTERDAY, TODAY and TOMORROW */

	epoc_time = time(NULL);
	memcpy((void*)&tm_today, (void*)localtime(&epoc_time), sizeof(struct tm));
	memcpy((void*)&tm_yesterday, (void*)&tm_today, sizeof(struct tm));
	memcpy((void*)&tm_tomorrow, (void*)&tm_today, sizeof(struct tm));
	tm_yesterday.tm_mday--;
	tm_tomorrow.tm_mday++;
	mktime(&tm_yesterday);
	mktime(&tm_tomorrow);

/* left justify indate field */

    for (i = 0; i < (int)inlen; i++)
    {
	if (indate[i] != ' ')
		break;
    }

    strncpy(itemp, (indate + i), (inlen - i));
    itemp[(inlen - i)] = '\0';

    len = (int)strlen(itemp);

/* trim trailing space */

    for (i = len - 1; i > 0 ; i--)
    {
	if (itemp[i] != ' ')
		break;
	itemp[i] = '\0';
    }
    len = (int)strlen(itemp);

/* custom for YESTERDAY, TODAY, TOMORROW */

	if (!strcmp(itemp, "YESTERDAY"))
	{
		sprintf(cc,"%02d",(tm_yesterday.tm_year / 100) + 19);
		sprintf(yy,"%02d",tm_yesterday.tm_year % 100);
		sprintf(mm,"%02d",tm_yesterday.tm_mon + 1);
		sprintf(dd,"%02d",tm_yesterday.tm_mday);
		goto FORMAT_OUTDATE;
	}
	else if (!strcmp(itemp, "TODAY"))
	{
		sprintf(cc,"%02d",(tm_today.tm_year / 100) + 19);
		sprintf(yy,"%02d",tm_today.tm_year % 100);
		sprintf(mm,"%02d",tm_today.tm_mon + 1);
		sprintf(dd,"%02d",tm_today.tm_mday);
		goto FORMAT_OUTDATE;
	}
	else if (!strcmp(itemp, "TOMORROW"))
	{
		sprintf(cc,"%02d",(tm_tomorrow.tm_year / 100) + 19);
		sprintf(yy,"%02d",tm_tomorrow.tm_year % 100);
		sprintf(mm,"%02d",tm_tomorrow.tm_mon + 1);
		sprintf(dd,"%02d",tm_tomorrow.tm_mday);
		goto FORMAT_OUTDATE;
	}

/* Parse the input date */

/* Check for delimited date components - we even support dates
 * of numeric strings.
*/

    p1 = strtok(itemp, "/-., ");
    p2 = strtok(NULL, "/-., ");
    p3 = strtok(NULL, "/-:T+ ");

    if ( len <= 8 && (!p2) && (itemp[0] < '0' || itemp[0] > '9'))
		return indate;	/* Unparsable Text */

/* Get today's date + 2day + 2hr for for 10-year window computation of "YY" */

    epoc_time = time(NULL);
	epoc_time += 180000;	/* add 2 days = 172800 seconds, 2 hr = 7200 seconds */
    tm_ptr = localtime(&epoc_time);
    memcpy(&tm_str, tm_ptr, sizeof(tm_str));

    if (len <= 4)		/* We have YY, xYY, or CCYY */
    {
		t1 = atoi(itemp);
		sprintf(yy,"%02d",(t1 % 100));

		if (t1 >= 1000)
		{
		    cc[0] = itemp[0];
		    cc[1] = itemp[1];
		    cc[2] = '\0';
		}
		else if (len > 2)
		{
		    sprintf(cc,"%02d",((t1 / 100) + 19));
		}
		else
		{
//		    t2 = ((tm_str.tm_year + DATEWINDOW) / 100) * 100;
//		    if (( t1 + t2 ) > (tm_str.tm_year + DATEWINDOW))
		    t2 = ((tm_str.tm_year + ccwin) / 100) * 100;
		    if (( t1 + t2 ) > (tm_str.tm_year + ccwin))
			t2--;
		    sprintf(cc,"%02d",((t2 + 1900) / 100));
		}
    }
    else if (p2)	/* We have delimited date parts */
    {

	/* t1 =   CCYY or MM   or DD     or CCYY   or CCYY        */
	/* t2 =   MM   or DD   or mon(0) or mon(0) or mon(0)      */
	/* t3 =   DD   or CCYY or CCYY   or DD     or (null)      */
	/* t4 =   (0) number of month in case (p2) is named month */

		t1 = atoi(p1);
		t2 = atoi(p2);
		t3 = p3 ? atoi(p3) : 0;
		t4 = 0;

		if (p1[0] >= '0' && p1[0] <= '9')	/* Numeric First p1 */
		{
		    if ( strchr("JFMASOND", p2[0]))
		    {

			if (strlen(p2) >3)
			    p2[3] = '\0';
			for ( t4 = 0; t4 < 13; t4++ )
			{
			    if (strstr(MonthStrings[t4].SearchMonth, p2))
				break;
			}
			if (t4 == 13)
			{
			    return indate;
			}
			if (p3 && (t3 > 31))	/* DD-month-CCYY */
			{
			    t2 = t1;
			    t1 = t3;
			    t3 = t2;
			    t2 = t4;
			    p1 = p3;		/* p1 is now = year */
			}
			else				/* CCYY-month-DD */
			{
			    t2 = t4;		/* p1 is already = year */
			}
		/* t1 = year */
		/* t2 = month */
		/* t3 = day */
	    }
	    else if (!p3)
	    {
			return indate;	/* parse error - insufficient number of parts */
	    }
	    else if (strlen(p1) > 4 || strlen(p3) > 4)
	    {
			return indate;
	    }
	    else if ((t1 <= 12) && strlen(p1) <= 2)	/* MM-DD-CCYY */
	    {
			t4 = t3;	/* year */
			t3 = t2;	/* day */
			t2 = t1;	/* month */
			t1 = t4;	/* t1, t2, t3 = year, month, day */
			p1 = p3;	/* p1 is now = year */
	    }

	    sprintf(dd, "%02d", (t3 % 100));
	    sprintf(mm, "%02d", (t2 % 100));
	    sprintf(yy, "%02d", (t1 % 100));
	    if (t1 >= 1000)
	    {
			cc[0] = p1[0];
			cc[1] = p1[1];
			cc[2] = '\0';
	    }
	    else if (strlen(p1) > 2)
	    {
			sprintf(cc, "%02d", ((t1 / 100) + 19));
	    }
	    else	/* Window the Century */
	    {
//			t2 = ((tm_str.tm_year + DATEWINDOW) / 100) * 100;
//			if (( t1 + t2 ) > (tm_str.tm_year + DATEWINDOW))
			t2 = ((tm_str.tm_year + ccwin) / 100) * 100;
			if (( t1 + t2 ) > (tm_str.tm_year + ccwin))
				t2--;
			sprintf(cc,"%02d",((t2 + 1900) / 100));
	    }
	}
	else		/* Look for named month in p1 */
	{
		itemp[3] = '\0';
		for (imonth = 0; imonth < 13; imonth++)
		{
			if (strstr(MonthStrings[imonth].SearchMonth, p1))
				break;
		}

		if (imonth == 13)
			return indate;	/* Unrecognized Month Name */

		sprintf(mm,"%02d",imonth);
		if (p3)		/* Found three date parts */
		{
		    sprintf(dd,"%02d",atoi(p2));
		}
		else
		{
		    p3 = p2;	/* To process the year */
		}
		t1 = atoi(p3);
		i = (int)strlen(p3);
		sprintf(yy,"%02d",(t1 % 100));
		if (t1 >= 1000)	/* have CCYY */
		{
		    cc[0] = p3[0];
		    cc[1] = p3[1];
		    cc[2] = '\0';
		}
		else if (i > 2)	/* have xYY */
		{
		    sprintf(cc,"%02d",((t1 / 100) + 19));
		}
		else		/* have YY - Window the Century */
		{
//		    t2 = ((tm_str.tm_year + DATEWINDOW) / 100) * 100;
//		    if (( t1 + t2 ) > (tm_str.tm_year + DATEWINDOW))
		    t2 = ((tm_str.tm_year + ccwin) / 100) * 100;
		    if (( t1 + t2 ) > (tm_str.tm_year + ccwin))
			t2--;
		    sprintf(cc,"%02d",((t2 + 1900) / 100));
		}
	}
  }
  else if (len == 5)	/* Cannot parse 12345 */
	return indate;
  else if (len == 6)	/* have YYMMDD or MMDDYY */
  {
	mm[0] = itemp[0];
	mm[1] = itemp[1];
	mm[2] = '\0';
	if (atoi(mm) < 13)
	{
	    dd[0] = itemp[2];
	    dd[1] = itemp[3];
	    dd[2] = '\0';
	    yy[0] = itemp[4];
	    yy[1] = itemp[5];
	    yy[2] = '\0';
	}
	else
	{
	    yy[0] = itemp[0];
	    yy[1] = itemp[1];
	    yy[2] = '\0';
	    mm[0] = itemp[2];
	    mm[1] = itemp[3];
	    mm[2] = '\0';
	    dd[0] = itemp[4];
	    dd[1] = itemp[5];
	    dd[2] = '\0';
	}
	t1 = atoi(yy);
//	t2 = ((tm_str.tm_year + DATEWINDOW) / 100) * 100;
//	if (( t1 + t2 ) > (tm_str.tm_year + DATEWINDOW))
	t2 = ((tm_str.tm_year + ccwin) / 100) * 100;
	if (( t1 + t2 ) > (tm_str.tm_year + ccwin))
		t2--;
	sprintf(cc,"%02d",((t2 + 1900) / 100));
  }
  else if (len == 7)		/* Have xYYMMDD */
  {
	if (itemp[0] < '0' || itemp[0] > '9')
		return indate;	/* parse error - not numeric */
	yy[0]=itemp[1];
	yy[1]=itemp[2];
	yy[2]='\0';
	mm[0]=itemp[3];
	mm[1]=itemp[4];
	mm[2]='\0';
	dd[0]=itemp[5];
	dd[1]=itemp[6];
	dd[2]='\0';
	itemp[3] = '\0';
	sprintf(cc,"%02d",((atoi(itemp) / 100) + 19)); 
  }
  else if (len == 8)		/* Have CCYYMMDD or MMDDCCYY */
  {
	cc[0] = itemp[0];
	cc[1] = itemp[1];
	cc[2] = '\0';
	if (atoi(cc) > 12)	/* Found CCYYMMDD */
	{
		yy[0] = itemp[2];
		yy[1] = itemp[3];
		yy[2] = '\0';
		mm[0] = itemp[4];
		mm[1] = itemp[5];
		mm[2] = '\0';
		dd[0] = itemp[6];
		dd[1] = itemp[7];
		dd[2] = '\0';
	}
	else				/* Have MMDDCCYY */
	{
		mm[0] = itemp[0];
		mm[1] = itemp[1];
		mm[2] = '\0';
		dd[0] = itemp[2];
		dd[1] = itemp[3];
		dd[2] = '\0';
		cc[0] = itemp[4];
		cc[1] = itemp[5];
		cc[2] = '\0';
		yy[0] = itemp[6];
		yy[1] = itemp[7];
		yy[2] = '\0';
	}
	t1 = atoi(cc);
	if (t1 < 10)
		sprintf(cc,"%02d",(t1 + 19));
  }
  else
  {
	return (indate);
  }

/* Finished Input Date Parsing into variables: cc, yy, mm, dd */
/* We have already returned indate on input date parsing failures */

FORMAT_OUTDATE:

/* Prepare Output Date */

  imonth = atoi(mm);
  if (imonth < 0 || imonth > 12)
	return (indate);

  if (!strcmp("YR2",fmt))		/* YY */
	sprintf(odate,"%2.2s",yy);

  else if (!strcmp("YR4",fmt))	/* CCYY */
	sprintf(odate,"%2.2s%2.2s",cc,yy);

  else if (!strcmp("FD6",fmt))	/* YYMMDD */
	sprintf(odate,"%2.2s%2.2s%2.2s",yy,mm,dd);

  else if (!strcmp("FD8",fmt))	/* CCYYMMDD */
	sprintf(odate,"%2.2s%2.2s%2.2s%2.2s",cc,yy,mm,dd);

  else if (!strcmp("FH6",fmt))	/* YY-MM-DD */
	sprintf(odate,"%2.2s-%2.2s-%2.2s",yy,mm,dd);

  else if (!strcmp("FH8",fmt))	/* CCYY-MM-DD */
	sprintf(odate,"%2.2s%2.2s-%2.2s-%2.2s",cc,yy,mm,dd);

  else if (!strcmp("ND6",fmt))	/* MMDDYY */
	sprintf(odate,"%2.2s%2.2s%2.2s",mm,dd,yy);

  else if (!strcmp("ND8",fmt))	/* MMDDCCYY */
	sprintf(odate,"%2.2s%2.2s%2.2s%2.2s",mm,dd,cc,yy);

  else if (!strcmp("NH6",fmt))	/* MM-DD-YY */
	sprintf(odate,"%2.2s-%2.2s-%2.2s",mm,dd,yy);

  else if (!strcmp("NH8",fmt))	/* MM-DD-CCYY */
	sprintf(odate,"%2.2s-%2.2s-%2.2s%2.2s",mm,dd,cc,yy);

  else if (!strcmp("FS6",fmt))	/* YY/MM/DD */
	sprintf(odate,"%2.2s/%2.2s/%2.2s",yy,mm,dd);

  else if (!strcmp("FS8",fmt))	/* CCYY/MM/DD */
	sprintf(odate,"%2.2s%2.2s/%2.2s/%2.2s",cc,yy,mm,dd);

  else if (!strcmp("NS6",fmt))	/* MM/DD/YY */
	sprintf(odate,"%2.2s/%2.2s/%2.2s",mm,dd,yy);

  else if (!strcmp("NS8",fmt))	/* MM/DD/CCYY */
	sprintf(odate,"%2.2s/%2.2s/%2.2s%2.2s",mm,dd,cc,yy);

  else if (!strcmp("IDT",fmt))	/* CCYY-MM-DDT00:00:00 */
	sprintf(odate,"%2.2s%2.2s-%2.2s-%2.2sT00:00:00",cc,yy,mm,dd);

  else if (!strcmp("MYR",fmt))	/* MON CCYY */
	sprintf(odate,"%s %2.2s%2.2s", 
	MonthStrings[imonth].StdMonth,cc,yy);

  else if (!strcmp("MXR",fmt))	/* MONTH CCYY */
	sprintf(odate,"%s %2.2s%2.2s",
	MonthStrings[imonth].LongMonth,cc,yy);

  else if (!strcmp("MDY",fmt))	/* MON DD CCYY */
	sprintf(odate,"%s %2.2s %2.2s%2.2s",
	MonthStrings[imonth].StdMonth,dd,cc,yy);

  else if (!strcmp("MDX",fmt))	/* MONTH DD CCYY */
	sprintf(odate,"%s %2.2s, %2.2s%2.2s",
	MonthStrings[imonth].LongMonth,dd,cc,yy);

  else if (!strcmp("DM6",fmt))	/* DD MON YY */
	sprintf(odate,"%2.2s %s %2.2s",
	dd,MonthStrings[imonth].StdMonth,yy);

  else if (!strcmp("DM8",fmt))	/* DD MON CCYY */
	sprintf(odate,"%2.2s %s %2.2s%2.2s",
	dd,MonthStrings[imonth].StdMonth,cc,yy);

  else if (!strcmp("DX6",fmt))	/* DD MONTH YY */
	sprintf(odate,"%2.2s %s %2.2s",
	dd,MonthStrings[imonth].LongMonth,yy);

  else if (!strcmp("DX8",fmt))	/* DD MONTH CCYY */
	sprintf(odate,"%2.2s %s %2.2s%2.2s",
	dd,MonthStrings[imonth].LongMonth,cc,yy);

  else if (!strcmp("LYR",fmt))	/* Mon CCYY */
	sprintf(odate,"%s %2.2s%2.2s",
	MonthStrings[imonth].StdMonthIc,cc,yy);

  else if (!strcmp("LXR",fmt))	/* Month CCYY */
	sprintf(odate,"%s %2.2s%2.2s",
	MonthStrings[imonth].LongMonthIc,cc,yy);

  else if (!strcmp("LDY",fmt))	/* Mon DD CCYY */
	sprintf(odate,"%s %2.2s %2.2s%2.2s",
	MonthStrings[imonth].StdMonthIc,dd,cc,yy);

  else if (!strcmp("LDX",fmt))	/* Month DD CCYY */
	sprintf(odate,"%s %2.2s, %2.2s%2.2s",
	MonthStrings[imonth].LongMonthIc,dd,cc,yy);

  else if (!strcmp("DL6",fmt))	/* DD Mon YY */
	sprintf(odate,"%2.2s %s %2.2s",
	dd,MonthStrings[imonth].StdMonthIc,yy);

  else if (!strcmp("DL8",fmt))	/* DD Mon CCYY */
	sprintf(odate,"%2.2s %s %2.2s%2.2s",
	dd,MonthStrings[imonth].StdMonthIc,cc,yy);

  else if (!strcmp("XL6",fmt))	/* DD Month YY */
	sprintf(odate,"%2.2s %s %2.2s",
	dd,MonthStrings[imonth].LongMonthIc,yy);

  else if (!strcmp("XL8",fmt))	/* DD Month CCYY */
	sprintf(odate,"%2.2s %s %2.2s%2.2s",
	dd,MonthStrings[imonth].LongMonthIc,cc,yy);

  else if (!strcmp("DM6",fmt))	/* DDMMYY */
	sprintf(odate,"%2.2s%2.2s%2.2s",
	dd,mm,yy);

  else if (!strcmp("DM8",fmt))	/* DDMMCCYY */
	sprintf(odate,"%2.2s%2.2s%2.2s%2.2s",
	dd,mm,cc,yy);

  else if (!strcmp("DH6",fmt))	/* DD-MM-YY */
	sprintf(odate,"%2.2s-%2.2s-%2.2s",
	dd,mm,yy);

  else if (!strcmp("DH8",fmt))	/* DD-MM-CCYY */
	sprintf(odate,"%2.2s-%2.2s-%2.2s%2.2s",
	dd,mm,cc,yy);

  else if (!strcmp("DS6",fmt))	/* DD-MM-YY */
	sprintf(odate,"%2.2s/%2.2s/%2.2s",
	dd,mm,yy);

  else if (!strcmp("DS8",fmt))	/* DD-MM-CCYY */
	sprintf(odate,"%2.2s/%2.2s/%2.2s%2.2s",
	dd,mm,cc,yy);

  else 
	return (indate);
  return (odate);
}

#if (0)

int main (int argc, char *argv[])
{
  int yrwin;
  if (argc != 4)
  {
    printf("Need Parameters: indate, fmt, window\n");
    exit (1);
  }

  yrwin = atoi(argv[3]);
  printf("\nConverted Date:\"%s\"\n",
    ORcvdate(argv[1], (int)strlen(argv[1]), argv[2], yrwin));
  exit(0);
}

#endif

