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
 * Rev  0.03     Aug 24 2011            Hathaway
 *      Fix Definition of EventsIndexType5 for binary I/O
 *
 * Rev  0.02     Nov 08 2010            Hathaway
 *      Add EventLog Type 5 Support - LEMS 3.4.0
 *
 * Rev  0.01     Nov 12 2009            Hathaway
 *      Change time_t to unsigned int, accommodating a development platform upgrade.
 *      Studio .NET 2005 (VC8) changes typedef time_t from 32-bit to 64-bit quantity.
 */

#ifndef OR_LEMS_EVLOG_HDR_H
#define OR_LEMS_EVLOG_HDR_H

/* Define time_t, struct tm, struct timeval, struct timespec */
#include <time.h>

#ifndef LEMSMAXUIDLEN
 /* A 20 character user name + null */
 #define LEMSMAXUIDLEN	21
#endif

typedef enum lems_ev_type_code {
	LOGON 			=	'n',
	LOGOFF 			=	'f',
	BADLOGON 		=	'B',
	INPUTCOMMAND 	=	'C',
	INPUTMESSAGE 	=	'I',
	ROUTEDONE 		=	'R',
	OUTPUTACK 		=	'A',
	OUTPUTNAK 		=	'N',
	OUTPUTMESSAGE 	=	'O',
	QUEUEADD 		=	'Q',
	QUEUEDELIVERY 	=	'D',
	SUPERTRANS 		=	'S',
	ALTROUTE 		=	'a',
	PURGE 			=	'P',
	INTSTATCHG 		=	'i',
	GEN_ERROR		=	'E',
	LEMSINIT		=	'L',
	LEMSCLEA		=	'Z'
} LemsEvType;

#define OUTPUTEVENT(x)	((x==OUTPUTACK) || (x==OUTPUTNAK) || (x==OUTPUTMESSAGE) || (x==PURGE))
#define INPUTEVENT(x)	((x==INPUTCOMMAND) || (x==INPUTMESSAGE) || (x==BADLOGON) || (x==LOGON) || (x==LOGOFF))

#define LEMS_EV_TYPE_CODES	"nfBCIRANOQDSaPiELZ"

/*
	LEMS 2000 Message Event Log Files, Header Structure
 */
// physical - file header layout
typedef struct lems_ev_log_hdr
{
	char	Lems2000[12];		// Message Switch Product Name: "LEMS 2000"
	char	Version[7];			// Message Switch Software Version:
								//   i.e 2.3, 3.0.2, 3.2.2, 3.2.4, 3.3.0, 3.4.1
	char	FileName[16];		// Log File Name: "/EVnnn_n.log"
	char	CreationDate[19];	// Log File Create Date Time: "CCYY-MM-DD HH:MM"
								//   no seconds.
	char	CreatingPgm[11];	// Program Name without extension, with SID appended,
								//   and truncated to 11 characters "ioclient148"
	char	ProgramVersion[7];	// null characters.
} LemsEvLogHdrType;


#define MAX_LEMS_EVTYPE_HEAD		sizeof(EventsIndexType)

/*
	General structure for LEMS Events File Header Information.
 */
// virtual - runtime info
typedef struct 
{
	int				IndexType;		// 0=Unknown, Otherwise 2,3,4,5
	time_t			LogDate;		// Date of LogFile Creation
	LemsEvType		Event_Id;
	char 			MSN[11];
	char			ISN[9];
	char		 	OSN[9];
	char 			Src_Dev[9];
	char 			Dest_Dev[9];
	char 			ORI_Id[10];
	char 			User_Id[LEMSMAXUIDLEN];
	char 			InLine[9];
	char 			OutLine[9];
	char 			InMke[9];
	char 			OutMke[9];
	size_t			MsgLen;
	size_t			LogLen;
	struct timeval 	IN_DTIME;
	struct timeval	OUT_DTIME;
	struct timeval	LogTime;
	char			Confidential;	// new in Type 5
	char			MsgText[4];
} EventsIndexType;


