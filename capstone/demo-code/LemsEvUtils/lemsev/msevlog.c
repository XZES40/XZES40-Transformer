/*
 * 'C' Source Library - Common Utilities - Supported by GNU and Windows VC7.1
 *
 * Copyright (c) 2008 - This is open source software in the pulbic domain.
 *
 * Developed by: Steven J. Hathaway
 *
 * For:  State of Oregon - Oregon State Police / CJIS-LEDS
 *
 * Rev  0.01          Oct 13 2009           Hathaway
 *   Function: MakeEvDirList, allow lemsDir to be empty or NULL.
 */

#include "platform.h"
#include "keylist.h"
#include "btcomp.h"
#include "dirlist.h"

#include <time.h>
#include <stdio.h>
#include <string.h>

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

char * MakeDateDirName(const struct tm * dDate)
{
	static char *dayOfWeek[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", NULL};
	char cymd[11];
	static char evDir[MAXPATHLEN];

	evDir[0] = '\0';
	
	if (!dDate)
		return evDir;

	strcpy(evDir, DIRSLASH);
	strcat(evDir, dayOfWeek[dDate->tm_wday]);
	strcat(evDir, DIRSLASH);
	sprintf(cymd,"%4.4d-%2.2d-%2.2d",
		(dDate->tm_year + 1900),
		(dDate->tm_mon + 1),
		dDate->tm_mday);
	strcat(evDir, cymd);
	return evDir;
}

/* ================================================================
 * TmParseDate
 * 
 * NOTE: Result String has Static Allocation
 *
 * Convert a string of "CCYYY-MM-DD^HH:MM:SS" format into a struct tm.
 *
 * Internal field punctuation is ignored, but the numeric digits are significant.
 * The argeument string is to be at least 19 characters in length.
 * The local system timezone is assumed.
 *
 * If the argument string is too short, the "struct tm" contains zero values.
 */

struct tm * TmParseDate(const char * theString)
{
	static struct tm timeInfo;
	char year[5];
	char month[3];
	char day[3];
	char hrs[3] = "00";
	char min[3] = "00";
	char sec[3] = "00";
	size_t length;

	memset((void *) &timeInfo, 0, sizeof(timeInfo));

	if (!theString || ((length = strlen(theString)) < 10))
		return &timeInfo;

	memcpy(year, &theString[0], 4);
	memcpy(month, &theString[5], 2);
	memcpy(day, &theString[8], 2);
	if (length >= 13)
		memcpy(hrs, &theString[11], 2);
	if (length >= 16)
		memcpy(min, &theString[14], 2);
	if (length >= 19)
		memcpy(sec, &theString[17], 2);
	year[4] = '\0';
	month[2] = '\0';
	day[2] = '\0';
	hrs[2] = '\0';
	min[2] = '\0';
	sec[2] = '\0';
	timeInfo.tm_sec = atoi(sec);
	timeInfo.tm_min = atoi(min);
	timeInfo.tm_hour = atoi(hrs);
	timeInfo.tm_mday = atoi(day);
	timeInfo.tm_mon = (atoi(month) -1);
	timeInfo.tm_year = (atoi(year) - 1900);
	timeInfo.tm_wday = 0;
	timeInfo.tm_yday = 0;
	timeInfo.tm_isdst = -1;
	mktime(&timeInfo);
	return &timeInfo;
}

/* ================================================================
 * MakeEvDirList
 *
 * Construct a list of event log directories based on message switch
 * strings and date/time structures.
 *
 * Update: 10/13/2009 - Allow lemsDir to be empty or null.
 */

KeyListEntry * MakeEvDirList (
	const struct tm * startDate,
	const struct tm * endDate,
	const char * lemsDir,
	const char * evLogDir)
{

	KeyListEntry * evNode = NULL;
	KeyListEntry * theList = NULL;
	struct tm tmDate;
	size_t baseDirLen;
	char baseDir[MAXPATHLEN];


	if (!startDate || !endDate || !lemsDir || !evLogDir)
	{
		/* Parameters cannot be NULL */
		return NULL;
	}

	if ((strlen(lemsDir) + strlen(evLogDir) + 45) >= MAXPATHLEN)
	{
		/* Possible Buffer Overrun */
		return NULL;
	}

	if (startDate->tm_year == 0 &&
		startDate->tm_mon == 0 &&
		startDate->tm_mday == 0)
	{
		/* Start Date Parse Failure */
		return NULL;
	}

	if (tmcomp(startDate, endDate) > 0)
	{
		/* Start Date-Time After End Date-Time */
		return NULL;
	}


	if (lemsDir == NULL || !lemsDir[0])
		strcpy(baseDir,evLogDir);
	else
		sprintf(baseDir, "%s%s%s",
			lemsDir, DIRSLASH, evLogDir);

	baseDirLen = strlen(baseDir) + 1;

	memcpy((void *) &tmDate, (void *) startDate, sizeof(struct tm));


	do
	{
		char evName[MAXPATHLEN + 100];

		sprintf(evName, "%s%s", baseDir,
			MakeDateDirName(&tmDate));

		if (!theList)
			theList = evNode = NewKeyListEntry();
		else
		{
			evNode->next = NewKeyListEntry();
			evNode=evNode->next;
		}

		SetKeyEntryName(evNode, evName);

		/* Advance to next day */
		tmDate.tm_mday++;
		tmDate.tm_hour = 0;
		tmDate.tm_min = 0;
		tmDate.tm_sec = 0;
		tmDate.tm_isdst = -1;
		mktime(&tmDate);

	} while (tmcomp(&tmDate, endDate) <= 0);

	return theList;
}


/* ================================================================
 * MakeEvFileList
 *
 * Construct a list of files that comprise the set of message switch
 * log files found in the specified directory.
 */

KeyListEntry * MakeEvFileList (const char * evFileDir)
{
	KeyListEntry * theNode = NULL;
	KeyListEntry * theList = NULL;
	size_t		fullDirLen;
	char		pathName[MAXPATHLEN + 15];
	DIRHANDLE	dHandle;

	/* we must have a directory name */

	if (!evFileDir)
		return NULL;

	/* check for buffer overrun, eventlog filenames are less than 15 chars */

	fullDirLen = strlen(evFileDir) + 15;

	if (fullDirLen > MAXPATHLEN)
		return NULL;

	sprintf(pathName, "%s%sEV*.log", evFileDir, DIRSLASH);

	if ((dHandle = FindFirstDirEntry(pathName)) == INVALID_HANDLE_VALUE)
		return NULL;

	do
	{
		if (DirentIsFile(dHandle))
		{
			if (!theNode)
				theList = theNode = NewKeyListEntry();
			else
				theNode = theNode->next = NewKeyListEntry();

			SetKeyEntryName(theNode, DirentGetFileName(dHandle));
		}
	} while (FindNextDirEntry(dHandle));

	CloseFindDir(dHandle);	

	return theList;
}

