// File: LemsOutStateStats.c
//
// Collect statistics distributed by all inquiries to Oregon resources
// initiated from out-of-state entities.
//
// Messages from NLETS
//   AA  - Amber Alert
//   AM  - Administrative Message
//   AML - Administrative Message
//   AQ  - CCH Manual Inquiry Request
//   BQ  - Boat Registration Inquiry
//   CPQ - Corrections Photo Inquiry
//   DNQ - Driver Operator License Inquiry
//   DQ  - Driver Operator License Inquiry
//   DQG - Driver Operator License Inquiry
//   FQ  - Full CCH Record Inquiry
//   HR  - Road and Weather Broadcasts (NWWS)
//   HS  - Homeland Security Advisory
//   HSL - Homeland Security Advisory
//   IQ  - CCH Index Inquiry
//   KQ  - Driver Operator History Inquiry
//   RQ  - Vehicle Registration Inquiry
//   RQG - Vehicle Registration Inquiry
//   SM  - Status Message
//   SQ  - Snowmobile Registration Inquiry
//   SWQ - State Warrants Inquiry
//   YQ  - Hit Confirmation Request
// Messages from NCIC/III
//   $.A.(CHR)
// Messages from CLETS
//   Group by Message Key for Incoming Inquiries
//
//
// Template for LEMS-2000 and LEMS/JX Event Log Processing
//

#include "platform.h"

#include <time.h>
#include <stdio.h>

#include "keylist.h"
#include "msevlog.h"
#include "lemslog.h"

#include "btcomp.h"

#include "fmtlib.h"