/*
	Structure for Events Index File (Since LEMS 3.4 and later)
*/
// physical index layout
typedef struct 
{
	unsigned char	IndexVer[5];	// "~|5|~"  New in LEMS 3.4
	unsigned char	Event_Id;		// LemsEvType Value
	char 			MSN[11];
	char			ISN[9];
	char		 	OSN[9];
	char 			Src_Dev[9];
	char 			Dest_Dev[9];
	char 			ORI_Id[10];
	char 			User_Id[LEMSMAXUIDLEN];
	char 			InLine[9];
	char 			OutLine[9];
	char 			InMke[9];
	char 			OutMke[9];
	unsigned int	MsgLen;
	unsigned int	LogLen;
	struct timeval 	IN_DTIME;
	struct timeval	OUT_DTIME;
	struct timeval	LogTime;
	char			Confidential;	// new in LEMS 3.4
	char			Filler[3];
	char			MsgText[4];
} EventsIndexType5;

/*
	Structure for Events Index File (Since LEMS 3.2 through LEMS 3.3)
*/
// physical index layout
typedef struct 
{
	unsigned char	Event_Id;		// LemsEvType Value
	char 			MSN[11];
	char			ISN[9];
	char		 	OSN[9];
	char 			Src_Dev[9];
	char 			Dest_Dev[9];
	char 			ORI_Id[10];
	char 			User_Id[LEMSMAXUIDLEN];
	char 			InLine[9];
	char 			OutLine[9];
	char 			InMke[9];
	char 			OutMke[9];
	unsigned int	MsgLen;
	unsigned int	LogLen;
	struct timeval 	IN_DTIME;
	struct timeval	OUT_DTIME;
	struct timeval	LogTime;
	char			MsgText[4];
} EventsIndexType4;

/*
	Structure for Events Index File (Since LEMS 2.3 until LEMS 3.2)
*/
// physical index layout
typedef struct 
{
	unsigned char	Event_Id;		// LemsEvType Value
	char 			MSN[11];
	char			ISN[9];
	char		 	OSN[9];
	char 			Src_Dev[9];
	char 			Dest_Dev[9];
	char 			ORI_Id[10];
	char 			User_Id[LEMSMAXUIDLEN];
	char 			InLine[9];
	char 			OutLine[9];
	char 			InMke[9];
	char 			OutMke[9];
	unsigned int	MsgLen;
	unsigned int	LogLen;
	unsigned int 	IN_DTIME;
	unsigned int	OUT_DTIME;
	unsigned int	LogTime;
	char			MsgText[4];
} EventsIndexType3;

/*
	Structure for Events Index File (Before LEMS 2.3)
*/
// physical index layout
typedef struct 
{
	unsigned char	Event_Id;		// LemsEvType Value
	char 			MSN[11];
	char			ISN[9];
	char		 	OSN[9];
	char 			Src_Dev[9];
	char 			Dest_Dev[9];
	char 			ORI_Id[10];
	char 			User_Id[11];	// Short Username (10 chars)
	char 			InLine[9];
	char 			OutLine[9];
	char 			InMke[9];
	char 			OutMke[9];
	unsigned int	MsgLen;
	unsigned int	LogLen;
	unsigned int 	IN_DTIME;
	unsigned int	OUT_DTIME;
	unsigned int	LogTime;
	char			MsgText[4];
} EventsIndexType2;

// virtual - runtime info
typedef struct lems_log_file_header {
	char			appName[16];
	char			appVersion[10];
	char			logFileName[18];
	char			logCrDate[22];
	time_t			logCrTime;
	char			logWriter[12];
} LemsLogFileHeaderStruct;


typedef struct lems_log_msg_header {
	int			msgHdrType;
	off_t			msgHdrOffset;
	size_t			msgHdrLength;
	EventsIndexType		theIndex;
}LemsLogMsgHeaderStruct;


typedef struct lems_log_msg_data {
	size_t			msgLogLen;
	size_t			msgLength;
	unsigned char		* msgBuffer;
}LemsLogMsgDataStruct;


