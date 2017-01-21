/*
 * 'C' Source Library - Common Utilities - Supported by GNU and Windows VC7.1
 *
 * Copyright (c) 2008 - 2010	Oregon State Police
 *
 *     This is open source software in the pulbic domain.
 *
 * Developed by: Steven J. Hathaway
 *
 * For:  State of Oregon - Oregon State Police / CJIS-LEDS
 *
 * Rev  0.05     Aug 24 2011            Hathaway
 *      Fix EventLog Type 5 Support for LEMS 3.4.3 (Release)
 *
 * Rev  0.04     Nov 08 2010            Hathaway
 *      Add EventLog Type 5 Support - LEMS 3.4.0 (Prototype)
 *
 * Rev  0.03     Nov 12 2009            Hathaway
 *   Change time_t to unsigned int, accommodating a development platform upgrade.
 *   Studio .NET 2005 (VC8) changes typedef time_t from 32-bit to 64-bit quantity.
 * 
 * Rev  0.02     Oct 17 2009               Hathaway
 *   Restored filename validity check - program written to edit log file headers
 *   with new filenames after the event log files are renamed.
 *
 * Rev  0.01     Oct 12 2009               Hathaway
 *   Removed filename validity check - stratus ftServer cutover.
 */

#include "platform.h"
#include "lemslog.h"

#include "msevlog.h"

#if defined (_WIN32)
 #include <winsock2.h>
#endif

#include <fcntl.h>
#include <string.h>

#if defined(_WIN32)
#include <io.h>
#endif

/* For Debugging */
#include <assert.h>

/*
 * Standard I/O Routines using Microsoft crt (The C RunTime Library).
 * NOTE: Microsoft crt may limit files to 2 GB in size (_off_t) issue.
 * Other 32_bit architectures limit files to 4 GB in size (off_t) issue.
 * This may not be a problem with 64_bit architectures, but the UNISYS
 * LEMS message switch code is compiled for 32_bit architecures and will
 * not currently operate on 64_bit systems.
 *
 * 64_bit architecures do not have this filesize limit, but LEMS2000
 * is currently compiled as a 32_bit architecture system.
 *
 * There may be problems on Microsoft systems if either _POSIX_ or __STDC__
 * are defined as precompiler macros.  If precompiler macros are defined,
 * their default value test is nonzero.  If undefined, their default value
 * test is zero.  The <windows.h> based headers have the many tests like
 *		# if !__STDC__
 * which if defined or != <zero>, do not provide the UNIX standard definitions
 * for common functions.  Microsoft, therefore prefixes the function names
 * with underscore, whereas GNU has no such underscore prefix.
 *      #ifdef _POSIX_
 * Causes Microsoft to supply POSIX constant definitions, but also refuses
 * to provide common UNIX function names without underscore prefix.
 */

/* TO BE DONE and VALIDATED
 * 
 * Make sure the library can report the common issues that corrupt LEMS
 * event log files (System clock changes, System Upgrades, System Downgrades).
 * And system shutdown with incomplete buffer flush before file is closed.
 *
 * This all appears to be working okay. - SJH - Oregon
 */

/* BASIC LEMS EVENT LOG FILE READING.
 *
 * LEMSLOGHANDLE evHandle;
 * LemsLogMessageStruct * theMessage;
 *
 * evHandle = OpenLemsLogFile (char * filename)
 * if (!evHandle)
 *		Could Not Open File, or Invalid for Other Reasons.
 *
 * -- read all messages in the file --
 * for (theMessage = GetLemsLogMessage(evHandle);
 *		theMessage;
 *		theMessage = GetNextLemsLogMessage(evHandle))
 *{
 *		Process the message
 *}
 *
 * -- read only a specified message --
 * SetLemsLogMessageIx(evHandle, off_t offsetValue);
 * theMessage = GetLemsLogMessage(evHandle);
 *		Process the message
 *
 * -- at end of processing
 * FreeLemsLogMessage(theMessage);
 * CloseLemsLogFile(evHandle);
 */

/* CHECKING FOR LOGFILE CORRUPTION
 *
 * Usually log file corruption will not prevent the log file from being
 * processed.  It is possible that one or more messages may be skipped
 * or be unusable.  After calling GetLemsLogMessage() or GetNextLemsLogMessage()
 * is called, the value of theMessage returned will be a pointer or NULL.
 * A NULL value indicates that either end-of-file is reached or there is
 * a system error suchas not enough memory resources for data allocation.
 * A pointer value returned indicates that a message has been found, in
 * which case a check of GetLemsLogErr(evHandle) will indicate whether
 * corrective action was performed when reading the returned message.
 */

/* ================================================================
 * OpenLemsLogFile
 *
 * Return a pointer to the LEMSLOG_HANDLE_STRUCT if the LEMS Event Log
 * File was successfully opened and validated.
 */