#ifdef __cplusplus
extern 'C' {
#endif

const char * 
NletsMke[] = {
	"AA","AM","AML","AQ",
	"BQ","CPQ","DNQ","DQ","DQG",
	"FQ","HR","HS","HSL","IQ",
	"KQ","RQ","RQG","SM","SQ","SWQ",
	"YQ",
	NULL};

/* SUMMATION KEYLIST
 *
 * KeyListEntry Node
 *    next -> Next Line
 *    k_name = Line Name
 *    k_id   = Counter
 *    k_list -> MessageKeyList
 *      k_name = MessageKey
 *      k_id   = Counter
 */

KeyListEntry * statsList = NULL;

// AddToCount - Accumulate counters to KeyList (statsList)
//
// @Param Line = line name
// @Param Mke = message key name
// @Return = 1 on failure, 0 on success

int AddToCount(const char * Line, const char *Mke)
{
	KeyListEntry * lineNode;
	KeyListEntry * mkeNode;
	KeyListEntry * tmpNode;

	if (statsList == NULL)
	{
		if (statsList = NewKeyListEntry())
		{
			if (SetKeyEntryName(statsList, Line) == NULL) 
				return 1; // memory allocation failure
		}
		else
		{
			return 1;	// memory allocation failure
		}
	}
	// find or create Line Node
	for (lineNode = statsList; lineNode; lineNode = lineNode->next)
	{
		tmpNode = lineNode;
		if (strcmp(lineNode->k_name, Line) == 0) break;
	}
	if (lineNode == NULL)
	{
		if ((tmpNode->next = NewKeyListEntry()) == NULL) 
			return 1;	// memory allocation failure
		lineNode = tmpNode->next;
		if (SetKeyEntryName(lineNode, Line) == NULL)
			return 1;	// memory allocation failure
	}
	// Now for the message key
	if (lineNode->k_list == NULL)
	{
		if (lineNode->k_list = NewKeyListEntry())
		{
			if (SetKeyEntryName(lineNode->k_list, Mke) == NULL) 
				return 1; // memory allocation failure
		}
		else
		{
			return 1;	// memory allocation failure
		}
	}
	// find or create Mke Node
	for (mkeNode = lineNode->k_list; mkeNode; mkeNode = mkeNode->next)
	{
		tmpNode = mkeNode;
		if (strcmp(mkeNode->k_name, Mke) == 0) break;
	}
	if (mkeNode == NULL)
	{
		if ((tmpNode->next = NewKeyListEntry()) == NULL) 
			return 1;	// memory allocation failure
		mkeNode = tmpNode->next;
		if (SetKeyEntryName(mkeNode, Mke) == NULL)
			return 1;	// memory allocation failure
	}
	// Increment the counters
	lineNode->k_id++;
	mkeNode->k_id++;
	return 0;	// success
}

KeyListEntry	* dirlist = NULL;

KeyListEntry	* fileList = NULL;

//KeyListEntry	* msgList = NULL;

struct tm	startDateTime;

struct tm	endDateTime;

char outFile[256];

// Select Messages to Count,
// Return 1 on system failure

int SelectMessage(const LemsLogMessageStruct * theMessage)
{
	const char * theLine;
	const char * theMke;
	int i;
	if (theMessage->msgHdr->theIndex.Event_Id != INPUTMESSAGE)
		return 0;
	theLine = theMessage->msgHdr->theIndex.InLine;
	theMke = theMessage->msgHdr->theIndex.InMke;
	if (strcmp(theLine, "NLETS") == 0)
	{
		for (i=0; NletsMke[i]; i++)
		{
			if (strcmp(theMke, NletsMke[i]) == 0) break;
		}
		if (NletsMke[i])
			return AddToCount(theLine, theMke);
		return 0;
	}
	if (strcmp(theLine, "NCIC") == 0)
	{
		if (strcmp(theMke, "$.A") == 0)
		{
			if (strstr(theMessage->msgData->msgBuffer, "&.A.CHR.") == 0)
				return AddToCount(theLine, theMke);
			return 0;
		}
		return 0;
	}
	if (strcmp(theLine, "CLETS") == 0)
	{
		if (strcmp(theMke, "CLTR") != 0)
			return AddToCount(theLine, theMke);
		return 0;
	}
	return 0;
}

main(int argc, char * argv[])
{

	char  startDTime[22];  // = "2009-02-04 13:00:00";
	char  endDTime[22];    // = "2009-02-05 08:59:59";
	char  lemsHome[50];    // = "C:\\lems2k";
	char  eventlogs[50];   // = "BackEvents";

	FILE * oFile;

	struct tm logtime_tm;
	time_t logtime_sec;

	char * evFileName = NULL;
	char * evDirName = NULL;

	KeyListEntry	* dirList = NULL;
	KeyListEntry	* fileList = NULL;

	KeyListEntry	* DListPtr = NULL;
	KeyListEntry	* FListPtr = NULL;

	if (argc != 6)
	{
		printf("\nUSAGE: %s LEMSHOME EVENTDIR STARTDTIME ENDDTIME OUTFILE\n", argv[0]);
		exit(1);
	}
	strcpy(lemsHome, argv[1]);
	strcpy(eventlogs, argv[2]);
	strcpy(startDTime, argv[3]);
	strcpy(endDTime, argv[4]);
	strcpy(outFile, lemsHome);
	strcat(outFile, "\\xfer\\");
	strcat(outFile, argv[5]);

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

			evFileName = GetKeyEntryName(FListPtr);	// returns pointer to data cached in keylist

			sprintf(fullpath,"%s\\%s",evDirName,evFileName);

			logFileHandle = OpenLemsLogFile(fullpath);

			if (logFileHandle)
			{
				LemsLogMessageStruct * theMessage;
//				printf("\nLEMS LOG FILE: %s\\%s\n", evDirName, evFileName);

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
						SelectMessage(theMessage);
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

	if (!statsList)	exit(0);	// nothing to report

	// Sort our findings
	statsList = KeyListNameSort(statsList);
	{
		KeyListEntry * KPtr;
		for (KPtr = statsList; KPtr; KPtr = KPtr->next)
		{
			KPtr->k_list = KeyListNameSort(KPtr->k_list);
		}
	}
	// Create output report file

	oFile = fopen(outFile, "w");
	if (oFile == NULL) exit(1) ; //fatal - cannot open report file

	// Print the stats
	{
		KeyListEntry * kLine, * kMke;
		
		fprintf(oFile, "Out-Of-State Statistics Report\n");
		fprintf(oFile, "FROM: %s\n", startDTime);
		fprintf(oFile, "TO: %s\n", endDTime);
		fprintf(oFile, "\nMessage Key records are tab delimited for spreadsheet import.\n");

		for (kLine = statsList; kLine; kLine = kLine->next)
		{
			fprintf(oFile, "\nLINE: %s    Total = %d\n", kLine->k_name, kLine->k_id);
			for (kMke = kLine->k_list; kMke; kMke = kMke->next)
			{
				fprintf(oFile, "\t%s\t%d\n", kMke->k_name, kMke->k_id);
			}
		}
	}
	fflush(oFile);
	fclose(oFile);

	// Cleanup

	FreeKeyList(&statsList);
	exit(0);
}

#ifdef __cplusplus
}
#endif
