/*
 * 'C' Source Library - Common Utilities - Supported by GNU and Windows VC7.1
 *
 * Copyright (c) 2008 - This is open source software in the pulbic domain.
 *
 * Developed by: Steven J. Hathaway
 *
 * For:  State of Oregon - Oregon State Police / CJIS-LEDS
 */

#ifndef OR_MS_EVNT_DATE_H
#define OR_MS_EVNT_DATE_H

/* Resolve parent dependency header files */
#include "keylist.h"

#include <time.h>

/* ================================================================
 * char * MakeDateDirName
 *
 * NOTE: Result String has Static Allocation
 *
 * Create a string component with day of week and calendar day
 * to be used in creating a LEMS message switch logfile directory name.
 *
 * Input  : a time structure "tm" containing the date.
 * Output : a delimited string with "\weekday\ccyy-mm-dd" format.
 */

char * MakeDateDirName(const struct tm * dateStruct);

/* ================================================================
 * TmParseDate
 * 
 * NOTE: Result (struct tm) has Static Allocation
 *
 * Convert a string of "CCYY-MM-DD^HH:MM:SS" format into a struct tm.
 *
 * Internal field punctuation is ignored, but the numeric digits are significant.
 * The argeument string is to be at least 10 characters in length. The Date is
 * required, the Time component is optional. The local system timezone is assumed.
 *
 * If the argument string is too short, the "struct tm" contains zero values.
 *
 * RETURN structure has static allocation, modified by next call
 * to TmParseDate().
 */

struct tm * TmParseDate(const char * theString);


/* ================================================================
 * MakeEvDirList
 *
 * Construct a list of event log directories based on message switch
 * strings and date/time structures. The return value is NULL if there
 * are processing or memory allocation errors.
 *
 * The returned structure must be deallocated by a call to
 * FreeKeyList().
 */

KeyListEntry * MakeEvDirList (
	const struct tm * startDate,
	const struct tm * endDate,
	const char * lemsDir,
	const char * evLogDir);


/* ================================================================
 * MakeEvFileList
 *
 * Construct a list of files that comprise the set of message switch
 * log files found in the specified directory.  The return value is NULL
 * if there are processing or memory allocation errors.
 *
 * The returned structure must be deallocated by a call to
 * FreeKeyList().
 */

KeyListEntry * MakeEvFileList(const char * evFileDir);

#endif /* OR_MS_EVNT_DATE_H */