LEMSLOGHANDLE OpenLemsLogFile(const char * filename)
{
	LEMSLOGHANDLE evHandle;
	int evFd;
	int rtn;
	const char * cptr;
	const char * sptr;
	char * ptr;
	LemsEvLogHdrType hdrData;
	struct tm *	tmPtr;
	struct tm tmDate;

	/* We must have a filename path. */

	if (!filename || !filename[0])
		return NULL;

#ifdef _WIN32
	evFd = _open(filename, _O_RDONLY | _O_BINARY);
#else
	evFd = open(filename, O_RDONLY);
#endif
	if (evFd == -1)
	{
		/* Cannot Open Log File for Read */
		return NULL;
	}

	evHandle = calloc(1,sizeof(LEMSLOG_HANDLE_STRUCT));
	if (!evHandle)
	{
		/* Memory Allocation Failure */
		close(evFd);
		return NULL;
	}

	evHandle->theFD = evFd;
	if (!(evHandle->theFullPath = strdup(filename)))
	{
		/* Memory Allocation Failure */
		close(evFd);
		free(evHandle);
		return NULL;
	}

	evHandle->theEofLocn = lseek(evFd, 0, SEEK_END);
	if (evHandle->theEofLocn < sizeof(LemsEvLogHdrType))
	{
		/* FILE TOO SMALL ... No EvLog File Header */
		close(evFd);
		free(evHandle->theFullPath);
		free(evHandle);
		return NULL;
	}

	/* Position to start of file and read EvLog File Header */

	lseek(evFd, 0, SEEK_SET);
	rtn = read(evFd, &hdrData, sizeof(LemsEvLogHdrType));
	if (rtn != sizeof(LemsEvLogHdrType))
	{
		/* Problem reading Event Log File Header */
		close(evFd);
		free(evHandle->theFullPath);
		free(evHandle);
		return NULL;
	}

	/* Register start of first message */

	evHandle->curMsgHdrOffset = sizeof(LemsEvLogHdrType);
	
	/* Get a buffer to save some useful EvLog file header information */

	if (!(evHandle->theFileHeader = calloc(1, sizeof(LemsLogFileHeaderStruct))))
	{
		/* Memory Allocation Failure */
		close(evFd);
		free(evHandle->theFullPath);
		free(evHandle);
		return NULL;
	}

	/* Save some useful LEMS2000 Header Information */

	memcpy((void *)evHandle->theFileHeader->appName, (void *)hdrData.Lems2000, 12);
	memcpy((void *)evHandle->theFileHeader->appVersion, (void *)hdrData.Version, 7);

	ptr = hdrData.FileName;
	if (ptr[0] == '/' || ptr[0] == '\\')
	{
		ptr++;
		memcpy((void *)evHandle->theFileHeader->logFileName, (void *)ptr, 15);
	}
	else
		memcpy((void *)evHandle->theFileHeader->logFileName, (void *)ptr, 16);

	memcpy((void *)evHandle->theFileHeader->logCrDate, (void *)hdrData.CreationDate, 16);

	if (tmPtr = TmParseDate(evHandle->theFileHeader->logCrDate))
	{
		/* copy the static (struct tm) into our process scope before calling mktime */
		memcpy((void *)&tmDate, (void *)tmPtr, sizeof(struct tm));
		evHandle->theFileHeader->logCrTime = mktime(&tmDate);
	}

	/* We have a LEMS Event Log File Header, Now Validate Its Content */

	if (evHandle->theFileHeader->logCrTime < 172800)
	{
		/* Bad Parsed Logfile Creation Date */
		close(evFd);
		free(evHandle->theFullPath);
		free(evHandle->theFileHeader);
		free(evHandle);
		return NULL;
	}

	/*
	 * LEMS appends the LEMS process id to the writing program name
	 * then truncates the string to 11 characters.
	 * Currently no LEMS program has numerics in the name, and no "EXE"
	 * extension is applied.  For simple logging, I am removing the numerics.
	 */

	if (ptr = strpbrk(hdrData.CreatingPgm, "0123456789"))
	{
		*ptr = '\0';
	}
	strncpy(evHandle->theFileHeader->logWriter, hdrData.CreatingPgm, 11);

	if ((rtn = strncmp(evHandle->theFileHeader->appName, "LEMS", 4)) != 0)
	{
		/* LEMS EVENT LOG FILE NOT WRITTEN BY LEMS SYSTEM !!! */

//		assert (rtn == 0);  /* Force Debug by initiating an assert trap */

		close(evFd);
		free(evHandle->theFullPath);
		free(evHandle->theFileHeader);
		free(evHandle);
		return NULL;
	}

	/* Find Filename Part of filename path, removing basepath */
	cptr = filename;
	for (sptr = strpbrk(cptr,"\\/"); sptr; sptr = strpbrk(sptr,"\\/"))
		cptr = ++sptr;

	if ((rtn = strncmp(cptr, evHandle->theFileHeader->logFileName, 16)) != 0)
	{
		/* LEMS EVENT LOG FILE APPEARS TO HAVE BEEN RENAMED !!!   */
		/* Registered logFileName != FileName                     */
		/* File may have been edited via text editor, reformated  */

// *** DEBUG *** - TEST TO ACCOMMODATE Stratus ftServer Hardware Cutover
// *** DEBUG *** - RENAMING EVENT LOGFILES 'EVnnn' -> 'EVXnnn'
// *** DEBUG *** - ALLOWING BOTH EVENT LOG DIRECTORIES TO BE MERGED
// *** DEBUG *** - Another program already written to fix Event Log Header Content.

		close(evFd);
		free(evHandle->theFullPath);
		free(evHandle->theFileHeader);
		free(evHandle);
		return NULL;
	}

	/* LEMS Message Log Event Headers have had a history of format changes.
	 * We do not support Type1, requiring separate data file and an index file.
	 * Type2 became effective with LEMS2000 Version 2.0 until Version 2.3
	 * Type3 became effective with LEMS2000 Version 2.3 until Version 3.2
	 * Type4 became effective with LEMS2000 Version 3.2 until Version 3.4
	 * Type5 became effective with LEMSJX   Version 3.4 until present.
	 *
	 * We try to handle situations where message switch updates have
	 * impacted the message log system with incompatible event header types.
	 * HdrTypeFirst = First attempt to validate.
	 * HdrTypeSecond = Second attempt to validate.
	 */

	/* Establish a default order for message index header valdation.   */
	/* This is to accommodate system upgrades and fallback operations. */

	rtn = strncmp(evHandle->theFileHeader->appVersion, "2.3", 3);
	if (rtn < 0)
	{
		evHandle->HdrTypeFirst = 2;
		evHandle->HdrTypeSecond = 0;
	}
	if (rtn == 0 && evHandle->theFileHeader->appVersion[0] == '2')
	{
		evHandle->HdrTypeFirst = 3;
		evHandle->HdrTypeSecond = 2;
	}
	if (rtn > 0 && evHandle->theFileHeader->appVersion[0] == '2')
	{
		evHandle->HdrTypeFirst = 3;
		evHandle->HdrTypeSecond = 0;
	}
	rtn = strncmp(evHandle->theFileHeader->appVersion, "3.2", 3);
	if (rtn < 0 && evHandle->theFileHeader->appVersion[0] == '3')
	{
		evHandle->HdrTypeFirst = 3;
		evHandle->HdrTypeSecond = 4;
	}
	if (rtn >= 0 && evHandle->theFileHeader->appVersion[0] == '3')
	{
		evHandle->HdrTypeFirst = 4;
		if (strncmp(evHandle->theFileHeader->logCrDate, "2009", 4) <= 0)
			evHandle->HdrTypeSecond = 3;
		else
			evHandle->HdrTypeSecond = 5;
	}
	rtn = strncmp(evHandle->theFileHeader->appVersion, "3.4", 3);
	if (rtn >= 0 && evHandle->theFileHeader->appVersion[0] == '3')
	{
		evHandle->HdrTypeFirst = 5;
		evHandle->HdrTypeSecond = 4;
	}

	return evHandle;	/* SUCCESS */
}