typedef struct lems_log_message {
	LemsLogMsgHeaderStruct	* msgHdr;		// Validated Header
	LemsLogMsgDataStruct	* msgData;		// Message Data
}LemsLogMessageStruct;


typedef struct lems_log_file_handle {
	int				theFD;				// file descriptor
	char *			theFullPath;		// full logfile path
	int				theErrFlag;			// error indicator
	off_t			theEofLocn;			// size of logfile
	off_t			prvMsgHdrOffset;	// prev message off_t
	off_t			curMsgHdrOffset;	// current message off_t
	off_t			nxtMsgHdrOffset;	// next message off_t
	int				HdrTypeFirst;		// validate index first
	int				HdrTypeSecond;		// validate index next
	LemsLogFileHeaderStruct * 	theFileHeader;		// one per log file
	LemsLogMessageStruct	* 	theCurrMessage;		// info on current msg
/* We do our own buffering for IO - */
/* This buffer currently used for EventsIndexType Validation */
	size_t					bufSize;			// io mgmt
	unsigned char			* theBuffer;		// io mgmt
	unsigned char			* bufPtr;			// io mgmt
	unsigned char			* bufPtrTail;		// io mgmt
}LEMSLOG_HANDLE_STRUCT, * LEMSLOGHANDLE;

/*
 * Standard I/O Routines using Microsoft crt (_POSIX)
 * NOTE: Microsoft crt limits files to 2 GB in size (_off_t) issue.
 */

/* ================================================================
 * OpenLemsLogFile
 *
 * Return a pointer to the LEMSLOG_HANDLE_STRUCT if the LEMS Event Log
 * File was successfully opened and validated.
 */

LEMSLOGHANDLE OpenLemsLogFile(const char * filename);


/* ================================================================
 * CloseLemsLogFile
 */

int CloseLemsLogFile (LEMSLOGHANDLE theHandle);

/* ================================================================
 * LemsEvTypeName
 *
 * Return the character string name for the EventsIndexType (Event_Id);
 * The returned string has static allocation.
 */

char * LemsEvTypeName(LemsEvType ev);

/* ================================================================
 * GetLemsLogCreateTime
 *
 * Return a time_t value indicating when the log file was created.
 */

time_t GetLemsLogCreateTime (LEMSLOGHANDLE theHandle);

/* ================================================================
 * GetLemsLogFileHeader
 *
 * Makes a copy of the LemsLogFileHeaderStuct associated with theHandle.
 * You should deallocate the copy by calling FreeLemsLogFileHeader().
 */

LemsLogFileHeaderStruct *
GetLemsLogFileHeader (LEMSLOGHANDLE theHandle);

/* ================================================================
 * CloneLemsLogFileHeader
 *
 * Makes a copy of the LemsLogFileHeaderStruct and returns a pointer
 * to the new copy.  A NULL pointer is returned on memory allocation error.
 */

LemsLogFileHeaderStruct *
CloneLemsLogFileHeader (LemsLogFileHeaderStruct * theHeader);

/* ================================================================
 * GetLemsLogMsgHeader
 *
 * -- NOT CURRENTLY IMPLEMENTED --
 * Makes a copy of the LemsLogMsgHeaderStruct associated with theHandle
 * and the current message.  You should deallocate the copy by calling
 * FreeLemsLogMsgHeader().
 */

LemsLogMsgHeaderStruct *
GetLemsLogMsgHeader (LEMSLOGHANDLE theHandle);

/* ================================================================
 * GetLemsLogMsgData
 *
 * -- NOT CURRENTLY IMPLEMENTED --
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
GetLemsLogMessage (LEMSLOGHANDLE theHandle);

/* ================================================================
 * CloneLemsLogMessage
 *
 * Make a copy of the message and return a pointer to the copy.
 * A NULL pointer is returned on error.  This allows a message to be
 * copied instead of being read again from a file handle.
 */

LemsLogMessageStruct *
CloneLemsLogMessage (LemsLogMessageStruct * theMessage);

/* ================================================================
 * FreeLemsLogFileHeader
 */

int FreeLemsLogFileHeader (LemsLogFileHeaderStruct * theHeader);

