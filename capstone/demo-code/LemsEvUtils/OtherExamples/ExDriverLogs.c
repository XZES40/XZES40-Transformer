
//
// Template for LEMS-2000 and LEMS/JX Event Log Processing
//

/**
 * Extract Driver Record Messages from Event Logs - for Terry O'Connel
 *
 * USAGE pgm lemsdir evlogdir start-dtime end-dtime outdir
 *
 * {outdir}\log-date\
 *   DLP-O.log				// InState Driver Request - To OR-DMV
 *   DLR-O.log				// InState Driver Request - To OR-DMV
 *   DLW-O.log				// InState Driver Request - To OR-DMV
 *
 *   DLP-I.log				// InState Driver Response - From OR-DMV
 *   DLR-I.log				// InState Driver Response - From OR-DMV
 *   DLW-I.log				// InState Driver Response - From OR-DMV
 *
 *   DQ-OR-NL.log			// From Oregon to Other State - To NLETS
 *   DQ-NL-OR.log			// From Other State to Oregon - To OR-DMV
 *   DQ-NL-MM.log			// From Other State to Oregon - via MM-MATCH
 *   DQ-MM-OR.log			// From MM-MATCH to Oregon    - To OR-DMV
 *   DR-OR-NL.log			// From Oregon to Other State - From OR-DMV
 *   DR-OR-MM.log			// From Oregon to Other State - via MM-MATCH
 *   DR-MM-NL.log			// From MM-MATCH to Other State - To NLETS
 *   DR-{state}-OR.log		// From Other State to Oregon - From NLETS
 *
 *   KQ-OR-NL.log			// From Oregon to Other State - To NLETS
 *   KQ-NL-OR.log			// From Other State to Oregon - To OR-DMV
 *   KR-OR-NL.log			// From Oregon to Other State - From OR-DMV
 *   KR-{state}-OR.log		// From Other State to Oregon - From NLETS
 */

#include "platform.h"

#include "btcomp.h"

#include "keylist.h"

#include "dirlist.h"

#include "msevlog.h"

#include "lemslog.h"

#if defined(_WIN32)
  #include <direct.h>	// needed to define mkdir()
#endif

#include <stdio.h>
#include <time.h>

KeyListEntry	* dirList = NULL;
KeyListEntry	* fileList = NULL;
KeyListEntry	* outFileList = NULL;

struct tm	startDateTime;
struct tm	endDateTime;

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

// Create Output Directories {outdir}\\ccyy-mm-dd for each date
// Return TRUE=1 for success
// Return FALSE=0 for failure

int createOutputDirs(const char * outdir, const struct tm *startDT, const struct tm *endDT)
{
	int rtn;
	struct tm incrTm;
	char yr_txt[5];
	char mo_txt[3];
	char da_txt[3];
	char fullpath[NAME_MAX +1];

	if (tmcomp(startDT,endDT) == 1)
		return FALSE;		// startDT must be less or equal to endDT

	yr_txt[4] = '\0';
	mo_txt[2] = '\0';
	da_txt[2] = '\0';
	fullpath[NAME_MAX] = '\0';

	// Prevent filename path overflow: Allow for date-string and local filename

	if ((int)strlen(outdir) > (NAME_MAX - 12 - 25))
		return FALSE;

	memcpy((void *) &incrTm, (void *) startDT, sizeof(struct tm));

	for(;(tmcomp(&incrTm, endDT) < 1); incrTm.tm_mday++, mktime(&incrTm))
	{
		sprintf(yr_txt,"%04d", incrTm.tm_year + 1900);
		sprintf(mo_txt,"%02d", incrTm.tm_mon + 1);
		sprintf(da_txt,"%02d", incrTm.tm_mday);
		sprintf(fullpath,"%s\\%s-%s-%s",outdir, yr_txt, mo_txt, da_txt);

		if (!testDirExists(fullpath))
		{
#if defined(__GNUC__)
			rtn = mkdir(fullpath);
			if (rtn)
				return FALSE;	// name conflict or bad permissions
#else	// _WIN32
			rtn = _mkdir(fullpath);
			if (rtn)
				return FALSE;	// name conflict or bad permissions
#endif
		}

	}
	return TRUE;
}