/* ================================================================
 * CloseLemsLogFile
 */

int CloseLemsLogFile (LEMSLOGHANDLE theHandle)
{
	if (!theHandle)
		return 0;

	if (theHandle->theFD)
		close(theHandle->theFD);

	if (theHandle->theFullPath)
		free(theHandle->theFullPath);

	if (theHandle->theBuffer)
		free(theHandle->theBuffer);

	if (theHandle->theFileHeader)
		free(theHandle->theFileHeader);

	if (theHandle->theCurrMessage)
		FreeLemsLogMessage(theHandle->theCurrMessage);

	memset(theHandle, 0, sizeof(LEMSLOG_HANDLE_STRUCT));
	free(theHandle);
	return 0;
}

/* ================================================================
 * GetLemsLogCreateTime
 *
 * Return a time_t value indicating when the log file was created.
 */

time_t GetLemsLogCreateTime (LEMSLOGHANDLE theHandle)
{
	if (theHandle && theHandle->theFileHeader)
		return theHandle->theFileHeader->logCrTime;
	return 0;
}

/* ================================================================
 * GetLemsLogFileHeader
 *
 * Makes a copy of the LemsLogFileHeaderStuct associated with theHandle.
 * You should deallocate the copy by calling FreeLemsLogFileHeader().
 */

LemsLogFileHeaderStruct *
GetLemsLogFileHeader (LEMSLOGHANDLE theHandle)
{
	if (theHandle && theHandle->theFileHeader)
		return CloneLemsLogFileHeader(theHandle->theFileHeader);
	return NULL;
}

/* ================================================================
 * CloneLemsLogFileHeader
 *
 * Makes a copy of the LemsLogFileHeaderStruct and returns a pointer
 * to the new copy.  A NULL pointer is returned on memory allocation error.
 */

LemsLogFileHeaderStruct *
CloneLemsLogFileHeader (LemsLogFileHeaderStruct * theHeader)
{
	LemsLogFileHeaderStruct	*tmpHdr = NULL;

	if (theHeader)
	{
		if (tmpHdr = malloc(sizeof(LemsLogFileHeaderStruct)))
			memcpy(tmpHdr, theHeader, sizeof(LemsLogFileHeaderStruct));
	}
	return tmpHdr;
}

/* ================================================================
 * GetLemsLogMsgHeader
 *
 * Makes a copy of the LemsLogMsgHeaderStruct associated with theHandle
 * and the current message.  You should deallocate the copy by calling
 * FreeLemsLogMsgHeader().
 */

LemsLogMsgHeaderStruct *
GetLemsLogMsgHeader (LEMSLOGHANDLE theHandle);

/* ================================================================
 * GetLemsLogMsgData
 *
 * Makes a copy of the LemsLogMsgDataStruct associated with theHandle
 * and the current message.  You should deallocate the copy by calling
 * FreeLemsLogMsgData().
 */

LemsLogMsgDataStruct *
GetLemsLogMsgData (LEMSLOGHANDLE theHandle);

/* ================================================================
 * GetLemsLogMessage
 *
 * Makes a copy of the LemsLogMessageStruct associated with theHandle
 * and the current message.  You should deallocate the copy by calling
 * FreeLemsLogMessage().
 */

