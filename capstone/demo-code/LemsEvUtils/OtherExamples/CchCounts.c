
//
// Template for LEMS-2000 and LEMS/JX Event Log Processing
//
// Modified for Oregon CCH Statistics - CCH inquiries to CrimeVue from Oregon Agencies
//
//	printf("ERROR - USAGE:\r\n"
//	"Program LogDir outfile Start-DayTime End-DayTime [ori ...]\r\n"
//      "If no [ori ...] is issued, all OR... Oregon ORIs are assumed.\r\n");
//      
//	exit(1);
//
// -- Items to do for production:  
// -- error recognition with cleanup and call-tree notification.
//

#include "platform.h"

#include <errno.h>
#include <time.h>
#include <stdio.h>

#include "keylist.h"
#include "msevlog.h"
#include "lemslog.h"
#include "fmtlib.h"

#include "btcomp.h"

#ifdef __cplusplus
extern 'C' {
#endif


//struct tm	startDateTime;
//struct tm	endDateTime;

// Items for Message Selection

int All_OR_ori = 1;				// Default (1) = ignore oriList
KeyListEntry	* oriList = NULL;		// the ORIs to find

const char * hour_table[25] = {
		"00:00", "01:00", "02:00", "03:00",
		"04:00", "05:00", "06:00", "07:00",
		"08:00", "09:00", "10:00", "11:00",
		"12:00", "13:00", "14:00", "15:00",
		"16:00", "17:00", "18:00", "19:00",
		"20:00", "21:00", "22:00", "23:00", NULL};

const char * mke_table[] = {
		"QH", "QWH", "QWHD", "QHD", "RR", "FQ", "IQ", NULL};

#define mke_table_length 7

const char * find_mke_in_table (const char * theMke)
{
  int i;

  for (i=0; i < mke_table_length; i++)
  {
    if (!strcmp(mke_table[i], theMke))
       break;
  }
  return mke_table[i];
}

const char * find_qualifying_ori (const char * theOri)
{
	KeyListEntry * KPtr1;
	KeyListEntry * KPtr2;
	if (!theOri || (strlen(theOri) != 9))
		return NULL;
	if (All_OR_ori)
	{
		if (!strncmp(theOri, "OR", 2))
			return theOri;
		else
			return NULL;
	}
	for (KPtr1 = oriList; KPtr1; KPtr1 = KPtr2)
	{
		KPtr2 = KPtr1->next;
		if (!strcmp(theOri, KPtr1->k_name))
			break;
	}
	if (!KPtr1)
		return NULL;

	return KPtr1->k_name;
}

// Items for Statistics Reporting

const char * DayName[] = {"Sunday", "Monday", "Tuesday", "Wednesday",
                "Thursday", "Friday", "Saturday", NULL};

KeyListEntry	* statsList = NULL;		//  Master List of Counters

void display_usage_and_exit()
{
  printf("ERROR - USAGE:\r\n"
     "Program LEMS_LogDir REPORT_OutFile Start-DayTime End-DayTime [ori ...]\r\n"
     "If no [ori ...] is issued, all OR... Oregon ORIs are assumed.\r\n");
  exit(1);
}

char * LogDirPath;
char * OutFileName;
struct tm start_tm;
struct tm end_tm;

int initialize_args (int argc, char * argv[])
{
	/* program   logpath   outfile   start-DTime   end-DTime   [ori ...] */

	char * startDTime;
	char * endDTime;
	int i;
	KeyListEntry * KP_ori;
	char tmpbuf[100] = "";

	if (argc < 5)
		display_usage_and_exit();
	LogDirPath = strdup(argv[1]);
	OutFileName = strdup(argv[2]);

	startDTime = argv[3];
	endDTime = argv[4];

	memcpy((void *) &start_tm, (void *) TmParseDate(startDTime), sizeof(struct tm));
	memcpy((void *) &end_tm, (void *) TmParseDate(endDTime), sizeof(struct tm));

	/* force to use full-day limits of startDTime and endDTime */

	start_tm.tm_sec = 0;
	start_tm.tm_min = 0;
	start_tm.tm_hour = 0;
	end_tm.tm_sec = 59;
	end_tm.tm_min = 59;
	end_tm.tm_hour = 23;

	if (argc > 5)
	{
		All_OR_ori = 0;

		for (i=5; i < argc; i++)
		{
			if (i == 5)
				KP_ori = oriList = NewKeyListEntry();
			else
			{
				KP_ori->next = NewKeyListEntry();
				KP_ori = KP_ori->next;
			}
			SetKeyEntryName(KP_ori, argv[i]);
		}
	}

	statsList = NewKeyListEntry();

	sprintf(tmpbuf, "%04d-%02d-%02d -- %04d-%02d-%02d",
		start_tm.tm_year + 1900,
		start_tm.tm_mon + 1,
		start_tm.tm_mday,
		end_tm.tm_year + 1900,
		end_tm.tm_mon + 1,
		end_tm.tm_mday);
	SetKeyEntryName(statsList, tmpbuf);

	return 0;
}


/***********************
** STATISTICS COLLECTION KEYLIST
**
**    TOTAL ------ SORI ------ SORI ------ SORI ------ SORI ------ SORI ------
**    | k_name = "date-range"    | k_name = "Agency_ORI"
**    | k_id = total-cch-count   | k_id = ori-cch-count
**    | k_data =                 | k_data =
**    | k_list                   | k_list
**      \                          \
**       Date ---- Date ----        Date ---- Date ---- Date ---- Date ----            
**       | k_name = "weekday"       | k_name = "weekday"
**       | k_id = daily_cch_count   | k_id = ori-daily-count
**       | k_data = "ccyy-mm-dd"    | k_data = "ccyy-mm-dd"
**       | k_list                   | k_list
**         \                          \
**          Hour ---- Hour ----        Hour ---- Hour ---- Hour ---- Hour ----
**          | k_name ="00:00" hour     | k_name = "00:00" hour
**          | k_id = hourly_cch_count  | k_id = ori-hourly-count
**          | k_data =                 | k_data = 
**          | k_list                   | k_list
**            \                          \
**             MKE ---- MKE ----          MKE ---- MKE ---- MKE ---- MKE ----
**             | k_name = "MKE"           | k_name = "
**             | k_id = hourly_mke_count  | k_id = hourly_mke_count
**
**
** MKE-LIST
**   MKE ---- MKE ---- MKE ---- MKE  (sorted by MKE)
**
****/

/************************
** STATISTICS LIST SORTING
**
** TOTAL ---- SORI ---- SORI --- SORI --- (sort by k_name "Agency_ORI")
**
** Date ---- Date ---- Date ---- Date --- (sort by k_data "ccyy-mm-dd")
**
** Hour ---- Hour ---- Hour ---- Hour --- (sort by k_name "00:00" the hour)
**
** MKE ---- MKE ---- MKE ---- MKE ---- (sort by k_name "MKE" the message key)
**
****/

int compare_k_data(const KeyListEntry * nodeA, const KeyListEntry * nodeB)
{
	return strcmp((char *) nodeA->k_data, (char *) nodeB->k_data);
}

#define KeyListSortDate(theList) KeyListSort(theList, &compare_k_data) 


/************************
** REPORTS (.data) tab separated values
**
** ----------------------------------------------------------------------------------------------
** LEDS CCH SUMMARY REPORT FOR (date-range)
**
** TOTALS       Total-cch-count   ccyy-mm-dd  daily-count  ccyy-mm-dd  daily-count ...
** Agency-ORI   Ori-cch-count     ccyy-mm-dd  daily-count  ccyy-mm-dd  daily-count ...
** Agency-ORI   Ori-cch-count     ccyy-mm-dd  daily-count  ccyy-mm-dd  daily-count ...
**   - - -         - - -            - - -       - - -         - - -       - - -
** Agency-ORI   Ori-cch-count     ccyy-mm-dd  daily-count  ccyy-mm-dd  daily-count ...
**
**
** ----------------------------------------------------------------------------------------------
** LEDS CCH DETAIL REPORT FOR (date-range)
**
** Agency-ORI   Sunday  ccyy-mm-dd  00:00  hour-count  01:00  hour-count  02:00  hour-count ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**              Monday  ccyy-mm-dd  00:00  hour-count  01:00  hour-count  02:00  hour-count ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**              - - -    - - -      - - -    - - -     - - -    - - -     - - -    - - -
** Agency-ORI   Sunday  ccyy-mm-dd  00:00  hour-count  01:00  hour-count  02:00  hour-count ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**              Monday  ccyy-mm-dd  00:00  hour-count  01:00  hour-count  02:00  hour-count ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
**                      MKE-Name    00:00  mke-count   01:00  mke-count   02:00  mke-count  ...
** - - -        - - -    - - -      - - -    - - -     - - -    - - -     - - -    - - -
**
** ----------------------------------------------------------------------------------------------
**/


int countMessage(LemsLogMessageStruct * theMessage)
{
	int rtn = 0;
	KeyListEntry * KTtotal;
	KeyListEntry * KTdate;
	KeyListEntry * KThour;
	KeyListEntry * KTmke;

	KeyListEntry * KPsori;
	KeyListEntry * KPdate;
	KeyListEntry * KPhour;
	KeyListEntry * KPmke;

	const char * SORI;
	const char * MKE;
	const char * Weekday;
	char StatsDate[12];
	char StatsHour[6];

	struct tm	msg_tm;

	/* Get the message LogTime components */

	memcpy((void *) &msg_tm,
		(void *) localtime((const time_t*) &theMessage->msgHdr->theIndex.LogTime.tv_sec),
		sizeof(struct tm));

	/* Get values to initialize new accumulators if required */

	SORI = theMessage->msgHdr->theIndex.ORI_Id;

	MKE = theMessage->msgHdr->theIndex.InMke;

	Weekday = DayName[msg_tm.tm_wday];

	sprintf(StatsDate,"%04d-%02d-%02d",
		msg_tm.tm_year + 1900,
		msg_tm.tm_mon + 1,
		msg_tm.tm_mday);

	sprintf(StatsHour,"%02d:00", msg_tm.tm_hour);

    /* Find pointers to all the required accumulators - initialize new if needed */

	KTtotal = statsList;

	for (KPsori = KTtotal; 
		KPsori && (strcmp(KPsori->k_name, SORI)); 
		KPsori = KPsori->next);

	if (!KPsori)
	{
		KPsori = FindLastKeyListEntry(KTtotal);
		KPsori->next = NewKeyListEntry();
		KPsori = KPsori->next;
		SetKeyEntryName(KPsori, SORI);
	}

	if (!KTtotal->k_list)
	{
		KTdate = NewKeySubList(KTtotal);
		SetKeyEntryName(KTdate,Weekday);
		KTdate->k_data = (void *) strdup(StatsDate);
	}

	for (KTdate = KTtotal->k_list; 
		KTdate && (strcmp((char *) KTdate->k_data, StatsDate)); 
		KTdate = KTdate->next);

	if (!KTdate)
	{
		KTdate = FindLastKeyListEntry(KTtotal->k_list);
		KTdate->next = NewKeyListEntry();
		KTdate = KTdate->next;
		SetKeyEntryName(KTdate,Weekday);
		KTdate->k_data = strdup(StatsDate);
	}

	if (!KTdate->k_list)
	{
		KThour = NewKeySubList(KTdate);
		SetKeyEntryName(KThour,StatsHour);
	}

	for (KThour = KTdate->k_list;
		KThour && (strcmp(KThour->k_name, StatsHour));
		KThour = KThour->next);

	if (!KThour)
	{
		KThour = FindLastKeyListEntry(KTdate->k_list);
		KThour->next = NewKeyListEntry();
		KThour = KThour->next;
		SetKeyEntryName(KThour, StatsHour);
	}

	if (!KThour->k_list)
	{
		KTmke = NewKeySubList(KThour);
		SetKeyEntryName(KTmke, MKE);
	}

	for (KTmke = KThour->k_list;
		KTmke && (strcmp(KTmke->k_name, MKE));
		KTmke = KTmke->next);

	if (!KTmke)
	{
		KTmke = FindLastKeyListEntry(KThour->k_list);
		KTmke->next = NewKeyListEntry();
		KTmke = KTmke->next;
		SetKeyEntryName(KTmke, MKE);
	}

	if (!KPsori->k_list)
	{
		KPdate = NewKeySubList(KPsori);
		SetKeyEntryName(KPdate,Weekday);
		KPdate->k_data = (void *) strdup(StatsDate);
	}

	for (KPdate = KPsori->k_list; 
		KPdate && (strcmp((char *) KPdate->k_data, StatsDate)); 
		KPdate = KPdate->next);

	if (!KPdate)
	{
		KPdate = FindLastKeyListEntry(KPsori->k_list);
		KPdate->next = NewKeyListEntry();
		KPdate = KPdate->next;
		SetKeyEntryName(KPdate,Weekday);
		KPdate->k_data = (void *) strdup(StatsDate);
	}

	if (!KPdate->k_list)
	{
		KPhour = NewKeySubList(KPdate);
		SetKeyEntryName(KPhour,StatsHour);
	}

	for (KPhour = KPdate->k_list;
		KPhour && (strcmp(KPhour->k_name, StatsHour));
		KPhour = KPhour->next);

	if (!KPhour)
	{
		KPhour = FindLastKeyListEntry(KPdate->k_list);
		KPhour->next = NewKeyListEntry();
		KPhour = KPhour->next;
		SetKeyEntryName(KPhour, StatsHour);
	}

	if (!KPhour->k_list)
	{
		KPmke = NewKeySubList(KPhour);
		SetKeyEntryName(KPmke, MKE);
	}

	for (KPmke = KPhour->k_list;
		KPmke && (strcmp(KPmke->k_name, MKE));
		KPmke = KPmke->next);

	if (!KPmke)
	{
		KPmke = FindLastKeyListEntry(KPhour->k_list);
		KPmke->next = NewKeyListEntry();
		KPmke = KPmke->next;
		SetKeyEntryName(KPmke, MKE);
	}

	/* Now Increment the Accumulators */

	KTtotal->k_id ++;
	KTdate->k_id ++;
	KThour->k_id ++;
	KTmke->k_id ++;

	KPsori->k_id ++;
	KPdate->k_id ++;
	KPhour->k_id ++;
	KPmke->k_id ++;

	return rtn;
}

void freeStatsLists()
{
  FreeKeyList(&oriList);
  FreeKeyList(&statsList);
}

int printStatsLists()
{
	FILE * OUT_FH;
	int rtn = 0;
	int i, t;
	int hr;

	KeyListEntry * KS;
	KeyListEntry * KSref;
	KeyListEntry * KShour;
	KeyListEntry * KSmke;

	KeyListEntry * KT;
	KeyListEntry * KTref;
	KeyListEntry * KThour;
//	KeyListEntry * KTmke;

	const char * ORagy = " ";
	const char * WKday = " ";
	const char * LogDay = " ";

	if (!statsList || !statsList->next)
		return 0;		// nothing to report

	KT = statsList;

	/* Sort the SORI list */
	KT->next = KeyListNameSort(KT->next);

	/* Sort the Totals Tally Forest */

	KT->k_list = KeyListSortDate(KT->k_list);

	for (KTref = KT->k_list; KTref; KTref = KTref->next)
	{
		KTref->k_list = KeyListNameSort(KTref->k_list);
		for (KThour = KTref->k_list; KThour; KThour = KThour->next)
		{
			KThour->k_list = KeyListNameSort(KThour->k_list);
		}
	}
	/* Sort the SORI Tally Forest */

	for (KS = KT->next; KS; KS = KS->next)
	{
		KS->k_list = KeyListSortDate(KS->k_list);
		for (KSref = KS->k_list; KSref; KSref = KSref->next)
		{
			KSref->k_list = KeyListNameSort(KSref->k_list);
			for (KShour = KSref->k_list; KShour; KShour = KShour->next)
			{
				KShour->k_list = KeyListNameSort(KShour->k_list);
			}
		}
	}

	/* Create New Output Report File */

	OUT_FH = fopen(OutFileName, "w+");

	if (!OUT_FH)
		return 1;		// Unable to open output report file
	
	fprintf(OUT_FH, "\nLEDS CCH SUMMARY REPORT FOR (%s)\n\n", KT->k_name);

	fprintf(OUT_FH, "TOTALS\t%d", KT->k_id);
	for (KTref = KT->k_list; KTref; KTref = KTref->next)
	{
		fprintf(OUT_FH, "\t%s\t%d", (char *) KTref->k_data, KTref->k_id);
	}
	fprintf(OUT_FH, "\n");

	fflush(OUT_FH);

	for (KS = KT->next; KS; KS = KS->next)
	{
		fprintf(OUT_FH, "%s\t%d", KS->k_name, KS->k_id);
		for (KTref = KT->k_list, KSref = KS->k_list; KSref;)
		{
			if (!strcmp((char *) KTref->k_data, (char *) KSref->k_data))
			{
				fprintf(OUT_FH,"\t%s\t%d", (char *) KSref->k_data, KSref->k_id);
				KSref = KSref->next;
				KTref = KTref->next;
			}
			else
			{
				fprintf(OUT_FH,"\t\t");
				KTref = KTref->next;
			}
		}
		fprintf(OUT_FH, "\n");
		fflush(OUT_FH);			/* Useful for debugging - save to file */
	}

	if (All_OR_ori)
	{
		/* Don't output the detail report - Just close the file and return */
		fflush(OUT_FH);
		fclose(OUT_FH);
		return rtn;
	}

	fprintf(OUT_FH, "\n\nLEDS CCH DETAIL REPORT FOR (%s)\n\n", KT->k_name);

	for (KS = KT->next; 
		KS; 
		KS = KS->next)
	{
//		KTref = KT->k_list;
//		Iterate through the SORI date records
		t = 0;
		ORagy = KS->k_name;

		for (KSref = KS->k_list; KSref; KSref = KSref->next)
		{
			WKday = KSref->k_name;
			LogDay = (const char *) KSref->k_data;

			/* Start the Agency-ORI date line */
			if (t == 0)
				fprintf(OUT_FH,"%s\t%s\t%s", ORagy, WKday, LogDay);
			else
				fprintf(OUT_FH,"\t%s\t%s", WKday, LogDay);
			t = 1;

			/* Finish the hourly summary line for ORI */

			for (hr = 0; hr < 24; hr++)
			{
				for (KShour = KSref->k_list; 
					KShour && (strcmp(KShour->k_name, hour_table[hr]));
					KShour = KShour->next);

				if (KShour)
				{
					fprintf(OUT_FH, "\t%s\t%d", hour_table[hr], KShour->k_id);
				}
				else
				{
					fprintf(OUT_FH, "\t%s\t0", hour_table[hr]);
				}
			}
			fprintf(OUT_FH, "\n");

			fflush(OUT_FH);			// Save line to output file Useful for Debug

			/* Do the hourly summary for MKEs */

			for (i=0; i < mke_table_length; i++)
			{
				/* Record the message key */
				fprintf(OUT_FH, "%s\t\t", mke_table[i]);

				for (hr=0; hr < 24; hr++)
				{
					for (KShour = KSref->k_list; 
						KShour && (strcmp(KShour->k_name, hour_table[hr]));
						KShour = KShour->next);

					if (KShour)
					{
						/* Found the SORI - hour record - now test for MKE */

						for (KSmke = KShour->k_list; 
							KSmke && (strcmp(KSmke->k_name, mke_table[i]));
							KSmke = KSmke->next);
	
						if (KSmke)
							fprintf(OUT_FH, "\t%s\t%d", hour_table[hr], KSmke->k_id);
						else
							fprintf(OUT_FH, "\t%s\t0", hour_table[hr]);
					}
					else
					{
						fprintf(OUT_FH, "\t%s\t0", hour_table[hr]);
					}
				}
			fprintf(OUT_FH, "\n");
			}
		}
		fprintf(OUT_FH, "\n");
		fflush(OUT_FH);			// save the MKE line to file for debug
	}

	fflush(OUT_FH);		// does not hurt to do this again.
	fclose(OUT_FH);
	return rtn;
}



main(int argc, char * argv[])
{
	int rtn;

	struct tm logtime_tm;
	time_t logtime_sec;

	char * evFileName = NULL;
	char * evDirName = NULL;

	KeyListEntry	* dirList = NULL;
	KeyListEntry	* fileList = NULL;

	KeyListEntry	* DListPtr = NULL;
	KeyListEntry	* FListPtr = NULL;


// GLOBALS
//	char * LogDirPath;
//	char * OutFileName;
//	struct tm start_tm;
//	struct tm end_tm;


// Process program arguments

	rtn = initialize_args(argc, argv);

//	memcpy((void *) &startDateTime, (void *) TmParseDate(startDTime), sizeof(struct tm));
//	memcpy((void *) &endDateTime, (void *) TmParseDate(endDTime), sizeof(struct tm));

//	LogDirPath = strdup(argc[1]);
//	dirList = MakeEvDirList(&startDateTime, &endDateTime, lemsHome, eventlogs);

	dirList = MakeEvDirList(&start_tm, &end_tm, LogDirPath, "");

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

/* Restrict to CrimeVue Event Log Files */
			if (!FindExprList(evFileName, 2, "EV14_0", "EVX14_0")) continue;

			sprintf(fullpath,"%s\\%s",evDirName,evFileName);

			logFileHandle = OpenLemsLogFile(fullpath);

			if (logFileHandle)
			{
				LemsLogMessageStruct * theMessage;
				printf("\nLEMS LOG FILE: %s\\%s\n", evDirName, evFileName);

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

//					if ((tmcomp(&logtime_tm, &startDateTime) >= 0) &&
//						(tmcomp(&logtime_tm, &endDateTime) <= 0))

					if ((tmcomp(&logtime_tm, &start_tm) >= 0) &&
						(tmcomp(&logtime_tm, &end_tm) <= 0))
					{
						if (theMessage->msgHdr->theIndex.Event_Id == OUTPUTMESSAGE &&
							(!strcmp(theMessage->msgHdr->theIndex.OutLine, "LEDS")) &&
							find_mke_in_table(theMessage->msgHdr->theIndex.InMke) &&
							find_qualifying_ori(theMessage->msgHdr->theIndex.ORI_Id))
						{
							rtn = countMessage(theMessage);
							if (rtn)
							{
								// Unsuccessful count
								printf("ERROR from countMessage() - %s\n", strerror(errno));
								exit(1);
								// Usually a memory allocation error - cannot continue
							}
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

	printStatsLists();
	freeStatsLists();

	exit(0);
}

#ifdef __cplusplus
}
#endif
