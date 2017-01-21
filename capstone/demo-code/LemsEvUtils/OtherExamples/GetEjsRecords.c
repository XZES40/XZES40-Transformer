
//
// Template for LEMS-2000 and LEMS/JX Event Log Processing
//
// Find All EAS Input Records - From Line SIS2 - for Date-Time Range
// Send output to named file

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


KeyListEntry	* dirlist = NULL;

KeyListEntry	* fileList = NULL;

//KeyListEntry	* msgList = NULL;

struct tm	startDateTime;

struct tm	endDateTime;

void USAGE(const char * pgm)
{
	printf("\nUSAGE: %s LEMSHOME EVENTDIR STARTDTIME ENDDTIME OUTFILE\n", pgm);
}

main(int argc, char * argv[])
{

	char  startDTime[22];  // = "2009-02-04 13:00:00";
	char  endDTime[22];    // = "2009-02-05 08:59:59";
	char  lemsHome[50];    // = "C:\\lems2k";
	char  eventlogs[50];   // = "BackEvents";
	char  outFileName[260];		// = "EasMessages.dat"
	char  tmpbuf[2048];
	FILE * OutFile;

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
		USAGE(argv[0]);
		exit(1);
	}
	strcpy(lemsHome, argv[1]);
	strcpy(eventlogs, argv[2]);
	strcpy(startDTime, argv[3]);
	strcpy(endDTime, argv[4]);
	strcpy(outFileName, argv[5]);

	if ((OutFile = fopen(outFileName, "w")) == NULL)
	{
		printf("ERROR: Cannot Open OUTFILE: %s\n", outFileName);
		USAGE(argv[0]);
		exit(1);
	}

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
						(theMessage->msgHdr->theIndex.Event_Id == 'I') &&
						(!strcmp(theMessage->msgHdr->theIndex.InLine, "SIS2")) &&
						(!strcmp(theMessage->msgHdr->theIndex.InMke, "EJS")))
					{
						char * p;
						// Found EAS - Enter Arrest Segment to Report
						memset(tmpbuf,0,sizeof(tmpbuf));
						sprintf(tmpbuf,"%04d-%02d-%02d:%02d:%02d:%02d.",
							logtime_tm.tm_year + 1900,
							logtime_tm.tm_mon + 1,
							logtime_tm.tm_mday,
							logtime_tm.tm_hour,
							logtime_tm.tm_min,
							logtime_tm.tm_sec);
						strncat(tmpbuf, theMessage->msgData->msgBuffer,
							(theMessage->msgHdr->theIndex.LogLen > 2000 ? 2000 : theMessage->msgHdr->theIndex.LogLen));
						if (p = strchr(tmpbuf, '\n'))
							*p = '\n';
						fprintf(OutFile, "%s\n", tmpbuf);
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

    fflush(OutFile);
	fclose(OutFile);

	exit(0);
}

#ifdef __cplusplus
}
#endif