LemsLogMessageStruct *
GetLemsLogMessage (LEMSLOGHANDLE theHandle)
{
	LemsLogMessageStruct		* tmpMessage;
	LemsLogMsgHeaderStruct		* tmpMsgHeader;
	LemsLogMsgDataStruct		* tmpMsgData;
	EventsIndexType				* indexInfo;
	size_t						dbuflen;
	size_t						count;
	int							icount;
	size_t						tmpSize;
	int							hdrSize[7];
	
	hdrSize[0] = 0;
	hdrSize[1] = 0;
	hdrSize[2] = sizeof(EventsIndexType2)-4;
	hdrSize[3] = sizeof(EventsIndexType3)-4;
	hdrSize[4] = sizeof(EventsIndexType4)-4;
	hdrSize[5] = sizeof(EventsIndexType5)-3; // Special Offset Required
	hdrSize[6] = 0;

	if (!theHandle)
		return NULL;
	if (theHandle->theCurrMessage)
	{
		/* Current message structure exists, so return a clone */

		if(!(tmpMessage = CloneLemsLogMessage(theHandle->theCurrMessage)))
		{
			theHandle->theErrFlag = 1;
			return NULL;					/* Allocation Failure */
		}
		else
			return tmpMessage;
	}

		/*** Now we must go to work - read the file, validate indexes and store message. ***/

	theHandle->theErrFlag = 0;

	if ((off_t)(theHandle->curMsgHdrOffset + sizeof(EventsIndexType2) -4) > (off_t)(theHandle->theEofLocn))
	{
		theHandle->curMsgHdrOffset = theHandle->theEofLocn;
		return NULL;	// Assuming EOF
	}

	tmpSize = MAX_LEMS_EVTYPE_HEAD + 16000;

	if (theHandle->theBuffer == NULL)
	{
		theHandle->theBuffer = malloc(tmpSize +1);
		theHandle->bufSize = tmpSize;
	}

	if (theHandle->bufSize < (MAX_LEMS_EVTYPE_HEAD + 1000) ||
		theHandle->theBuffer == NULL)
	{
		theHandle->theBuffer = realloc(theHandle->theBuffer, tmpSize + 1);
		theHandle->bufSize = tmpSize;
	}

	if (!theHandle->theBuffer)
	{
		theHandle->theErrFlag = 1;	// not enough memory for file buffer
		return NULL;
	}

	if ((lseek(theHandle->theFD, theHandle->curMsgHdrOffset, SEEK_SET)) == (off_t)-1)
	{
		theHandle->theErrFlag = 1;	// cannot seek on the log file
		return NULL;
	}

	icount = read(theHandle->theFD, theHandle->theBuffer, MAX_LEMS_EVTYPE_HEAD);
	if (icount <=0)
	{
		theHandle->curMsgHdrOffset = theHandle->theEofLocn;
		return NULL;	// assuming EOF
	}
	count = icount;


	/* Clear some bookkeeping data */

	theHandle->nxtMsgHdrOffset = 0;
	
	/* We have a message index header to validate */

	theHandle->bufPtr = theHandle->theBuffer;
	theHandle->bufPtrTail = theHandle->theBuffer + count;
	if (!(indexInfo = TestEventsIndexTypeN(theHandle->HdrTypeFirst,
			theHandle->theFileHeader->logCrTime, theHandle->bufPtr)))
	{
		theHandle->theErrFlag = 3;	// EventsIndexType Fault

		if (!(indexInfo = TestEventsIndexTypeN(theHandle->HdrTypeSecond,
			theHandle->theFileHeader->logCrTime, theHandle->bufPtr)))
		{
			theHandle->theErrFlag = 2; // File Corruption - Overwrite

			/* We were unable to validate the header -- this gets ugly    */
			/* We now try to read blocks of data until end of file or     */
			/* Have a EventsIndexType validation match.  But first try    */
			/* to safely backtrack to find the next valid EventsIndexType */
			/* allowing partial overwrite of the LEMS Events Log file.    */

			if (theHandle->prvMsgHdrOffset)
				theHandle->curMsgHdrOffset = theHandle->prvMsgHdrOffset +1;
			else if(theHandle->curMsgHdrOffset > (sizeof(LemsEvLogHdrType) + 100))
				theHandle->curMsgHdrOffset -= 100;
			
			/* Read some file data into parsing buffer */
			if ((lseek(theHandle->theFD, theHandle->curMsgHdrOffset, SEEK_SET)) == (off_t)-1)
			{
				theHandle->theErrFlag = 1;	// cannot seek on the log file
				return NULL;
			}
			count = (theHandle->bufSize < 32767) ? theHandle->bufSize -64 : 32700;
			icount = read(theHandle->theFD, theHandle->theBuffer, count);
			if (icount <= 0)
			{
				/* Error or End of File */
				theHandle->curMsgHdrOffset = theHandle->theEofLocn;
				return NULL;	// Assuming EOF
			}
			count = icount;
			theHandle->bufPtr = theHandle->theBuffer;
			theHandle->bufPtrTail = theHandle->bufPtr + count;

			/* We now have an initialized parsing buffer based at relocated curMsgHdrOffset */

			for(;(count != 0) && (indexInfo == NULL);)
			{
				theHandle->curMsgHdrOffset++;
				theHandle->bufPtr++;
				if ((theHandle->bufPtrTail - theHandle->bufPtr) < hdrSize[theHandle->HdrTypeFirst])
				{
					/* Read some file data */

					if ((lseek(theHandle->theFD, theHandle->curMsgHdrOffset, SEEK_SET)) == (off_t)-1)
					{
						theHandle->curMsgHdrOffset = theHandle->theEofLocn;
						theHandle->theErrFlag = 1;	// cannot seek on the log file
						return NULL;
					}
					count = (theHandle->bufSize < 32767) ? theHandle->bufSize -64 : 32700;
					icount = read(theHandle->theFD, theHandle->theBuffer, count);
					if (icount <= 0)
					{
						/* End of file or read error */
						icount = 0;
					}
					count = icount;
					theHandle->bufPtr = theHandle->theBuffer;
					theHandle->bufPtrTail = theHandle->bufPtr + count;
				}
				if ((indexInfo = TestEventsIndexTypeN(theHandle->HdrTypeFirst,
						theHandle->theFileHeader->logCrTime, theHandle->bufPtr)) == NULL &&
						theHandle->HdrTypeSecond)
				{
					/* Failed Try 1 and HdrTypeSecond is Declared */
					if ((theHandle->bufPtrTail - theHandle->bufPtr) < hdrSize[theHandle->HdrTypeSecond])
					{
							/* Read some file data */
						if ((lseek(theHandle->theFD, theHandle->curMsgHdrOffset, SEEK_SET)) == (off_t)-1)
						{
							theHandle->theErrFlag = 1;	// cannot seek on the log file
							return NULL;
						}
						count = (theHandle->bufSize < 32767) ? theHandle->bufSize -64 : 32700;
						icount = read(theHandle->theFD, theHandle->theBuffer, count);
						if (icount <= 0)
						{
							/* end of file or read error */
							icount = 0;
						}
						count = icount;
						theHandle->bufPtr = theHandle->theBuffer;
						theHandle->bufPtrTail = theHandle->bufPtr + count;
					}
					/* Now Try 2 */
					indexInfo = TestEventsIndexTypeN(theHandle->HdrTypeSecond,
						theHandle->theFileHeader->logCrTime, theHandle->bufPtr);
				}
			}
		}
		else
		{
			/* Success on Second Try, so swap the testing order for next time*/
			int itmp;
			itmp = theHandle->HdrTypeFirst;
			theHandle->HdrTypeFirst = theHandle->HdrTypeSecond;
			theHandle->HdrTypeSecond = itmp;
		}
	}
	if (!indexInfo)
	{
		/* Could not validate a message index header, assuming EOF */
		/* I've been thinking about a clean EOF test */

		theHandle->curMsgHdrOffset = theHandle->theEofLocn;
		return NULL;
	}
	/* We have valid indexInfo of type EventsIndexType */

	if (!(tmpMessage = malloc(sizeof(LemsLogMessageStruct))))
	{
		/* Memory Allocation Failure */
		free(indexInfo);
		theHandle->theErrFlag = 1;
		return NULL;
	}
	if (!(tmpMsgHeader = malloc(sizeof(LemsLogMsgHeaderStruct))))
	{
		/* Memory Allocation Failure */
		free(indexInfo);
		free(tmpMessage);
		theHandle->theErrFlag = 1;
		return NULL;
	}
	if (!(tmpMsgData = malloc(sizeof(LemsLogMsgDataStruct))))
	{
		/* Memory Allocation Failure */
		free(indexInfo);
		free(tmpMessage);
		free(tmpMsgHeader);
		theHandle->theErrFlag = 1;
		return NULL;
	}

	tmpMsgHeader->msgHdrType = indexInfo->IndexType;
	tmpMsgHeader->msgHdrOffset = theHandle->curMsgHdrOffset;
	tmpMsgHeader->msgHdrLength = hdrSize[indexInfo->IndexType];
	memcpy(&tmpMsgHeader->theIndex, indexInfo, sizeof(EventsIndexType));

	tmpMsgData->msgLength = indexInfo->MsgLen;
	tmpMsgData->msgLogLen = indexInfo->LogLen;
	dbuflen = (indexInfo->MsgLen <= indexInfo->LogLen) ? indexInfo->LogLen : indexInfo->MsgLen;
	tmpMsgData->msgBuffer = calloc(1,dbuflen+4);

	if (tmpMsgData->msgBuffer && dbuflen > 0)
	{
		/* Read the file data */
		lseek(theHandle->theFD, 
			(theHandle->curMsgHdrOffset + (int) tmpMsgHeader->msgHdrLength) ,SEEK_SET);
		icount = read(theHandle->theFD, tmpMsgData->msgBuffer, (unsigned int) dbuflen);
		if (icount <= 0)
		{
			/* end of file or read error */
			icount = 0;
		}
		count = icount;
	}

	/* Register warning value if msglen != loglen - but do not mask serious issues */

//	if ((theHandle->theErrFlag == 0) && (
//	if(		tmpMsgData->msgLength != tmpMsgData->msgLogLen)
//		theHandle->theErrFlag = 5;	// Notice of different message lengths.

	tmpMessage->msgHdr = tmpMsgHeader;
	tmpMessage->msgData = tmpMsgData;

	/** Set the location of the next message in theHandle **/

	theHandle->nxtMsgHdrOffset = theHandle->curMsgHdrOffset
//		+ hdrSize[indexInfo->IndexType] + dbuflen;
		+ hdrSize[indexInfo->IndexType] + (int) indexInfo->LogLen;

	/** Register the message to theHandle structure **/

	theHandle->theCurrMessage = tmpMessage;

	/** Need to Ensure that we return a Message Cloned Copy **/

	free(indexInfo);

	return CloneLemsLogMessage(tmpMessage);
}

