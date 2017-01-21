
//
// Template for LEMS-2000 and LEMS/JX Event Log Processing
//

/**
 * Extract LEMS Device Stats for all devices on a LINE or a set of LINES
 *
 * USAGE pgm lemsdir evlogdir start-dtime end-dtime OutFile LineList ...
 *
 * start-dtime and end-dtime are of format "CCYY-MM-DD HH:MM:SS"
 *
 * STATS_TREE
 *   \_ LineList :: LINE_NAME (Line_io_count) ...
 *                   \_ Device_List :: DEVICE_NAME (Device_io_count) ...
 *
 *
 */

#include "platform.h"

#include "btcomp.h"

#include "keylist.h"

#include "dirlist.h"

#include "msevlog.h"

#include "lemslog.h"

//#if defined(_WIN32)
//  #include <direct.h>	// needed to define mkdir()
//#endif

#include <stdio.h>
#include <time.h>


// The File Global Data

struct tm	startDateTime;
struct tm	endDateTime;

int mallocErr = 0;

FILE * outHandle;

KeyListEntry * STATS_TREE = NULL;

// Returns TRUE=1 if directory exists
// Returns FALSE=0 if directory does not exist

int testDirExists(const char * dirname)
{
	DIRHANDLE	dirH = NULL;

	dirH = FindFirstDirEntry(dirname);

	if (dirH == NULL || dirH == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if (!DirentIsDirectory(dirH))
	{
		CloseFindDir(dirH);
		return FALSE;
	}

	CloseFindDir(dirH);
	return TRUE;
}

// Returns TRUE=1 if data file exists
// Returns FALSE=0 if data file does not exist

int testFileExists(const char * filepath)
{
	DIRHANDLE	dirH = NULL;

	dirH = FindFirstDirEntry(filepath);

		if (dirH == NULL || dirH == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	if (!DirentIsFile(dirH))
	{
		CloseFindDir(dirH);
		return FALSE;
	}

	CloseFindDir(dirH);
	return TRUE;
}

// THE USAGE INFORMATION - AND EXIT WITH ERROR

void usage(int argc, char * argv[])
{
	fprintf(stderr,"\nUSAGE: %s lemsdir evlogdir start-dtime end-dtime outfile lines ...\n\n"
	"lemsdir is the base of LEMSJX or LEMS2000 i.e. F:\\lems2k\n"
	"evlogdir is the eventlog or backevents subdirectory under lemsdir\n"
	"start-dtime is the start date-time in \"ccyy-mm-dd hh:mm:ss\" format\n"
	"end-dtime is the ending date-time in \"ccyy-mm-dd hh:mm:ss\" format\n"
	"outfile is the name of file in {lemsdir}\\xfer\\\n"
	"lines ... is the list of line names for device statistics\n\n", argv[0]);

	exit (1);
}


// Return 0 on success, non-zero on failure
int initializeStatsTree(void)
{
	if (!STATS_TREE)
	{
		STATS_TREE = NewKeyListEntry();
		if (STATS_TREE)
		{
			if (!SetKeyEntryName(STATS_TREE, "STATS_TREE"))
			{
				mallocErr = 1;
				return 1;
			}
		}
		else
		{
			mallocErr  = 1;
			return 1;
		}
	}
	return 0;
}

void freeStatsTree(void)
{
	FreeKeyList(&STATS_TREE);
}

// return 0 on success otherwise failure
int incrementLineDeviceCounter(const char * lineid, const char * deviceid)
{
	int				found = 0;
	KeyListEntry	*KPtr = NULL;

	KPtr = STATS_TREE;
	KPtr->k_id++;

	for (;KPtr->next != NULL; KPtr = KPtr->next)
	{
		if (!strcmp(KPtr->k_name, lineid))
		{
			found = 1;
			break;
		}
	}
	if (strcmp(KPtr->k_name, lineid))
	{
		KPtr->next = NewKeyListEntry();
		KPtr = KPtr->next;
		if (!KPtr)
			return 1;	// allocation error
		if (!SetKeyEntryName(KPtr,lineid))
			return 1;	// allocation error
	}
	KPtr->k_id++;

	if (KPtr->k_list == NULL)
	{
		KPtr->k_list = NewKeyListEntry();
		if (KPtr->k_list == NULL)
			return 1;	// allocation error
		if (!SetKeyEntryName(KPtr->k_list,deviceid))
			return 1;	// allocation error
	}
	
	for (KPtr = KPtr->k_list;KPtr->next != NULL; KPtr = KPtr->next)
	{
		if (!strcmp(KPtr->k_name, deviceid))
		{
			found = 1;
			break;
		}
	}
	if (strcmp(KPtr->k_name, deviceid))
	{
		KPtr->next = NewKeyListEntry();
		KPtr = KPtr->next;
		if (!KPtr)
			return 1;	// allocation error
		if (!SetKeyEntryName(KPtr,deviceid))
			return 1;	// allocation error
	}
	KPtr->k_id++;
	return 0;
}

// return 0 on success, otherwise error
int openStatsFile(const char * outfile)
{
	if (!(outHandle = fopen(outfile, "w+")))
		return 1;
	{
		fprintf(outHandle,"OREGON MESSAGE SWITCH - DEVICE USAGE ACTIVITY\n\n");
	}
	return 0;
}

int writeStatsFile(void)
{
	KeyListEntry * LPtr;
	KeyListEntry * DPtr;
	KeyListEntry * KPsorted;

	if (!STATS_TREE)
		return 1;	// nothing to report

	fprintf(outHandle, "GRAND TOTAL = %d\n", STATS_TREE->k_id);

	KPsorted = KeyListNameSort(STATS_TREE->next);
	STATS_TREE->next = KPsorted;
	for (LPtr = STATS_TREE->next; LPtr; LPtr = LPtr->next)
	{
		fprintf(outHandle,"LINE %s TOTAL = %d\n", LPtr->k_name, LPtr->k_id);
		KPsorted = KeyListNameSort(LPtr->k_list);
		LPtr->k_list = KPsorted;
		for (DPtr = LPtr->k_list;DPtr; DPtr = DPtr->next)
		{
			fprintf(outHandle,"          %s  = %d\n", DPtr->k_name, DPtr->k_id);
		}
	}
	fflush(outHandle);
	return 0;
}

int closeStatsFile(void)
{
	fflush(outHandle);
	fclose(outHandle);
	return 0;
}


// THE MAIN PROGRAM 

int main(int argc, char * argv[])
{
	KeyListEntry * dirList;
	KeyListEntry * fileList;

	char * startDTime = "ccyy-mm-dd hh:mm:ss";
	char * endDTime = "ccyy-mm-dd hh:mm:ss";
	char * lemsHome = "C:\\lems2k";
	char * eventlogs = "eventlogs";
    char * outfile;
	struct tm evLogTm;
	char * LineTable[10];	// perform statistics for upto 10 lines
	int NbrLines;			// number of lines for statistics
	int i;
	time_t msgtime;
	
	char rptFile[NAME_MAX +1];

	KeyListEntry	* DListPtr = NULL;
	KeyListEntry	* FListPtr = NULL;

	int init_err = FALSE;

		//  PGM lemsdir evlogdir start end outfile line1 line2 ...

	if (argc < 7 || argc >16)
	{
		init_err = TRUE;
		usage(argc, argv);
		exit(1);
	}

	lemsHome = strdup(argv[1]);
	eventlogs = strdup(argv[2]);
	startDTime = strdup(argv[3]);
	endDTime = strdup(argv[4]);
	outfile = strdup(argv[5]);

	NbrLines = argc - 6;
	for (i=0; i < NbrLines; i++)
	{
		LineTable[i] = strdup(argv[i + 6]);
	}


	sprintf(rptFile,"%s\\xfer",lemsHome);

    if (testDirExists(rptFile) == FALSE)
	{
		init_err = TRUE;
		printf("\nERROR: LEMS\\xfer Directory %s DOES NOT EXIST!!!\n\n",rptFile);
		usage(argc, argv);
		exit(1);
	}

	strcat(rptFile,"\\");
	strcat(rptFile,outfile);

    if (testFileExists(rptFile))
	{
		init_err = TRUE;
		printf("\nERROR: Output File Already Exists:\n  %s\n\n",rptFile);
		usage(argc, argv);
		exit(1);
	}

	if (openStatsFile(rptFile))
	{
		init_err = TRUE;
		printf("\nERROR: Cannot Open Statistics Output File:\n  %s\n\n",rptFile);
		usage(argc, argv);
		exit(1);
	}

	memcpy((void *) &startDateTime, (void *) TmParseDate(startDTime), sizeof(struct tm));
	memcpy((void *) &endDateTime, (void *) TmParseDate(endDTime), sizeof(struct tm));

	if (initializeStatsTree())
		exit(1);		// allocation of stats keylists failed.

	dirList = MakeEvDirList(&startDateTime, &endDateTime, lemsHome, eventlogs);

	for (DListPtr = dirList; DListPtr; DListPtr=GetKeyEntryNext(DListPtr))
	{
		fileList = MakeEvFileList(GetKeyEntryName(DListPtr));

		for (FListPtr = fileList; FListPtr; FListPtr=GetKeyEntryNext(FListPtr))
		{
			char evFileFullPath[NAME_MAX +1];
			LEMSLOGHANDLE	logFileHandle;
			sprintf(evFileFullPath,"%s\\%s",
				GetKeyEntryName(DListPtr),
				GetKeyEntryName(FListPtr));
			logFileHandle = OpenLemsLogFile(evFileFullPath);
			if (logFileHandle)
			{
				// NOTE: LEMS/JX event log is successfully opened.
				// Define a pointer to receive copies of LEMS/JX messages.

				LemsLogMessageStruct * theMessage;

				for (theMessage = GetLemsLogMessage(logFileHandle); theMessage;
					theMessage = GetNextLemsLogMessage (logFileHandle))
				{

			/***
					// NOTE:  (theMessage) points to structured copy of message content
					// and should be freed when done, FreeLemsLogMessage(theMessage);

					// MESSAGE OFFSET INDEX

					(off_t)			= GetLemsLogMessageIx(logFileHandle);

					// MESSAGE HEADER INFORMATION

					(int)			= theMessage->msgHdr->theIndex.IndexType
					(time_t)		= theMessage->msgHdr->theIndex.LogDate;
					(LemsEvType)		= theMessage->msgHdr->theIndex.Event_Id;
					(char[11])		= theMessage->msgHdr->theIndex.MSN;
					(char[9])		= theMessage->msgHdr->theIndex.ISN;
					(char[9])		= theMessage->msgHdr->theIndex.OSN;
					(char[9])		= theMessage->msgHdr->theIndex.Src_Dev;
					(char[9])		= theMessage->msgHdr->theIndex.Dest_Dev;
					(char[10])		= theMessage->msgHdr->theIndex.ORI_Id;
					(char[LEMSMAXUIDLEN])	= theMessage->msgHdr->theIndex.User_Id;
					(char[9])		= theMessage->msgHdr->theIndex.InLine;
					(char[9])		= theMessage->msgHdr->theIndex.OutLine;
					(char[9])		= theMessage->msgHdr->theIndex.InMke;
					(char[9])		= theMessage->msgHdr->theIndex.OutMke;
					(size_t)		= theMessage->msgHdr->theIndex.MsgLen;
					(size_t)		= theMessage->msgHdr->theIndex.LogLen;
					(struct timeval)	= theMessage->msgHdr->theIndex.IN_DTIME;
					(struct timeval)	= theMessage->msgHdr->theIndex.OUT_DTIME;
					(struct timeval)	= theMessage->msgHdr->theIndex.LogTime;
					(char[4])		= theMessage->msgHdr->theIndex.MsgText;

					// MESSAGE BODY CONTENT

					(size_t)		= theMessage->msgData->msgLogLen;
					(size_t)		= theMessage->msgData->msgLength;
					(unsigned char *)	= theMessage->msgData->msgBuffer;
			****/

					msgtime = theMessage->msgHdr->theIndex.LogTime.tv_sec;
					memcpy ((void *)&evLogTm, (void *) localtime(&msgtime), sizeof(struct tm));

					if (
						(tmcomp(&evLogTm, &startDateTime) >= 0) &&
						(tmcomp(&evLogTm, &endDateTime) <= 0))
					{
						int found = 0;
						switch (theMessage->msgHdr->theIndex.Event_Id)
						{
						case INPUTMESSAGE:
							for (i = 0; i < NbrLines; i++)
							{
								if (!strcmp(theMessage->msgHdr->theIndex.InLine, LineTable[i]))
								{
									found = 1;
									break;
								}
							}
							if (found && incrementLineDeviceCounter(
									theMessage->msgHdr->theIndex.InLine,
									theMessage->msgHdr->theIndex.Src_Dev))
							{
								fprintf(outHandle,"\nERROR malloc failure\n");
								fclose(outHandle);
								exit(1);
							}
							break;
//						case OUTPUTMESSAGE:
//							for (i = 0; i < NbrLines; i++)
//							{
//								if (!strcmp(theMessage->msgHdr->theIndex.OutLine, LineTable[i]))
//								{
//									found = 1;
//									break;
//								}
//							}
//							if (found && incrementLineDeviceCounter(
//									theMessage->msgHdr->theIndex.OutLine,
//									theMessage->msgHdr->theIndex.Dest_Dev))
//							{
//								fprintf(outHandle,"\nERROR malloc failure\n");
//								fclose(outHandle);
//								exit(1);
//							}
//							break;
						}
					}
					// GET RID OF THE MESSAGE COPY

					FreeLemsLogMessage(theMessage);
				}

				CloseLemsLogFile (logFileHandle);
			}
		}

		FreeKeyList(&fileList);			// free the directory list of files
	}

	FreeKeyList(&dirList);				// done processing list of logfile directories

	writeStatsFile();
	closeStatsFile();
	freeStatsTree();

	exit(0);							// exit success
}