// test File KeyName
// return pointer to KeyListEntry for success (found)
// return NULL for failure (not found)

KeyListEntry * testFileKeyName(const char *fileKey)
{
	KeyListEntry * LPnode;

	for (LPnode = outFileList; LPnode ; LPnode = LPnode->next)
	{
		if (!strcmp(GetKeyEntryName(LPnode), fileKey))
			break;
	}
	return LPnode;
}

int closeOutputFileNode(KeyListEntry * theNode)
{
	int rtn;
	rtn = fflush((FILE *) theNode->k_data);
	rtn = fclose((FILE *) theNode->k_data);
	theNode->k_data = NULL;
	return rtn;
}

// create a new keylist node
// open/create a data file and save its (FILE *) to the list
// save the fName as the file key
// register the closeOutputFileNode function as the keylist destructor

KeyListEntry * getOutputFileNode(const char * outdir, const struct tm *dateTm, const char * fName, const char *fExt)
{
	char fullpath[NAME_MAX +1];
	FILE * fHandle;
	KeyListEntry * theNode = NULL;
	KeyListEntry * lastNode = NULL;
	KeyListEntry * newNode = NULL;

	if (theNode = testFileKeyName(fName))
		return theNode;					// already have an outFileList node

	if (outFileList == NULL)
		outFileList = newNode = NewKeyListEntry();
	else
	{
		lastNode = FindLastKeyListEntry(outFileList);
		newNode = lastNode->next = NewKeyListEntry();
	}

	if (newNode == NULL)
		return NULL;				// memory allocation error

	sprintf(fullpath,"%s\\%04d-%02d-%02d\\%s%s",
		outdir,dateTm->tm_year + 1900,
		dateTm->tm_mon + 1, dateTm->tm_mday,
		fName, fExt);

	if (!(SetKeyEntryName(newNode, fName)))
	{
		if (outFileList == newNode)
			outFileList = DropKeyListEntry(newNode);
		else
			lastNode->next = DropKeyListEntry(newNode);
		return NULL;
	}

	fHandle = fopen(fullpath,"ab+"); // open-create-append binary + read

	if (fHandle == NULL)
	{
		if (outFileList == newNode)
			outFileList = DropKeyListEntry(newNode);
		else
			lastNode->next = DropKeyListEntry(newNode);
		return NULL;
	}

	// Register file I/O handle and close on list destruction

	SetKeyEntryDataAndDestroy(newNode, (void *) fHandle, &closeOutputFileNode);

	return newNode;
}

// Find the fileKey in the keylist of output files and write theData
// return -1 on error
// return zero on success

int writeOutputFileNode(const char * fileKey, const char * theData, size_t theLength)
{
	size_t amt;
	size_t len;

	KeyListEntry * LPtr;

	LPtr = testFileKeyName(fileKey);

	if (!LPtr)
		return -1;	// unable to find output file node

	len = theLength;

	amt = fwrite(theData, 1, len, (FILE *) LPtr->k_data);

	if (amt != len)
		return -1;
	return 0;
}


// THE USAGE INFORMATION - AND EXIT WITH ERROR

void usage(int argc, char * argv[])
{
	fprintf(stderr,"\nUSAGE: %s lemsdir evlogdir start-dtime end-dtime outdir\n\n"
	"lemsdir is the base of LEMSJX or LEMS2000 i.e. F:\\lems2k\n"
	"evlogdir is the eventlog or backevents subdirectory under lemsdir\n"
	"start-dtime is the start date-time in \"ccyy-mm-dd hh:mm:ss\" format\n"
	"end-dtime is the ending date-time in \"ccyy-mm-dd hh:mm:ss\" format\n"
	"outdir is the fully-qualified directory path for logfile extractions\n\n", argv[0]);

	exit (1);
}

// THE MAIN PROGRAM 