/* ================================================================
 * CloneLemsLogMessage
 *
 * Make a copy of the message and return a pointer to the copy.
 * A NULL pointer is returned on error.  This allows a message to be
 * copied instead of being read again from a file handle.
 */

LemsLogMessageStruct *
CloneLemsLogMessage (LemsLogMessageStruct * theMessage)
{
	LemsLogMessageStruct	* tmpMessage;
	LemsLogMsgHeaderStruct	* tmpMsgHeader;
	LemsLogMsgDataStruct	* tmpMsgData;
	size_t		tmpSize;

	if (!theMessage)
		return NULL;

	if (theMessage->msgHdr == NULL || theMessage->msgData == NULL)
		return NULL;	/* incomplete source message */

	if (!(tmpMessage = malloc(sizeof(LemsLogMessageStruct))))
		return NULL;	/* Memory allocation failure */

	if (!(tmpMsgHeader = malloc(sizeof(LemsLogMsgHeaderStruct))))
	{
		free(tmpMessage);
		return NULL;	/* Memory allocation failure */
	}

	if (!(tmpMsgData = malloc(sizeof(LemsLogMsgDataStruct))))
	{
		free(tmpMessage);
		free(tmpMsgHeader);
		return NULL;	/* Memory allocation failure */
	}

	memcpy(tmpMsgData, theMessage->msgData, sizeof(LemsLogMsgDataStruct));
	memcpy(tmpMsgHeader, theMessage->msgHdr, sizeof(LemsLogMsgHeaderStruct));
	
	/** Revisit this when we know the difference between MsgLen and LogLen **/
	/* msgLogLen = amount of message data in log file */
	/* msgLength = application message length info */
	/* Differences can happen if IMG/BINARY DATA REMOVED ... etc. */
	/* tmpSize = max(msgLogLen, msgLength) */

	tmpSize = (tmpMsgData->msgLength <= tmpMsgData->msgLogLen) ?
		tmpMsgData->msgLogLen : tmpMsgData->msgLength;

	if (tmpMsgData->msgBuffer)
	{
		tmpMsgData->msgBuffer = malloc(tmpSize +1);
		if (tmpMsgData->msgBuffer)
		{
			memcpy(tmpMsgData->msgBuffer, theMessage->msgData->msgBuffer, tmpSize);
			tmpMsgData->msgBuffer[tmpSize] = '\0';
		}
	}
	tmpMessage->msgData = tmpMsgData;
	tmpMessage->msgHdr = tmpMsgHeader;
	return tmpMessage;
}


