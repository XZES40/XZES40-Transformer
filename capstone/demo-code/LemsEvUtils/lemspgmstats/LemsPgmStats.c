
/*
 * FILE:  LemsPgmStats.c
 *
 */
/*
 * Rev 0.1          Aug 25 2011              Hathaway
 *    Initial Edit - Custom Statistics.
 */

/*
 * This is the Main() program to report custom transaction statistics from LEMS eventlog files.
 *
 * USAGE: theProgram mountPoint eventLogDir startDTime endDTime outfile.csv
 *
 *	theProgram	= name of the program
 *	mountPoint	= drive: or mountPoint
 *  eventLogDir = log directory relative to the mount point
 *	startDTime	= "ccyy-mm-dd 00:00:00" for the starting date-time
 *	endDTime	= "ccyy-mm-dd 23:59:59" for the ending date-time
 *	outfile.csv	= full-path to output spreadsheet (.csv) file
 *	
 */

//
// Template for LEMS-2000 and LEMS/JX Event Log Processing -- from msgwalker.c
//

#include "platform.h"

#include <time.h>
#include <stdio.h>

#include "btcomp.h"
#include "keylist.h"
#include "lemslog.h"
#include "msevlog.h"
#include "LemsPgmStats.h"
//#include "debuglog.h"

#ifdef __cplusplus
extern 'C' {
#endif

int usage_help(FILE * OutHandle, int argc, const char * argv[])
{
	fprintf(OutHandle,"\n\nUSAGE: %s  mountPoint  eventlogdir  startDateTime  endDateTime  outfile\n\n"
	"     mountPoint    = device or lemshome\n"
	"     eventlogdir   = path to eventlogs directory relative to mountPoint\n"
	"     startDateTime = \"CCYY-MM-DD 00:00:00\"\n"
	"     endDateTime   = \"CCYY-MM-DD 23:59:59\"\n"
	"     outfile        = full-path to spreadsheet output (.csv)\n\n", argv[0]);
	return 0;
}


main(int argc, char * argv[])
{
	int    prsErr	= 0;

	struct tm	startDateTime;
	struct tm	endDateTime;

	char * startDTime	= "ccyy-mm-dd 00:00:00";
	char * endDTime		= "ccyy-mm-dd 23:59:59";
	char * evMountPt	= "X:";
	char * eventLogs	= "";
	char * outfile		= "C:\\lems2k\\xfer\\LemsPgmStats.csv";

	char * evFileName = NULL;
	char * evDirName = NULL;

	FILE * CsvFile = NULL;

	KeyListEntry	* dirList = NULL;
	KeyListEntry	* fileList = NULL;

	KeyListEntry	* DListPtr = NULL;
	KeyListEntry	* FListPtr = NULL;

	if (argc != 6)
	{
		usage_help(stderr, argc, argv);
		exit(1);
	}

	evMountPt = strdup(argv[1]);
	eventLogs = strdup(argv[2]);
	startDTime = strdup(argv[3]);
	endDTime = strdup(argv[4]);
	outfile = strdup(argv[5]);

	if (!(CsvFile = OpenFileCsv(outfile)))
	{
		fprintf(stderr, "\nERROR - Cannot Open Output File: %s\n", outfile);
		usage_help(stderr, argc, argv);
		exit(1);
	}

	/* END OF INITIALIZE GLOBAL VARIABLES FROM MAIN() */

	memcpy((void *) &startDateTime, (void *) TmParseDate(startDTime), sizeof(struct tm));
	memcpy((void *) &endDateTime, (void *) TmParseDate(endDTime), sizeof(struct tm));

	dirList = MakeEvDirList(&startDateTime, &endDateTime, evMountPt, eventLogs);

	for (DListPtr = dirList; DListPtr; DListPtr=GetKeyEntryNext(DListPtr))
	{
		evDirName = GetKeyEntryName(DListPtr);	// returns pointer to data cached in keylist

		// Show to user the directory we are processing
		printf("STATS-DIR: %s\n", evDirName);

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
				LemsLogMessageStruct * theMessage = NULL;

				for (theMessage = GetLemsLogMessage(logFileHandle); theMessage;
					theMessage = GetNextLemsLogMessage (logFileHandle))
				{
					// if message timestamp not in date-time range
					// then free the message and "continue" for next message

					// Currently using full dated file regardless of internal timestamp

					int err = 0;
					err += ChQ2Leds(theMessage);
					err += ChU2Leds(theMessage);
					err += ChQ2Ncic(theMessage);
					err += ChU2Ncic(theMessage);
					err += ChQU2Nics(theMessage);
					err += ChQR2Dmv(theMessage);
					err += ChQP2Dmv(theMessage);
					err += ChQ2Nlets(theMessage);
					err += ChQ2ommp(theMessage);
					err += ChQ2ormh(theMessage);
					err += ChQ2Mugs(theMessage);
					err += ChQ2Ptld(theMessage);
					err += ChQ2Cord(theMessage);
					err += ChQ2Clas(theMessage);
					err += ChQ2SunRain(theMessage);
					err += Ch2DmvHelp(theMessage);
					err += Ch4AM(theMessage);
					err += Ch4Weather(theMessage);
					err += Ch4HSec(theMessage);
					err += OtherSums(theMessage);
					if (err)
					{
						CloseFileCsv(CsvFile);
						fprintf(stderr,"\nERROR: SEVERE PROBLEM\n");
						exit(1);
					}

					// GET RID OF THE MESSAGE COPY
					FreeLemsLogMessage(theMessage);
				}
				CloseLemsLogFile(logFileHandle);
			}
		}
		FreeKeyList(&fileList);
	}

	FreeKeyList(&dirList);

	WriteCsvReport(CsvFile);
	CloseFileCsv(CsvFile);

	exit(0);
}

#ifdef __cplusplus
}
#endif