/* ================================================================
 * FreeLemsLogMsgHeader
 */

int FreeLemsLogMsgHeader (LemsLogMsgHeaderStruct * theHeader);

/* ================================================================
 * FreeLemsLogMsgData
 */

int FreeLemsLogMsgData (LemsLogMsgDataStruct * theData);

/* ================================================================
 * FreeLemsLogMessage
 */

int FreeLemsLogMessage (LemsLogMessageStruct * theMessage);

/* ================================================================
 * SetLemsLogMessageIx
 *
 * Success: returns the specified file offset value (theOffset).
 * Failure: returns zero, the internal handle is unchanged.
 */

off_t SetLemsLogMessageIx (LEMSLOGHANDLE theHandle, off_t theOffset);

/* ================================================================
 * GetLemsLogMessageIx
 */

off_t GetLemsLogMessageIx (LEMSLOGHANDLE theHandle);

/* ================================================================
 * GetNextLemsLogMessageIx
 *
 * This set when a successful message is read. It is cleared
 * when the message index offset is explicitly set.
 */

off_t GetNextLemsLogMessageIx (LEMSLOGHANDLE theHandle);

/* ================================================================
 * GetNextLemsLogMsgHeader
 *
 * -- NOT CURRENTLY IMPLEMENTED --
 */

LemsLogMsgHeaderStruct *
GetNextLemsLogMsgHeader (LEMSLOGHANDLE theHandle);

/* ================================================================
 * GetNextLemsLogMessage
 *
 * This command requires a previous successful GetLemsLogMessage().
 * The returned message must be deallocated via FreeLemsLogMessage().
 */

LemsLogMessageStruct *
GetNextLemsLogMessage (LEMSLOGHANDLE theHandle);

/* ================================================================
 * TestEventsIndexTypeN
 *
 * Success: Returns a new structure of EventsIndexType.
 *		Deallocate EventsIndexType object via FreeEventsIndexType().
 * Failure: Returns NULL
 */

EventsIndexType *
TestEventsIndexTypeN (int theType, time_t logDate, const unsigned char * theIndex);

/* ================================================================
 * TestEventsIndexType2
 *
 * Not currently used, calls TestEventsIndexTypeN(2, ...)
 */

EventsIndexType *
TestEventsIndexType2 (time_t logDate, const unsigned char * theIndex);

/* ================================================================
 * TestEventsIndexType3
 *
 * Not currently used, calls TestEventsIndexTypeN(3, ...)
 */

EventsIndexType *
TestEventsIndexType3 (time_t logDate, const unsigned char * theIndex);

/* ================================================================
 * TestEventsIndexType4
 *
 * Not currently used, calls TestEventsIndexTypeN(4, ...)
 */

EventsIndexType *
TestEventsIndexType4 (time_t logDate, const unsigned char * theIndex);

/* ================================================================
 * FreeEventsIndexType
 *
 * Deallocate the structure returned from TestEventsIndexTypeN().
 */

int FreeEventsIndexType (EventsIndexType *);

/* ================================================================
 * TestLemsLogEof
 *
 * Returns 1 if theHandle is at end of file. Otherwise returns zero.
 */

int TestLemsLogEof (LEMSLOGHANDLE theHandle);

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
 
int TestLemsLogErr (LEMSLOGHANDLE theHandle);

/* ================================================================
 * SetLemsLogErr
 *
 * -- Not Currently Used --
 * Set an integer error code for use with TestLemsLogErr()
 */

void SetLemsLogErr (LEMSLOGHANDLE theHandle, int theError);

/* ================================================================
 * GetLemsLogErr
 *
 * Same As TestLemsLogErr()
 * -- THIS WILL PROBABLY BE DEPRECATED --
 */

int GetLemsLogErr (LEMSLOGHANDLE theHandle);

/* ================================================================
 * ClearLemsLogErr
 *
 * Clear the error flag associated with a LEMSLOGHANDLE.
 */

void ClearLemsLogErr (LEMSLOGHANDLE theHandle);

#endif /* OR_LEMS_EVLOG_HDR_H */