/* ================================================================
 * FreeLemsLogFileHeader
 */

int FreeLemsLogFileHeader (LemsLogFileHeaderStruct * theHeader)
{
	if (theHeader)
		free(theHeader);
	return 0;
}

/* ================================================================
 * FreeLemsLogMsgHeader
 */

int FreeLemsLogMsgHeader (LemsLogMsgHeaderStruct * theHeader)
{
	if (theHeader)
	{
		free(theHeader);
	}
	return 0;
}

/* ================================================================
 * FreeLemsLogMsgData
 */

int FreeLemsLogMsgData (LemsLogMsgDataStruct * theData)
{
	if (theData)
	{
		if (theData->msgBuffer)
			free(theData->msgBuffer);
		free(theData);
	}
	return 0;
}

/* ================================================================
 * FreeLemsLogMessage
 */

int FreeLemsLogMessage (LemsLogMessageStruct * theMessage)
{
	if (theMessage)
	{
		if (theMessage->msgHdr)
			FreeLemsLogMsgHeader(theMessage->msgHdr);
		if (theMessage->msgData)
			FreeLemsLogMsgData(theMessage->msgData);
		free(theMessage);
	}
	return 0;
}

/* ================================================================
 * SetLemsLogMessageIx
 *
 * Success: returns the specified file offset value (theOffset).
 * Failure: returns zero, the internal handle is unchanged.
 */

off_t SetLemsLogMessageIx (LEMSLOGHANDLE theHandle, off_t theOffset)
{
	if (!theHandle)
		return 0;

	if (theOffset < sizeof(LemsEvLogHdrType) || theOffset > theHandle->theEofLocn)
		return 0;

	if (theOffset != theHandle->curMsgHdrOffset)
	{
		theHandle->prvMsgHdrOffset = 0;
		theHandle->nxtMsgHdrOffset = 0;
		theHandle->curMsgHdrOffset = theOffset;
		free(theHandle->theBuffer);
		theHandle->bufSize = 0;
		FreeLemsLogMessage(theHandle->theCurrMessage);
		theHandle->theCurrMessage = NULL;
	}

	return theOffset;
}

/* ================================================================
 * GetLemsLogMessageIx
 */

off_t GetLemsLogMessageIx (LEMSLOGHANDLE theHandle)
{
	if (theHandle)
		return theHandle->curMsgHdrOffset;
	return 0;
}

/* ================================================================
 * GetNextLemsLogMessageIx
 *
 * This set when a successful message is read. It is cleared
 * when the message index offset is explicitly set.
 */

off_t GetNextLemsLogMessageIx (LEMSLOGHANDLE theHandle)
{
	if (theHandle)
		return theHandle->nxtMsgHdrOffset;
	return 0;
}

/* ================================================================
 * GetNextLemsLogMsgHeader
 */

LemsLogMsgHeaderStruct *
GetNextLemsLogMsgHeader (LEMSLOGHANDLE theHandle);

/* ================================================================
 * GetNextLemsLogMessage
 */

LemsLogMessageStruct *
GetNextLemsLogMessage (LEMSLOGHANDLE theHandle)
{
	if (!theHandle)
		return NULL;
	if (theHandle->nxtMsgHdrOffset == 0)
		return NULL;
	theHandle->prvMsgHdrOffset = theHandle->curMsgHdrOffset; 
	if (theHandle->theCurrMessage)
		FreeLemsLogMessage(theHandle->theCurrMessage);
	theHandle->theCurrMessage = NULL;
	theHandle->curMsgHdrOffset = theHandle->nxtMsgHdrOffset;
	theHandle->nxtMsgHdrOffset = 0;

	return GetLemsLogMessage(theHandle);
}

/* ================================================================
 * LemsEvTypeName
 *
 * Return the character string name for the EventsIndexType (Event_Id);
 */

char * LemsEvTypeName(LemsEvType ev)
{
	if (ev == LOGON)
		return "LOGON";
	if (ev == LOGOFF)
		return "LOGOFF";
	if (ev == BADLOGON)
		return "BADLOGON";
	if (ev == INPUTCOMMAND)
		return "INPUTCOMMAND";
	if (ev == INPUTMESSAGE)
		return "INPUTMESSAGE";
	if (ev == ROUTEDONE)
		return "ROUTEDONE";
	if (ev == OUTPUTACK)
		return "OUTPUTACK";
	if (ev == OUTPUTNAK)
		return "OUTPUTNAK";
	if (ev == OUTPUTMESSAGE)
		return "OUTPUTMESSAGE";
	if (ev == QUEUEADD)
		return "QUEUEADD";
	if (ev == QUEUEDELIVERY)
		return "QUEUEDELIVERY";
	if (ev == SUPERTRANS)
		return "SUPERTRANS";
	if (ev == ALTROUTE)
		return "ALTROUTE";
	if (ev == PURGE)
		return "PURGE";
	if (ev == INTSTATCHG)
		return "INTSTATCHG";
	if (ev == GEN_ERROR)
		return "GEN_ERROR";
	if (ev == LEMSINIT)
		return "LEMSINIT";
	if (ev == LEMSCLEA)
		return "LEMSCLEA";
	return "UNKNOWN";
}