main(int argc, char * argv[])
{

	char * startDTime = "ccyy-mm-dd hh:mm:ss";
	char * endDTime = "ccyy-mm-dd hh:mm:ss";
	char * lemsHome = "C:\\lems2k";
	char * eventlogs = "eventlogs";
    char * outdir = "outdir";
	struct tm tvtime;
	char HeaderBuf[80];
	const char * TrailerBuf = "\n==== MESSAGE END ====\n";

	KeyListEntry	* DListPtr = NULL;
	KeyListEntry	* FListPtr = NULL;
	KeyListEntry	* KFileNode = NULL;

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
	outdir = strdup(argv[5]);

	if (testDirExists(outdir) == FALSE)
	{
		init_err = TRUE;
		printf("\nERROR: Directory %s DOES NOT EXIST!!!\n\n",outdir);
		usage(argc, argv);
		exit(1);
	}

	memcpy((void *) &startDateTime, (void *) TmParseDate(startDTime), sizeof(struct tm));
	memcpy((void *) &endDateTime, (void *) TmParseDate(endDTime), sizeof(struct tm));

	if (!createOutputDirs(outdir, &startDateTime, &endDateTime))
	{
		init_err = TRUE;
		printf("\nERROR: Cannot create date-sequenced directories under\n"
			"        %s!!!\n\n",outdir);
		usage(argc, argv);
		exit(1);
	}

	dirList = MakeEvDirList(&startDateTime, &endDateTime, lemsHome, eventlogs);

	for (DListPtr = dirList; DListPtr; DListPtr=GetKeyEntryNext(DListPtr))
	{
		fileList = MakeEvFileList(GetKeyEntryName(DListPtr));

		/* close output files before processing messages in this directory*/

		FreeKeyList(&outFileList);

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

					// Only examine OUTPMESSAGE/INPUTMESSAGE event types containing
					// OR-DMV and NLETS Driver License Messages

					if (theMessage->msgHdr->theIndex.Event_Id == OUTPUTMESSAGE ||
						theMessage->msgHdr->theIndex.Event_Id == INPUTMESSAGE)
					{
						LemsEvType evType;
						time_t timesec;
						char nlstate[3];
						int n1, n2, m1, m2, d1, d2, d3, d4;

						evType = theMessage->msgHdr->theIndex.Event_Id;

						nlstate[2] = '\0';

						n1 = strcmp(theMessage->msgHdr->theIndex.InLine, "NLETS");
						n2 = strcmp(theMessage->msgHdr->theIndex.OutLine, "NLETS");
						m1 = strcmp(theMessage->msgHdr->theIndex.InLine, "MSGMATCH");
						m2 = strcmp(theMessage->msgHdr->theIndex.OutLine, "MSGMATCH");
						d1 = strcmp(theMessage->msgHdr->theIndex.InLine, "DMVP");
						d2 = strcmp(theMessage->msgHdr->theIndex.OutLine, "DMVP");
						d3 = strcmp(theMessage->msgHdr->theIndex.InLine, "DMVT");
						d4 = strcmp(theMessage->msgHdr->theIndex.OutLine, "DMVT");

						// Test for DMVT, DMVP, or NLETS basis for eventlog file
						// to increase performance capability.  The each eventlog file
						// is associated with a device or line io-handler.

						if (n1 && n2 && m1 && m2 && d1 && d2 && d3 && d4)
						{
							// ignore messages not using lines DMVP, DMVT, NLETS or MSGMATCH
							FreeLemsLogMessage(theMessage);
							continue;
						}

						if ((!strcmp(theMessage->msgHdr->theIndex.InMke, "DLP")) && 
							evType == OUTPUTMESSAGE)
						{
							if (d1 == 0 || d3 == 0) // DMV Input "DLP-I" response
							{
								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "DLP-I", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "DLP-I");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"DLP-I",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("DLP-I", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("DLP-I",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("DLP-I", TrailerBuf, strlen(TrailerBuf));

							}
							else if (d2 == 0 || d4 == 0) // DMV Output "DLP-O" inquiry
							{

								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "DLP-O", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "DLP-O");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));


								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"DLP-O",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("DLP-O", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("DLP-O",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("DLP-O", TrailerBuf, strlen(TrailerBuf));

							}
						}
						else if (!strcmp(theMessage->msgHdr->theIndex.InMke, "DLR") &&
								 evType == OUTPUTMESSAGE)
						{
							if (d1 == 0 || d3 == 0) // DMV Input "DLR-I" response
							{
								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "DLR-I", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "DLR-I");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));


								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"DLR-I",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("DLR-I", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("DLR-I",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("DLR-I", TrailerBuf, strlen(TrailerBuf));

							}
							else if (d2 == 0 || d4 == 0) // DMV Output "DLR-O" inquiry
							{
								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "DLR-O", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "DLR-O");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"DLR-O",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("DLR-O", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("DLR-O",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("DLR-O", TrailerBuf, strlen(TrailerBuf));

							}
						}
						else if (!strcmp(theMessage->msgHdr->theIndex.InMke, "DLW") &&
								 evType == OUTPUTMESSAGE)
						{
							if (d1 == 0 || d3 == 0) // DMV Input "DLW-I" inquiry
							{
								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "DLW-I", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "DLW-I");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"DLW-I",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("DLW-I", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("DLW-I",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("DLW-I", TrailerBuf, strlen(TrailerBuf));

							}
							else if (d2 == 0 || d4 == 0) // DMV Output "DLW-O" inquiry
							{
								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "DLW-O", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "DLW-O");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"DLW-O",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("DLW-O", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("DLW-O",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("DLW-O", TrailerBuf, strlen(TrailerBuf));

							}
						}
						else if (!strcmp(theMessage->msgHdr->theIndex.InMke, "DQ"))
						{
							if (n1 == 0 && evType == INPUTMESSAGE)	// NLETS Input "DQ-NL-OR" - DMV Output inquiry
							{
								strcpy(nlstate, "NL");

								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "DQ-NL-OR", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "DQ-NL-OR");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"DQ-NL-OR",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("DQ-NL-OR", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("DQ-NL-OR",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("DQ-NL-OR", TrailerBuf, strlen(TrailerBuf));

							}
							else if (m1 == 0 && evType == OUTPUTMESSAGE)	// MSGM Output "DQ-MM-OR"
							{
								strcpy(nlstate, "MM");

								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "DQ-MM-OR", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "DQ-MM-OR");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"DQ-MM-OR",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("DQ-MM-OR", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("DQ-MM-OR",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("DQ-MM-OR", TrailerBuf, strlen(TrailerBuf));

							}
							else if (n2 == 0)		// NLETS Output "DQ-OR-NL" - User Input
							{
								strcpy(nlstate, "NL");

								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "DQ-OR-NL", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "DQ-OR-NL");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"DQ-OR-NL",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("DQ-OR-NL", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("DQ-OR-NL",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("DQ-OR-NL", TrailerBuf, strlen(TrailerBuf));

							}
						}
						else if (!strcmp(theMessage->msgHdr->theIndex.InMke, "DR"))
						{
							if (n1 == 0 && evType == INPUTMESSAGE)		// NLETS Input "DR-nl-OR" - User Output
							{
								char FileKey[10];
								nlstate[0] = theMessage->msgHdr->theIndex.ORI_Id[0];
								nlstate[1] = theMessage->msgHdr->theIndex.ORI_Id[1];
								nlstate[2] = '\0';
								sprintf(FileKey,"DR-%s-OR",nlstate);

								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, FileKey, ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, FileKey);
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									FileKey,
									theMessage->msgData->msgLogLen);

								writeOutputFileNode(FileKey, HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode(FileKey,
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode(FileKey, TrailerBuf, strlen(TrailerBuf));

							}
							else if (n2 == 0)		// NLETS Output "DR-OR-NL" - DMV/MM response
							{
								strcpy(nlstate, "NL");

								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "DR-OR-NL", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "DR-OR-NL");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"DR-OR-NL",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("DR-OR-NL", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("DR-OR-NL",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("DR-OR-NL", TrailerBuf, strlen(TrailerBuf));
							}
							else if (m2 == 0)		// MSGMATCH Output "DR-OR-MM" - DMV/MM response
							{
								strcpy(nlstate, "MM");

								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "DR-OR-MM", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "DR-OR-MM");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"DR-OR-MM",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("DR-OR-MM", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("DR-OR-MM",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("DR-OR-MM", TrailerBuf, strlen(TrailerBuf));

							}
						}
						else if (!strcmp(theMessage->msgHdr->theIndex.InMke, "KQ"))
						{
							if (n1 == 0 && evType == INPUTMESSAGE)		// NLETS Input "KQ-NL-OR" - DMV Output inquiry
							{
								strcpy(nlstate,"NL");

								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "KQ-NL-OR", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "KQ-NL-OR");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"KQ-NL-OR",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("KQ-NL-OR", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("KQ-NL-OR",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("KQ-NL-OR", TrailerBuf, strlen(TrailerBuf));

							}
							else if (n2 == 0)		// NLETS Output "KQ-OR-NL" - User Input
							{
								strcpy(nlstate,"NL");

								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "KQ-OR-NL", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "KQ-OR-NL");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"KQ-OR-NL",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("KQ-OR-NL", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("KQ-OR-NL",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("KQ-OR-NL", TrailerBuf, strlen(TrailerBuf));

							}
						}
						else if (!strcmp(theMessage->msgHdr->theIndex.InMke, "KR"))
						{
							if (n1 == 0 && evType == INPUTMESSAGE)		// NLETS Input "KR-nl-OR" - Response
							{
								char FileKey[10];
								nlstate[0] = theMessage->msgHdr->theIndex.ORI_Id[0];
								nlstate[1] = theMessage->msgHdr->theIndex.ORI_Id[1];
								nlstate[2] = '\0';
								sprintf(FileKey,"KR-%s-OR",nlstate);

								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, FileKey, ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, FileKey);
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									FileKey,
									theMessage->msgData->msgLogLen);

								writeOutputFileNode(FileKey, HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode(FileKey,
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode(FileKey, TrailerBuf, strlen(TrailerBuf));

							}
							else if (n2 == 0)		// NLETS Output "KR-OR-NL" - DMV Input response
							{
								strcpy(nlstate,"NL");

								memcpy((void *) &tvtime,
									(void *) localtime(&theMessage->msgHdr->theIndex.LogDate),
									sizeof(struct tm));

								KFileNode = getOutputFileNode(outdir, &tvtime, "KR-OR-NL", ".log");

								if (!KFileNode)
								{
									printf("\nFATAL ERROR: Unable To Create Output File - %04d-%02d%-2d\\%s.log\n",
										tvtime.tm_year + 1900,
										tvtime.tm_mon + 1,
										tvtime.tm_mday, "KR-OR-NL");
									FreeKeyList(&outFileList);
									exit (1);
								}

								timesec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
								memcpy((void *) &tvtime,
									(void *) localtime(&timesec),
									sizeof(struct tm));

								sprintf(HeaderBuf,"#### %04d-%02d-%02d %02d:%02d:%02d #### %s #### %d ####\n",
									tvtime.tm_year + 1900,
									tvtime.tm_mon + 1,
									tvtime.tm_mday,
									tvtime.tm_hour,
									tvtime.tm_min,
									tvtime.tm_sec,
									"KR-OR-NL",
									theMessage->msgData->msgLogLen);

								writeOutputFileNode("KR-OR-NL", HeaderBuf, strlen(HeaderBuf));

								writeOutputFileNode("KR-OR-NL",
									theMessage->msgData->msgBuffer, theMessage->msgData->msgLogLen);

								writeOutputFileNode("KR-OR-NL", TrailerBuf, strlen(TrailerBuf));

							}
						}
					}

					// GET RID OF THE MESSAGE COPY

					FreeLemsLogMessage(theMessage);
				}

				CloseLemsLogFile (logFileHandle);
			}
		}

		FreeKeyList(&outFileList);		// closes all message output file handles for the day

		FreeKeyList(&fileList);			// free the directory list of files
	}

	FreeKeyList(&dirList);				// done processing list of logfile directories

	exit(0);
}

