
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

const char * NCJoris[] = {
"OR001CPS0",
"OR003CPS0",
"OR003SPD0",
"OR003SPDM",
"OR003SSD0",
"OR005CPS0",
"OR006019Q",
"OR006CPS0",
"OR006IG00",
"OR006SSD0",
"OR009CPS0",
"OR010CPS0",
"OR010IG00",
"OR015CPS0",
"OR016IG00",
"OR018IG00",
"OR020CPS0",
"OR020SSD0",
"OR021IG00",
"OR022CPS0",
"OR022SSD0",
"OR023CPS0",
"OR0240CJC",
"OR0240DVA",
"OR0240HCS",
"OR0240MD0",
"OR024ACLB",
"OR024ADU0",
"OR024BAE0",
"OR024BCE0",
"OR024BCSW",
"OR024BMT0",
"OR024CCB0",
"OR024CCD0",
"OR024CMH0",
"OR024CPS0",
"OR024CRU0",
"OR024D0G0",
"OR024D0R0",
"OR024DAS0",
"OR024DCB0",
"OR024DE00",
"OR024DIF0",
"OR024EDHR",
"OR024EHB0",
"OR024HA01",
"OR024MCE0",
"OR024PR0Z",
"OR024REA0",
"OR024SMH0",
"OR024SSA0",
"OR024SSDE",
"OR024SWC0",
"OR024TSP0",
"OR0260BNE",
"OR0260CFB",
"OR0260MMP",
"OR026B0P0",
"OR026BOP0",
"OR026BRT0",
"OR026CPS0",
"OR026EMS0",
"OR026HSID",
"OR026MCB0",
"OR026PTLB",
"OR026SCF0",
"OR026SSD0",
"OR027IG00",
"OR030CPS0",
"OR030IG00",
"OR033CPS0",
"OR034CPS0",
"OR034SBN0",
"OR037MHD0",
"OR0BPSTMS",
"OR0D0JCVC",
"OR0DCBS00",
"OR0DFW000",
"OR0LCCLID",
"OR0LN0000",
"OR0PGMR00",
"ORPCC0000",
"ORWEATHER",
NULL
};

/* SUMMATION COUNTERS */

int	RegulatoryCCH = 0;
int VehiclesStolen = 0;
int VehiclesRecovered = 0;
int WarrantsEntered = 0;
int WarrantsCleared = 0;
int ArrestsEntered = 0;


KeyListEntry	* dirlist = NULL;

KeyListEntry	* fileList = NULL;

//KeyListEntry	* msgList = NULL;

struct tm	startDateTime;

struct tm	endDateTime;

int TestNCori(const char * theOri)
{
	int i;
	if (! theOri || (*theOri == '\0'))
		return 0;
	for (i=0; NCJoris[i]; i++)
	{
		if (strcmp(NCJoris[i], theOri) == 0)
			return 1;
	}
	return 0;
}

main(int argc, char * argv[])
{

	char  startDTime[22];  // = "2009-02-04 13:00:00";
	char  endDTime[22];    // = "2009-02-05 08:59:59";
	char  lemsHome[50];    // = "C:\\lems2k";
	char  eventlogs[50];   // = "BackEvents";

	struct tm logtime_tm;
	time_t logtime_sec;

	char * evFileName = NULL;
	char * evDirName = NULL;

	KeyListEntry	* dirList = NULL;
	KeyListEntry	* fileList = NULL;

	KeyListEntry	* DListPtr = NULL;
	KeyListEntry	* FListPtr = NULL;

	if (argc != 5)
	{
		printf("\nUSAGE: %s LEMSHOME EVENTDIR STARTDTIME ENDDTIME\n", argv[0]);
		exit(1);
	}
	strcpy(lemsHome, argv[1]);
	strcpy(eventlogs, argv[2]);
	strcpy(startDTime, argv[3]);
	strcpy(endDTime, argv[4]);

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
						(tmcomp(&logtime_tm, &endDateTime) <= 0) &&
						(theMessage->msgHdr->theIndex.Event_Id == 'I'))
					{	

						/* Accumulate Counts */
						if (strcmp(theMessage->msgHdr->theIndex.InMke, "QWHD") == 0 && 
							TestNCori(theMessage->msgHdr->theIndex.ORI_Id))
						{
							RegulatoryCCH++;
						}
						else if (strcmp(theMessage->msgHdr->theIndex.InMke, "EAS") == 0 &&
							strcmp(theMessage->msgHdr->theIndex.ORI_Id, "OR0SBI000") == 0)
						{
							ArrestsEntered++;
						}
						else if (strcmp(theMessage->msgHdr->theIndex.InMke, "XAS") == 0 &&
							strcmp(theMessage->msgHdr->theIndex.ORI_Id, "OR0SBI000") == 0)
						{
							ArrestsEntered--;
						}
						else if (FindExprList(theMessage->msgHdr->theIndex.InMke, 9,
							"EW-C", "EW", "EWJC", "EWJ", "SW-C", "SW", "RW-C", "RW", "ET"))
						{
							WarrantsEntered++;
						}
						else if (FindExprList(theMessage->msgHdr->theIndex.InMke, 2,
							"XW", "XT"))
						{
							WarrantsEntered--;
						}
						else if (FindExprList(theMessage->msgHdr->theIndex.InMke, 2,
							"CW", "CT"))
						{
							WarrantsCleared++;
						}
						else if (FindExprList(theMessage->msgHdr->theIndex.InMke, 4,
							"EV-A", "EV-F", "EV-P", "EV") &&
							strcmp(theMessage->msgHdr->theIndex.InLine, "LEDS") != 0)
						{
							VehiclesStolen++;
						}
						else if (strcmp(theMessage->msgHdr->theIndex.InMke, "XV") == 0 &&
							strcmp(theMessage->msgHdr->theIndex.InLine, "LEDS") != 0)
						{
							VehiclesStolen--;
						}
						else if (strcmp(theMessage->msgHdr->theIndex.InMke, "CV") == 0 &&
							strcmp(theMessage->msgHdr->theIndex.InLine, "LEDS") != 0)
						{
							VehiclesRecovered++;
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

//	int	RegulatoryCCH = 0;
//	int VehiclesStolen = 0;
//	int VehiclesRecovered = 0;
//	int WarrantsEntered = 0;
//	int WarrantsCleared = 0;
//	int ArrestsEntered = 0;					}

//	Report our Findings to STDOUT

	printf("\nMESSAGE SWITCH COUNTS FOR DATE-TIME RANGE\n\n");
	printf("Regulatory Agency Background Checks = %d\n", RegulatoryCCH);
	printf("Stolen Vehicles Entered in Oregon   = %d\n", VehiclesStolen);
	printf("Recoverd Oregon Vehicles            = %d\n", VehiclesRecovered);
	printf("Warrants Entered in Oregon          = %d\n", WarrantsEntered);
	printf("Warrants Cleared in Oregon          = %d\n", WarrantsCleared);
	printf("Arrests Entered in Oregon by OSP/ID = %d\n", ArrestsEntered);
	printf("\n\n");

	exit(0);
}

#ifdef __cplusplus
}
#endif