/* ================================================================
 * TestEventsIndexTypeN
 *
 * Success: Returns a new structure of EventsIndexType.
 *		Deallocate EventsIndexType object via FreeEventsIndexType().
 * Failure: Returns NULL
 */

EventsIndexType *
TestEventsIndexTypeN (int theType, time_t logDate, const unsigned char * theIndex)
{
	EventsIndexType		hdrInfo;
	EventsIndexType *	rtnInfo;
	const unsigned char * EvIndexPtr = theIndex;
	int i;
//	const u_short	* s_ptr;
//	const time_t	* t_ptr;
	const u_long	* u_ptr;
//	const struct timeval * tv_ptr;
//	unsigned char c_bin[26];

	memset(&hdrInfo, 0, sizeof(hdrInfo));

	if (theType <2 || theType > 5)
		return NULL;

	if (!theIndex)
		return NULL;

/* Accommodate "~|typeid|~" beginning with EventType5 and newer,
 *   Put into the strchr("...",theIndex[2]) the supported types "56789".
 * Types "234" begin with one of LEMS_EV_TYPE_CODES.
 */

	if (EvIndexPtr[0] == '~' &&
		EvIndexPtr[1] == '|' &&
		EvIndexPtr[2] ==  5  &&
		EvIndexPtr[3] == '|' &&
		EvIndexPtr[4] == '~')
	{
		if (!strchr(LEMS_EV_TYPE_CODES, EvIndexPtr[5]))
			return NULL;
		hdrInfo.IndexType = EvIndexPtr[2];
		hdrInfo.Confidential = ((EventsIndexType5 const *) EvIndexPtr)->Confidential;
		EvIndexPtr += 5;
	}
	else if (strchr(LEMS_EV_TYPE_CODES, EvIndexPtr[0]))
	{
		hdrInfo.IndexType = theType;
	}
	else
		return NULL;

	if (logDate < 172800)
		return NULL;

//	hdrInfo.IndexType = theType;

	hdrInfo.LogDate = logDate;

	hdrInfo.Event_Id = EvIndexPtr[0];
	i = 1;
	memcpy(hdrInfo.MSN, &EvIndexPtr[i], 11);
	i += 11;
	if (EvIndexPtr[i-1])
		return NULL;
	memcpy(hdrInfo.ISN, &EvIndexPtr[i], 9);
	i += 9;
	if (EvIndexPtr[i-1])
		return NULL;
	memcpy(hdrInfo.OSN, &EvIndexPtr[i], 9);
	i += 9;
	if (EvIndexPtr[i-1])
		return NULL;
	memcpy(hdrInfo.Src_Dev, &EvIndexPtr[i], 9);
	i += 9;
	if (EvIndexPtr[i-1])
		return NULL;
	memcpy(hdrInfo.Dest_Dev, &EvIndexPtr[i], 9);
	i += 9;
	if (EvIndexPtr[i-1])
		return NULL;
	memcpy(hdrInfo.ORI_Id, &EvIndexPtr[i], 10);
	i += 10;
	if (EvIndexPtr[i-1])
		return NULL;
	if (theType == 2)
	{
		memset(hdrInfo.User_Id, 0, LEMSMAXUIDLEN);
		memcpy(hdrInfo.User_Id, &EvIndexPtr[i], 11);
		i += 11;
	}
	else
	{
		memcpy(hdrInfo.User_Id, &EvIndexPtr[i], LEMSMAXUIDLEN);
		i += LEMSMAXUIDLEN;
	}
	if (EvIndexPtr[i-1])
		return NULL;
	memcpy(hdrInfo.InLine, &EvIndexPtr[i], 9);
	i += 9;
	if (EvIndexPtr[i-1])
		return NULL;
	memcpy(hdrInfo.OutLine, &EvIndexPtr[i], 9);
	i += 9;
	if (EvIndexPtr[i-1])
		return NULL;
	memcpy(hdrInfo.InMke, &EvIndexPtr[i], 9);
	i += 9;
	if (EvIndexPtr[i-1])
		return NULL;
	memcpy(hdrInfo.OutMke, &EvIndexPtr[i], 9);
	i += 9;
	if (EvIndexPtr[i-1])
		return NULL;

	i++; /* Adjust for Binary Alignment */

	u_ptr = (u_long *) &EvIndexPtr[i];
	hdrInfo.MsgLen = (u_ptr[0]);
	hdrInfo.LogLen = (u_ptr[1]);
	i += (2 * sizeof(u_long));

	/* some arbitrary max length of 4,000,000 bytes - sanity check */

	if (hdrInfo.LogLen > 4000000 || hdrInfo.MsgLen > 4000000 ||
		hdrInfo.LogLen < 0 || hdrInfo.MsgLen < 0)
		return NULL;

	if (hdrInfo.IndexType >= 4)
	{
		u_ptr = (u_long *) &EvIndexPtr[i];
		hdrInfo.IN_DTIME.tv_sec = u_ptr[0];
		hdrInfo.IN_DTIME.tv_usec = u_ptr[1];
		hdrInfo.OUT_DTIME.tv_sec = u_ptr[2];
		hdrInfo.OUT_DTIME.tv_usec = u_ptr[3];
		hdrInfo.LogTime.tv_sec = u_ptr[4];
		hdrInfo.LogTime.tv_usec = u_ptr[5];
		i += (6 * sizeof(u_long));
	}
	else
	{
		u_ptr = (u_long *) &EvIndexPtr[i];
		hdrInfo.IN_DTIME.tv_sec = u_ptr[0];
		hdrInfo.IN_DTIME.tv_usec = 0;
		hdrInfo.OUT_DTIME.tv_sec =u_ptr[1];
		hdrInfo.OUT_DTIME.tv_usec = 0;
		hdrInfo.LogTime.tv_sec = u_ptr[2];
		hdrInfo.LogTime.tv_usec = 0;
		i += (3 * sizeof(u_long));
	}

	/* time_t of 3600 seconds = 1 hour , for yessterday's log file*/
	/* time_t of 90000 seconds = 25 hours, for tomorrow's log file */
	/* time_t became 157,000,000 sometime in 1976 */

	if (hdrInfo.LogTime.tv_sec < 157000000  ||
		hdrInfo.LogTime.tv_sec < (logDate - 4000) ||
		hdrInfo.LogTime.tv_sec > (logDate + 90000))
	{
		/* Bad LogTime in Index */
		return NULL;
	}

	/* Allow input messages to remain in q for 5 days (-432000 seconds) */
	/* Allow input messages to remain in q for 30 days (-2592000 seconds) */

	if (hdrInfo.IN_DTIME.tv_sec > 172800 && (
//		hdrInfo.IN_DTIME.tv_sec < (logDate - 432000) ||
		hdrInfo.IN_DTIME.tv_sec < (logDate - 2592000) ||
		hdrInfo.IN_DTIME.tv_sec > (logDate + 90000)))
	{
		/* Bad IN_DTIME in Index */
		return NULL;
	}

	/* Allow output message logging across midnight to yestarday-tomorrow */

	if (hdrInfo.OUT_DTIME.tv_sec > 172800 && (
		hdrInfo.OUT_DTIME.tv_sec < (logDate - 4000) ||
		hdrInfo.OUT_DTIME.tv_sec > (logDate + 90000) ||
		hdrInfo.OUT_DTIME.tv_sec < (hdrInfo.IN_DTIME.tv_sec - 4000)))
	{
		/* Bad OUT_DTIME in Index */
		return NULL;
	}

	/* Advance past Type(5) Confidential + Filler */
	if (hdrInfo.IndexType >= 5)
		i += 4;

	memcpy(hdrInfo.MsgText, &EvIndexPtr[i], 4);  /* First four characters of Msg */

	if (!(rtnInfo = malloc(sizeof(EventsIndexType))))
		return NULL;
	memcpy(rtnInfo, &hdrInfo, sizeof(EventsIndexType));
	return rtnInfo;	
}

