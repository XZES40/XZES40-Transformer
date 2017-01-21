
//
// Template for LEMS-2000 and LEMS/JX Event Log Processing
//

/**
 * Extract Photo Statistics - for Terry O'Connel
 *
 * USAGE pgm lemsdir evlogdir start-dtime end-dtime outdir
 *
 * Counters (By Month)
 *
 * DPL-OREGON				in-state only
 *
 * DQ-TO-NLETS				from Oregon
 *   State-Counters
 * DR-FROM-NLETS			to Oregon
 *   State-Counters
 *
 * DQ-FROM-NLETS			to Oregon
 *   State-Counters
 * DR-TO-NLETS				from Oregon
 *   State-Counters
 *
 * CPQ-INQUIRY-TO			Corrections Photos
 *   Oregon or NLETS
 * CPR-RESPONSE-FR			Corrections Photos
 *   Oregon or NLETS
 *
 *----------------------------------------------------
 * KeyListStructures
 *
 * Name = state-code
 * Value = counter
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

FILE * csvHandle;

struct tm ReportTM;
struct tm NextTM;

KeyListEntry * DQ_TO_NLETS = NULL;
	// Counters = State Code or UNK
KeyListEntry * DQ_FROM_NLETS = NULL;
	// Counters = State Code or UNK
KeyListEntry * DR_TO_NLETS = NULL;
	// Counters = State Code or UNK
KeyListEntry * DR_FROM_NLETS = NULL;
	// Counters = State Code or UNK
KeyListEntry * CPQ_INQUIRY_TO = NULL;
	// Counters = State Code or UNK
KeyListEntry * CPR_RESPONSE_FR = NULL;
	// Counters = State Code or UNK
KeyListEntry * DPL_PHOTOS = NULL;
	// Counters = OREGON, NLETS
KeyListEntry * DPL_INQUIRY = NULL;
	// Counters = OREGON, NLETS

KeyListEntry ** ListTable[] = {
	&DPL_INQUIRY, &DPL_PHOTOS,
	&DQ_TO_NLETS, &DQ_FROM_NLETS,
	&DR_TO_NLETS, &DR_FROM_NLETS,
	&CPQ_INQUIRY_TO, &CPR_RESPONSE_FR, NULL };

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
	fprintf(stderr,"\nUSAGE: %s lemsdir evlogdir start-dtime end-dtime outcsv\n\n"
	"lemsdir is the base of LEMSJX or LEMS2000 i.e. F:\\lems2k\n"
	"evlogdir is the eventlog or backevents subdirectory under lemsdir\n"
	"start-dtime is the start date-time in \"ccyy-mm-dd hh:mm:ss\" format\n"
	"end-dtime is the ending date-time in \"ccyy-mm-dd hh:mm:ss\" format\n"
	"outcsv is the .csv filename created in the lemsdir\\xfer directory\n\n", argv[0]);

	exit (1);
}


// Return 0 on success, non-zero on failure
int initializeStatsKeyLists(void)
{
	if (! DQ_TO_NLETS)
	{
		DQ_TO_NLETS = NewKeyListEntry();
		if (DQ_TO_NLETS)
		{
			if (!SetKeyEntryName(DQ_TO_NLETS, "DQ_TO_NLETS"))
			{
				mallocErr = 1;
				return 1;
			}
		}
		else
		{
			mallocErr = 1;
			return 1;
		}
	}
	if (! DQ_FROM_NLETS)
	{
		DQ_FROM_NLETS = NewKeyListEntry();
		if (DQ_FROM_NLETS)
		{
			if (!SetKeyEntryName(DQ_FROM_NLETS, "DQ_FROM_NLETS"))
			{
				mallocErr = 1;
				return 1;
			}
		}
		else
		{
			mallocErr = 1;
			return 1;
		}
	}
	if (! DR_TO_NLETS)
	{
		DR_TO_NLETS = NewKeyListEntry();
		if (DR_TO_NLETS)
		{
			if (!SetKeyEntryName(DR_TO_NLETS, "DR_TO_NLETS"))
			{
				mallocErr = 1;
				return 1;
			}
		}
		else
		{
			mallocErr = 1;
			return 1;
		}
	}
	if (! DR_FROM_NLETS)
	{
		DR_FROM_NLETS = NewKeyListEntry();
		if (DR_FROM_NLETS)
		{
			if (!SetKeyEntryName(DR_FROM_NLETS, "DR_FROM_NLETS"))
			{
				mallocErr = 1;
				return 1;
			}
		}
		else
		{
			mallocErr = 1;
			return 1;
		}
	}
	if (! CPQ_INQUIRY_TO)
	{
		CPQ_INQUIRY_TO = NewKeyListEntry();
		if (CPQ_INQUIRY_TO)
		{
			if (!SetKeyEntryName(CPQ_INQUIRY_TO, "CPQ_INQUIRY_TO"))
			{
				mallocErr = 1;
				return 1;
			}
		}
		else
		{
			mallocErr = 1;
			return 1;
		}
	}
	if (! CPR_RESPONSE_FR)
	{
		CPR_RESPONSE_FR = NewKeyListEntry();
		if (CPR_RESPONSE_FR)
		{
			if (!SetKeyEntryName(CPR_RESPONSE_FR, "CPR_RESPONSE_FR"))
			{
				mallocErr = 1;
				return 1;
			}
		}
		else
		{
			mallocErr = 1;
			return 1;
		}
	}
	if (! DPL_PHOTOS)
	{
		DPL_PHOTOS = NewKeyListEntry();
		if (DPL_PHOTOS)
		{
			if (!SetKeyEntryName(DPL_PHOTOS, "DPL_PHOTOS"))
			{
				mallocErr = 1;
				return 1;
			}
		}
		else
		{
			mallocErr = 1;
			return 1;
		}
	}
	if (! DPL_INQUIRY)
	{
		DPL_INQUIRY = NewKeyListEntry();
		if (DPL_INQUIRY)
		{
			if (!SetKeyEntryName(DPL_INQUIRY, "DPL_INQUIRY"))
			{
				mallocErr = 1;
				return 1;
			}
		}
		else
		{
			mallocErr = 1;
			return 1;
		}
	}
	return 0;
}

void freeStatsKeyLists(void)
{
	FreeKeyList(&DQ_TO_NLETS);
	FreeKeyList(&DQ_FROM_NLETS);
	FreeKeyList(&DR_TO_NLETS);
	FreeKeyList(&DR_FROM_NLETS);
	FreeKeyList(&CPQ_INQUIRY_TO);
	FreeKeyList(&CPR_RESPONSE_FR);
	FreeKeyList(&DPL_PHOTOS);
	FreeKeyList(&DPL_INQUIRY);
}

// return 0 on success otherwise failure
int incrementKeyCounter(const char * listId, const char * stateId)
{
	int				found = 0;
	KeyListEntry	*KPtr = NULL;

	if (!strcmp(listId, "DQ_TO_NLETS"))
		KPtr = DQ_TO_NLETS;
	else if (!strcmp(listId, "DQ_FROM_NLETS"))
		KPtr = DQ_FROM_NLETS;
	else if (!strcmp(listId,"DR_TO_NLETS"))
		KPtr = DR_TO_NLETS;
	else if (!strcmp(listId,"DR_FROM_NLETS"))
		KPtr = DR_FROM_NLETS;
	else if (!strcmp(listId,"CPQ_INQUIRY_TO"))
		KPtr = CPQ_INQUIRY_TO;
	else if (!strcmp(listId,"CPR_RESPONSE_FR"))
		KPtr = CPR_RESPONSE_FR;
	else if (!strcmp(listId,"DPL_PHOTOS"))
		KPtr = DPL_PHOTOS;
	else if (!strcmp(listId,"DPL_INQUIRY"))
		KPtr = DPL_INQUIRY;
	else
		return 2;	// unknown list
	KPtr->k_id++;
	for (;KPtr->next != NULL; KPtr = KPtr->next)
	{
		if (!strcmp(KPtr->k_name, stateId))
		{
			found = 1;
			break;
		}
	}
	if (strcmp(KPtr->k_name, stateId))
	{
		KPtr->next = NewKeyListEntry();
		KPtr = KPtr->next;
		if (!KPtr)
			return 1;	// allocation error
		if (!SetKeyEntryName(KPtr,stateId))
			return 1;	// allocation error
	}
	KPtr->k_id++;
	return 0;
}

// return 0 on success, otherwise error
int openMonthlyStatsFile(const char * outcsv)
{
	if (!(csvHandle = fopen(outcsv, "w+")))
		return 1;
	{
		fprintf(csvHandle,"OREGON STATISTICS FOR PHOTO PROJECTS\n\n");
		fprintf(csvHandle,"YEAR-MONTH,COLLECTION,STATE,COUNT,TOTALS\n");
	}
	return 0;
}

int writeMonthlyStatsFile(void)
{
	KeyListEntry * LPtr;
	KeyListEntry * LPsorted;
	int i;
	for (i=0; ListTable[i]; i++)
	{
		LPtr = *ListTable[i];
		fprintf(csvHandle,"\n%4d-%02d,%s,,,%d\n",
			ReportTM.tm_year + 1900,
			ReportTM.tm_mon + 1,
			GetKeyEntryName(LPtr),
			GetKeyEntryIdValue(LPtr));

		LPsorted = KeyListNameSort(LPtr->next);	// sort by state code

//		for (LPtr = LPtr->next; LPtr; LPtr= LPtr->next)
		for (LPtr = LPsorted; LPtr; LPtr= LPtr->next)
		{
			fprintf(csvHandle,",,%s,%d\n",
				GetKeyEntryName(LPtr),
				GetKeyEntryIdValue(LPtr));
		}
	}
	fflush(csvHandle);
	return 0;
}

int closeMonthlyStatsFile(void)
{
	fflush(csvHandle);
	fclose(csvHandle);
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
	struct tm evLogTM;
	
	char csvFile[NAME_MAX +1];

	KeyListEntry	* DListPtr = NULL;
	KeyListEntry	* FListPtr = NULL;

	int init_err = FALSE;

	if (argc != 6)
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

	sprintf(csvFile,"%s\\xfer",lemsHome);

    if (testDirExists(csvFile) == FALSE)
	{
		init_err = TRUE;
		printf("\nERROR: LEMS\\xfer Directory %s DOES NOT EXIST!!!\n\n",csvFile);
		usage(argc, argv);
		exit(1);
	}

	strcat(csvFile,"\\");
	strcat(csvFile,outfile);

    if (testFileExists(csvFile))
	{
		init_err = TRUE;
		printf("\nERROR: Output File Already Exists:\n  %s\n\n",csvFile);
		usage(argc, argv);
		exit(1);
	}

	if (openMonthlyStatsFile(csvFile))
	{
		init_err = TRUE;
		printf("\nERROR: Cannot Open csv Output File:\n  %s\n\n",csvFile);
		usage(argc, argv);
		exit(1);
	}

	memcpy((void *) &startDateTime, (void *) TmParseDate(startDTime), sizeof(struct tm));
	memcpy((void *) &endDateTime, (void *) TmParseDate(endDTime), sizeof(struct tm));

	memcpy((void *) &ReportTM, (void *) &startDateTime, sizeof(struct tm));
	ReportTM.tm_mday = 1; // start of first day of month
	ReportTM.tm_hour = 0;
	ReportTM.tm_min = 0;
	ReportTM.tm_sec = 0;
	memcpy((void *) &NextTM, (void *) &ReportTM, sizeof(struct tm));
	NextTM.tm_mon++;
	NextTM.tm_mday = 0;
	NextTM.tm_hour=23;
	NextTM.tm_min=59;
	NextTM.tm_sec=59;
	mktime(&NextTM);	// midnight on last day of month.

	if (initializeStatsKeyLists())
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
					memcpy((void *) &evLogTM, (void *) localtime(&theMessage->msgHdr->theIndex.LogDate), sizeof(struct tm));
					if (tmcomp(&evLogTM, &NextTM) > 0)
					{
						writeMonthlyStatsFile();

						memcpy((void *) &ReportTM, &evLogTM, sizeof(struct tm));
						ReportTM.tm_mday = 1; // start of first day of month
						ReportTM.tm_hour = 0;
						ReportTM.tm_min = 0;
						ReportTM.tm_sec = 0;
						memcpy((void *) &NextTM, (void *) &ReportTM, sizeof(struct tm));
						NextTM.tm_mon++;
						NextTM.tm_mday = 0;
						NextTM.tm_hour=23;
						NextTM.tm_min=59;
						NextTM.tm_sec=59;
						mktime(&NextTM);	// midnight on last day of month.

						freeStatsKeyLists();
						initializeStatsKeyLists();
					}

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

					if (theMessage->msgHdr->theIndex.Event_Id == INPUTMESSAGE)
					{
						if (!strcmp(theMessage->msgHdr->theIndex.InMke, "DQ") &&
							!strcmp(theMessage->msgHdr->theIndex.InLine, "NLETS"))
						{
							char * p;
							char st[3];
							if (p = strstr(theMessage->msgData->msgBuffer,"BinaryRequestIndicator>"))
							{
								p += 23;
								if (strchr("1Tt",p[0]))
								{
									st[0] = theMessage->msgHdr->theIndex.ORI_Id[0];
									st[1] = theMessage->msgHdr->theIndex.ORI_Id[1];
									st[2] = '\0';
									incrementKeyCounter("DQ_FROM_NLETS",st);
								}
							}
						}
						else if (!strcmp(theMessage->msgHdr->theIndex.InMke, "DR") &&
							!strcmp(theMessage->msgHdr->theIndex.InLine, "NLETS"))
						{
							char * p;
							char st[3];
							if (p = strstr(theMessage->msgData->msgBuffer,"BinaryAvailableIndicator>"))
							{
								p += 25;
								if (strchr("1Tt",p[0]))
								{
									st[0] = theMessage->msgHdr->theIndex.ORI_Id[0];
									st[1] = theMessage->msgHdr->theIndex.ORI_Id[1];
									st[2] = '\0';
									incrementKeyCounter("DR_FROM_NLETS",st);
								}
							}
						}
						else if (!strcmp(theMessage->msgHdr->theIndex.InMke, "DPL") &&
							(!strcmp(theMessage->msgHdr->theIndex.InLine, "DMVT") ||
							!strcmp(theMessage->msgHdr->theIndex.InLine, "DMVP")))
						{
							if (strstr(theMessage->msgData->msgBuffer, ".IMG/"))
							{
								if (strstr(theMessage->msgData->msgBuffer, "MMNLET"))
									incrementKeyCounter("DPL_PHOTOS", "NLETS");
								else
									incrementKeyCounter("DPL_PHOTOS", "OREGON");
							}
						}
					}
					else if (theMessage->msgHdr->theIndex.Event_Id == OUTPUTMESSAGE)
					{
						if (!strcmp(theMessage->msgHdr->theIndex.InMke, "DQ") &&
							!strcmp(theMessage->msgHdr->theIndex.OutLine, "NLETS"))
						{
							char * p;
							char st[3];
							if (p = strstr(theMessage->msgData->msgBuffer,"BinaryRequestIndicator>"))
							{
								p += 23;
								if (strchr("1Tt",p[0]))
								{
									for (p = theMessage->msgData->msgBuffer; p &&
										(p = strstr(p, "DocumentDestinationID>")); p++)
									{
										if (p=strstr(p, ":ID>"))
										{
											p += 4;
											st[0] = p[0];
											st[1] = p[1];
											st[2] = '\0';
											incrementKeyCounter("DQ_TO_NLETS",st);
											if (!(p = strstr(p, "DocumentDestinationID>")))
											{
												p = theMessage->msgData->msgBuffer;
												p += (theMessage->msgData->msgLogLen -1);
											}
										}
									}
								}
							}
						}
						else if (!strcmp(theMessage->msgHdr->theIndex.InMke, "DR") &&
							!strcmp(theMessage->msgHdr->theIndex.OutLine, "NLETS"))
						{
							char * p;
							char st[3];
							if (p = strstr(theMessage->msgData->msgBuffer,"BinaryAvailableIndicator>"))
							{
								p += 25;
								if (strchr("1Tt",p[0]))
								{
									for (p = theMessage->msgData->msgBuffer; p &&
										(p = strstr(p, "DocumentDestinationID>")); p++)
									{
										if (p=strstr(p, ":ID>"))
										{
											p += 4;
											st[0] = p[0];
											st[1] = p[1];
											st[2] = '\0';
											incrementKeyCounter("DR_TO_NLETS",st);
											if (!(p = strstr(p, "DocumentDestinationID>")))
											{
												p = theMessage->msgData->msgBuffer;
												p += (theMessage->msgData->msgLogLen -1);
											}
										}
										else
										{
											p = theMessage->msgData->msgBuffer;
											p += (theMessage->msgData->msgLogLen -1);
										}
									}
									for (p = theMessage->msgData->msgBuffer; p &&
										(p = strstr(p, "DocumentSubject.Organization>")); p++)
									{
										if (p=strstr(p,"OrganizationORIID>"))
										{
											if (p=strstr(p, ":ID>"))
											{
												p += 4;
												st[0] = p[0];
												st[1] = p[1];
												st[2] = '\0';
												incrementKeyCounter("DR_TO_NLETS",st);
												if (!(p = strstr(p, "DocumentSubject.Organization>")))
												{
													p = theMessage->msgData->msgBuffer;
													p += (theMessage->msgData->msgLogLen -1);
												}
											}
										}
										else
										{
											p = theMessage->msgData->msgBuffer;
											p += (theMessage->msgData->msgLogLen -1);
										}
									}
								}
							}
						}
						else if (!strcmp(theMessage->msgHdr->theIndex.InMke, "DPL") &&
							(!strcmp(theMessage->msgHdr->theIndex.OutLine, "DMVT") ||
							!strcmp(theMessage->msgHdr->theIndex.OutLine, "DMVP")))
						{
							if (!strcmp(theMessage->msgHdr->theIndex.InLine, "MSGMATCH"))
								incrementKeyCounter("DPL_INQUIRY", "NLETS");
							else
								incrementKeyCounter("DPL_INQUIRY", "OREGON");
						}
						else if (!strcmp(theMessage->msgHdr->theIndex.InMke, "CPQ") &&
							((!strcmp(theMessage->msgHdr->theIndex.OutLine, "MUGS")) ||
							(!strcmp(theMessage->msgHdr->theIndex.OutLine, "NLETS"))))
						{
							if (!strcmp(theMessage->msgHdr->theIndex.OutLine, "NLETS"))
								incrementKeyCounter("CPQ_INQUIRY_TO", "NLETS");
							else
								incrementKeyCounter("CPQ_INQUIRY_TO", "OREGON");
						}
						else if (!strcmp(theMessage->msgHdr->theIndex.InMke, "CPR") &&
							((!strcmp(theMessage->msgHdr->theIndex.InLine, "MUGS")) ||
							(!strcmp(theMessage->msgHdr->theIndex.InLine, "NLETS"))))
						{
							if (!strcmp(theMessage->msgHdr->theIndex.InLine, "NLETS"))
								incrementKeyCounter("CPR_RESPONSE_FR", "NLETS");
							else
								incrementKeyCounter("CPR_RESPONSE_FR", "OREGON");
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

	writeMonthlyStatsFile();
	closeMonthlyStatsFile();
	freeStatsKeyLists();

	exit(0);							// exit success
}

