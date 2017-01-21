
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

#ifdef __cplusplus
extern 'C' {
#endif


KeyListEntry	* dirlist = NULL;

KeyListEntry	* fileList = NULL;

//KeyListEntry	* msgList = NULL;

struct tm	startDateTime;

struct tm	endDateTime;


main(int argc, char * argv[])
{

	char * startDTime = "2009-02-04 13:00:00";
	char * endDTime = "2009-02-05 08:59:59";
	char * lemsHome = "C:\\lems2k";
	char * eventlogs = "BackEvents";

	struct tm logtime_tm;
	time_t logtime_sec;

	char * evFileName = NULL;
	char * evDirName = NULL;

	KeyListEntry	* dirList = NULL;
	KeyListEntry	* fileList = NULL;

	KeyListEntry	* DListPtr = NULL;
	KeyListEntry	* FListPtr = NULL;

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

					msgnbr ++;

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

//			if (theMessage->msgHdr->theIndex.LogLen < theMessage->msgHdr->theIndex.MsgLen)

			logtime_sec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
			memcpy(&logtime_tm, localtime(&logtime_sec), sizeof(struct tm));

			if ((tmcomp(&logtime_tm, &startDateTime) >= 0) &&
				(tmcomp(&logtime_tm, &endDateTime) <= 0) &&
				(theMessage->msgHdr->theIndex.LogLen < theMessage->msgHdr->theIndex.MsgLen))
			{	

					printf("  MESSAGE: %8.8d %8.8s %8.8s %8.8s %8.8s %8.8s %8.8s\n",
						msgnbr,
						theMessage->msgHdr->theIndex.InLine,
						theMessage->msgHdr->theIndex.Src_Dev,
						theMessage->msgHdr->theIndex.InMke,
						theMessage->msgHdr->theIndex.OutLine,
						theMessage->msgHdr->theIndex.Dest_Dev,
						theMessage->msgHdr->theIndex.OutMke);

					printf("     %4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d\n",
						logtime_tm.tm_year + 1900,
						logtime_tm.tm_mon + 1,
						logtime_tm.tm_mday,
						logtime_tm.tm_hour,
						logtime_tm.tm_min,
						logtime_tm.tm_sec);

					printf("     Size Values:  MsgLen = %d,   LogLen = %d\n",
						theMessage->msgHdr->theIndex.MsgLen,
						theMessage->msgHdr->theIndex.LogLen);

					printf("     Size Values:  MsgLen = %d,   LogLen = %d\n",
						theMessage->msgData->msgLength,
						theMessage->msgData->msgLogLen);

					theMessage->msgData->msgBuffer[theMessage->msgData->msgLogLen] = '\0';
					printf("     Data: %s\n", theMessage->msgData->msgBuffer);

			}
					// MESSAGE BODY CONTENT

//					(size_t)		= theMessage->msgData->msgLogLen;
//					(size_t)		= theMessage->msgData->msgLength;
//					(unsigned char *)	= theMessage->msgData->msgBuffer;

					// GET RID OF THE MESSAGE COPY

					FreeLemsLogMessage(theMessage);
				}


				CloseLemsLogFile (logFileHandle);
			}
		}

		FreeKeyList(&fileList);
	}

	FreeKeyList(&dirList);

	exit(0);
}

#ifdef __cplusplus
}
#endif
