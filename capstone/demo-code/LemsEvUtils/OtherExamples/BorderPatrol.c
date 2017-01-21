
//
// Template for LEMS-2000 and LEMS/JX Event Log Processing
//
// Modified for ORI Statistics
//
//		printf("ERROR - USAGE:\r\n"
//			"Program LogDir OutDir Start-DayTime End-DayTime Ori MkeFile.csv DayFile.csv\r\n");
//		exit(1);
//
// This program creates two output files
//   OriMonthMke.csv
//   OriDailyCount.csv
//

#include "platform.h"

#include <time.h>
#include <stdio.h>

#include "keylist.h"
#include "msevlog.h"
#include "lemslog.h"

#include "btcomp.h"

#ifdef __cplusplus
extern 'C' {
#endif

// Items for Event Log Traversal

KeyListEntry	* dirlist = NULL;

KeyListEntry	* fileList = NULL;

struct tm	startDateTime;

struct tm	endDateTime;

// Items for Statistics Reporting

const char * MonthName[] = {"", "January", "February", "March", "April", "May", "June",
		"July", "August", "September", "October", "November", "December"};

KeyListEntry	* mkeCounts = NULL;
KeyListEntry	* dailyCounts = NULL;

int countMke(const char * mke, const struct tm * DayTm)
{
	int rtn = 0;
	KeyListEntry	* MonthPtr = NULL;
	KeyListEntry	* MkePtr = NULL;
	KeyListEntry	* TPtr = NULL;
	int yrmon;

	yrmon = (DayTm->tm_year + 1900) * 100 + (DayTm->tm_mon + 1);

	if (mkeCounts == NULL)
		mkeCounts = NewKeyListEntry();
	if (!mkeCounts)
		return 1;	// memory allocation failure

//***
	for (TPtr = mkeCounts; TPtr->k_id && TPtr->next; TPtr = TPtr->next)
	{
		if (TPtr->k_id == yrmon) break;
	}

	if (!TPtr->k_id)
		TPtr->k_id = yrmon;

	if (TPtr->k_id != yrmon)
	{
		TPtr->next = NewKeyListEntry();
		if (!TPtr->next)
			return 1;	// memory allocation failure
		TPtr = TPtr->next;
		TPtr->k_id = yrmon;
	}

	MonthPtr = TPtr;

	if (!MonthPtr->k_list)
	{
		MonthPtr->k_list = NewKeyListEntry();
		if (!MonthPtr->k_list)
			return 1;	// memory allocation failure
	}

	for (TPtr = MonthPtr->k_list; TPtr->k_name && TPtr->next;  TPtr = TPtr->next)
	{
		if (!strcmp(TPtr->k_name, mke))
			break;
	}

	if (!TPtr->k_name)
	{
		if (!SetKeyEntryName(TPtr, mke))
			return 1;	// memory allocation failure
	}

	if (strcmp(TPtr->k_name, mke))
	{
		TPtr->next = NewKeyListEntry();
		if (!TPtr)
			return 1;	// memory allocation failure
		TPtr = TPtr->next;
		if (!SetKeyEntryName(TPtr, mke))
			return 1;	// memory allocation failure
	}

	MkePtr = TPtr;

	MonthPtr->k_size++;

	MkePtr->k_size++;

	return rtn;
}


int countDay(const struct tm * DayTm)
{
	int rtn = 0;
	KeyListEntry	* MonthPtr = NULL;
	KeyListEntry	* DayPtr = NULL;
	KeyListEntry	* TPtr = NULL;
	int yrmon;

	yrmon = (DayTm->tm_year + 1900) * 100 + (DayTm->tm_mon + 1);

	if (dailyCounts == NULL)
		dailyCounts = NewKeyListEntry();
	if (!dailyCounts)
		return 1;	// memory allocation failure

	for (TPtr = dailyCounts; TPtr->k_id && TPtr->next; TPtr = TPtr->next)
	{
		if (TPtr->k_id == yrmon) break;
	}

	if (!TPtr->k_id)
		TPtr->k_id = yrmon;

	if (TPtr->k_id != yrmon)
	{
		TPtr->next = NewKeyListEntry();
		if (!TPtr->next)
			return 1;	// memory allocation failure
		TPtr = TPtr->next;
		TPtr->k_id = yrmon;
	}

	MonthPtr = TPtr;

	if (!MonthPtr->k_list)
	{
		MonthPtr->k_list = NewKeyListEntry();
		if (!MonthPtr->k_list)
			return 1;	// memory allocation failure
	}

	for (TPtr = MonthPtr->k_list; TPtr->k_id && TPtr->next; TPtr = TPtr->next)
	{
		if (TPtr->k_id == DayTm->tm_mday)
			break;
	}

	if (TPtr->k_id == 0)
		TPtr->k_id = DayTm->tm_mday;

	if (TPtr->k_id != DayTm->tm_mday)
	{
		TPtr->next = NewKeyListEntry();
		if (!TPtr->next)
			return 1;	// memory allocation failure
		TPtr = TPtr->next;
		TPtr->k_id = DayTm->tm_mday;
	}

	DayPtr = TPtr;

	MonthPtr->k_size++;

	DayPtr->k_size++;

	return rtn;
}


void freeStatsLists()
{
	FreeKeyList(&mkeCounts);
	FreeKeyList(&dailyCounts);
}

int printStatsLists(const char * ori, const char * mkefile, const char * dailyfile)
{
	int rtn = 0;
	KeyListEntry	* MPtr;
	KeyListEntry	* TPtr;

	FILE * MKE_FH;

	FILE * DAY_FH;

	// Sort the lists by ascending year-month values

	mkeCounts = KeyListIDValueSort(mkeCounts);

	dailyCounts = KeyListIDValueSort(dailyCounts);

	// Sort mke sublists by name - the mke's

	for (MPtr = mkeCounts; MPtr; MPtr = MPtr->next)
		MPtr->k_list = KeyListNameSort(MPtr->k_list);

	// Sort daily sublists by ID Value - the days

	for (MPtr = dailyCounts; MPtr; MPtr = MPtr->next)
		MPtr->k_list = KeyListIDValueSort(MPtr->k_list);

// Open mkefile

	MKE_FH = fopen(mkefile, "w");

	// error test and report
	// continue

	// write the report to mkefile.csv file
	//
	// OREGON MESSAGE SWITCH - ACTIVITY REPORT
	//
	// MONTHLY SUMMARY OF MESSAGES INITIATED BY ORI = OR0245030
	//
	//  January YYYY,	Total =, tally
	//			, mke1 =, tally1
	//			, mke2 =, tally2
	//			, mke3 =, tally3
	//			, mke4 =, tally4
	//  February YYYY,  Total = , tally
	//			, mke1 =, tally1
	//			, mke2 =, tally2
	//			, mke3 =, tally3
	//			, mke4 =, tally4
	//  March YYYY,  Total = , tally
	//			, mke1 =, tally1
	//			, mke2 =, tally2
	//			, mke3 =, tally3
	//			, mke4 =, tally4
	//  April YYYY,  Total = , tally
	//			, mke1 =, tally1
	//			, mke2 =, tally2
	//			, mke3 =, tally3
	//			, mke4 =, tally4

	fprintf(MKE_FH, "OREGON MESSAGE SWITCH - ACTIVITY REPORT\n\n"
		            "MONTHLY SUMMARY OF MESSAGES INITIATED BY ORI = %s\n\n",ori);

	for (MPtr = mkeCounts; MPtr; MPtr = MPtr->next)
	{
		fprintf(MKE_FH, "%s %d,Total =,%d\n",
			MonthName[MPtr->k_id % 100], MPtr->k_id / 100, MPtr->k_size);

		for (TPtr = MPtr->k_list; TPtr; TPtr = TPtr->next)
		{
			fprintf(MKE_FH, ",%s =,%d\n",
				TPtr->k_name, TPtr->k_size);
		}
	}

// Close mkefile

	fflush(MKE_FH);
	fclose(MKE_FH);

// Open dailyfile

	DAY_FH = fopen(dailyfile, "w");

	// error test and report
	// continue

	// write the report to dailyfile.csv file
	//
	// OREGON MESSAGE SWITCH - ACTIVITY REPORT
	//
	// DAILY SUMMARY OF MESSAGES INITIATED BY ORI = OR0245030
	//
	//  January YYYY,	Total =, tally
	//			, 1=, tally, 2=, tally, 3=, tally, ...
	//  February YYYY,  Total = , tally
	//			, 1=, tally, 2=, tally, 3=, tally, ...
	//  March YYYY,  Total = , tally
	//			, 1=, tally, 2=, tally, 3=, tally, ...
	//  April YYYY,  Total = , tally
	//			, 1=, tally, 2=, tally, 3=, tally, ...

	fprintf(DAY_FH, "OREGON MESSAGE SWITCH - ACTIVITY REPORT\n\n"
		            "DAILY SUMMARY OF MESSAGES INITIATED BY ORI = %s\n\n",ori);

	for (MPtr = dailyCounts; MPtr; MPtr = MPtr->next)
	{
		fprintf(DAY_FH, "%s %d,Total =,%d\n",
			MonthName[MPtr->k_id % 100], MPtr->k_id / 100, MPtr->k_size);

		for (TPtr = MPtr->k_list; TPtr; TPtr = TPtr->next)
		{
			fprintf(DAY_FH, ",%d =,%d", TPtr->k_id, TPtr->k_size);
		}
		fprintf(DAY_FH, "\n");
	}

// close dailyfile

	fflush(DAY_FH);
	fclose(DAY_FH);

	return rtn;
}



main(int argc, char * argv[])
{

	char * startDTime = "2009-02-04 13:00:00";
	char * endDTime = "2009-02-05 08:59:59";
	char * lemsHome = "";
	char * outdir;
	char * theOri;
	char theMkeFile[MAXPATHLEN];
	char theDayFile[MAXPATHLEN];
	char * eventlogs = "C:\\lems2k\\BackEvents";

	struct tm logtime_tm;
	time_t logtime_sec;

	char * evFileName = NULL;
	char * evDirName = NULL;

	KeyListEntry	* dirList = NULL;
	KeyListEntry	* fileList = NULL;

	KeyListEntry	* DListPtr = NULL;
	KeyListEntry	* FListPtr = NULL;


// Process program arguments

	if (argc != 8)
	{
		printf("ERROR - USAGE:\r\n"
			"Program LogDir OutDir Start-DayTime End-DayTime Ori MkeFile.csv DayFile.csv\r\n");
		exit(1);
	}

	eventlogs = strdup(argv[1]);
	outdir = strdup(argv[2]);
	startDTime = strdup(argv[3]);
	endDTime = strdup(argv[4]);
	theOri = strdup(argv[5]);

	sprintf(theMkeFile,"%s%s%s",outdir,DIRSLASH,argv[6]);
	sprintf(theDayFile,"%s%s%s",outdir,DIRSLASH,argv[7]);

	memcpy((void *) &startDateTime, (void *) TmParseDate(startDTime), sizeof(struct tm));

	memcpy((void *) &endDateTime, (void *) TmParseDate(endDTime), sizeof(struct tm));

	dirList = MakeEvDirList(&startDateTime, &endDateTime, lemsHome, eventlogs);

	for (DListPtr = dirList; DListPtr; DListPtr=GetKeyEntryNext(DListPtr))
	{
		evDirName = GetKeyEntryName(DListPtr);	// returns pointer to data cached in keylist
		fileList = MakeEvFileList(evDirName);

		for (FListPtr = fileList; FListPtr; FListPtr=GetKeyEntryNext(FListPtr))
		{
			LEMSLOGHANDLE	logFileHandle;
			char fullpath[MAXPATHLEN];
			int msgnbr = 0;

			evFileName = GetKeyEntryName(FListPtr);	// returns pointer to data cached in keylist

			sprintf(fullpath,"%s\\%s",evDirName,evFileName);

			logFileHandle = OpenLemsLogFile(fullpath);

			if (logFileHandle)
			{
				LemsLogMessageStruct * theMessage;
				printf("\nLEMS LOG FILE: %s\\%s\n", evDirName, evFileName);

				for (theMessage = GetLemsLogMessage(logFileHandle); theMessage;
					theMessage = GetNextLemsLogMessage (logFileHandle))
				{

					// NOTE:  (theMessage) points to structured copy of message content
					// and should be freed when done, FreeLemsLogMessage(theMessage);

					// MESSAGE OFFSET INDEX

//					(off_t)			= GetLemsLogMessageIx(logFileHandle);

					// MESSAGE HEADER INFORMATION

//					(int)			= theMessage->msgHdr->theIndex.IndexType
//					(time_t)		= theMessage->msgHdr->theIndex.LogDate;
//					(LemsEvType)		= theMessage->msgHdr->theIndex.Event_Id;
//					(char[11])		= theMessage->msgHdr->theIndex.MSN;
//					(char[9])		= theMessage->msgHdr->theIndex.ISN;
//					(char[9])		= theMessage->msgHdr->theIndex.OSN;
//					(char[9])		= theMessage->msgHdr->theIndex.Src_Dev;
//					(char[9])		= theMessage->msgHdr->theIndex.Dest_Dev;
//					(char[10])		= theMessage->msgHdr->theIndex.ORI_Id;
//					(char[LEMSMAXUIDLEN])	= theMessage->msgHdr->theIndex.User_Id;
//					(char[9])		= theMessage->msgHdr->theIndex.InLine;
//					(char[9])		= theMessage->msgHdr->theIndex.OutLine;
//					(char[9])		= theMessage->msgHdr->theIndex.InMke;
//					(char[9])		= theMessage->msgHdr->theIndex.OutMke;
//					(size_t)		= theMessage->msgHdr->theIndex.MsgLen;
//					(size_t)		= theMessage->msgHdr->theIndex.LogLen;
//					(struct timeval)	= theMessage->msgHdr->theIndex.IN_DTIME;
//					(struct timeval)	= theMessage->msgHdr->theIndex.OUT_DTIME;
//					(struct timeval)	= theMessage->msgHdr->theIndex.LogTime;
//					(char[4])		= theMessage->msgHdr->theIndex.MsgText;

					logtime_sec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
					memcpy(&logtime_tm, localtime(&logtime_sec), sizeof(struct tm));

					if ((tmcomp(&logtime_tm, &startDateTime) >= 0) &&
						(tmcomp(&logtime_tm, &endDateTime) <= 0))
					{
						if (theMessage->msgHdr->theIndex.Event_Id == INPUTMESSAGE &&
							theMessage->msgHdr->theIndex.ORI_Id &&
							(strcmp(theMessage->msgHdr->theIndex.InLine, "LEDS")) &&
							(strcmp(theMessage->msgHdr->theIndex.InLine, "NLETS")) &&
							(strcmp(theMessage->msgHdr->theIndex.InLine, "MUGS")) &&
							(!strcmp(theMessage->msgHdr->theIndex.ORI_Id, theOri)))
						{
							countMke(theMessage->msgHdr->theIndex.InMke, &logtime_tm);
							countDay(&logtime_tm);
						}
					}

					// GET RID OF THE MESSAGE COPY

					FreeLemsLogMessage(theMessage);
				}
				CloseLemsLogFile (logFileHandle);
			}
		}
		FreeKeyList(&fileList);
	}
	FreeKeyList(&dirList);

	printStatsLists(theOri, theMkeFile, theDayFile);
	freeStatsLists();

	exit(0);
}

#ifdef __cplusplus
}
#endif