/* ================================================================
 * TestEventsIndexType2
 *
 * Success: Returns a new structure of EventsIndexType.
 *		Deallocate EventsIndexType object via FreeEventsIndexType().
 * Failure: Returns NULL
 */

EventsIndexType *
TestEventsIndexType2 (time_t logDate, const unsigned char * theIndex)
{
	return TestEventsIndexTypeN (2, logDate, theIndex);
}

/* ================================================================
 * TestEventsIndexType3
 *
 * Success: Returns a new structure of EventsIndexType.
 *		Deallocate EventsIndexType object via FreeEventsIndexType().
 * Failure: Returns NULL
 */

EventsIndexType *
TestEventsIndexType3 (time_t logDate, const unsigned char * theIndex)
{
	return TestEventsIndexTypeN (3, logDate, theIndex);
}

/* ================================================================
 * TestEventsIndexType4
 *
 * Success: Returns a new structure of EventsIndexType.
 *		Deallocate EventsIndexType object via FreeEventsIndexType().
 * Failure: Returns NULL
 */

EventsIndexType *
TestEventsIndexType4 (time_t logDate, const unsigned char * theIndex)
{
	return TestEventsIndexTypeN (4, logDate, theIndex);
}


/* ================================================================
 * TestEventsIndexType5
 *
 * Success: Returns a new structure of EventsIndexType.
 *		Deallocate EventsIndexType object via FreeEventsIndexType().
 * Failure: Returns NULL
 */

EventsIndexType *
TestEventsIndexType5 (time_t logDate, const unsigned char * theIndex)
{
	return TestEventsIndexTypeN (5, logDate, theIndex);
}

/* ================================================================
 * FreeEventsIndexType
 *
 * Deallocate the structure returned from TestEventsIndexTypeN().
 */

int FreeEventsIndexType (EventsIndexType * theIndex)
{
	if (theIndex)
		free(theIndex);
	return 0;
}

/* ================================================================
 * TestLemsLogEOF
 */

int TestLemsLogEOF (LEMSLOGHANDLE theHandle)
{
	if (theHandle && theHandle->curMsgHdrOffset &&
		(theHandle->curMsgHdrOffset == theHandle->theEofLocn))
		return 1;
	return 0;
}

/* ================================================================
 * TestLemsLogErr
 *
 * Returns NonZero error code if a log file error was encountered.
 * Returns Zero if no error is identified.
 *		1 = memory allocation error - or other system error
 *		2 = application error
 *		3 = logfile overwrite error
 *		4 = event header upgrade-downgrade type change
 */

int TestLemsLogErr (LEMSLOGHANDLE theHandle)
{
	if (theHandle)
		return theHandle->theErrFlag;
	return 0;
}


/* ================================================================
 * SetLemsLogErr
 */

void SetLemsLogErr (LEMSLOGHANDLE theHandle, int theError)
{
	if (theHandle)
		theHandle->theErrFlag = theError;
}


/* ================================================================
 * GetLemsLogErr
 *
 * Same As TestLemsLogErr()
 * -- THIS WILL PROBABLY BE DEPRECATED --
 */

int GetLemsLogErr (LEMSLOGHANDLE theHandle)
{
	if (theHandle)
		return theHandle->theErrFlag;
	return 0;
}


/* ================================================================
 * ClearLemsLogErr
 *
 * Clear the error flag associated with a LEMSLOGHANDLE.
 */

void ClearLemsLogErr (LEMSLOGHANDLE theHandle)
{
	if (theHandle)
		theHandle->theErrFlag = 0;
}
