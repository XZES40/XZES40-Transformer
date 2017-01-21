/**
 * Program CCHQueryStats
 * Prepared for Trish Whitfield - CJIS Director - for legislative information
 * This file walks the LEMS message switch logs and counts events.
 * The output is a (csv) file as data to import into Excel or Database
 *
 * Purpose of this program - consultation with John Tobey - is to provide for
 * billable statistics to recover costs from specific agencies.
 *
 * Additional ORI profile is required to specifically add counts to billable
 * accumulators.
 *
 * Output Record Format
 * Year,Month,(and counts by ORI agency type)
 *
 * This program is based on the LEMSLOG message walker template.
 *
 * Rev 0.1          Aug 19, 2016              Hathaway
 *    Initial Edit - Custom Statistics - CrimeVue CCH User Inquiry Stats
 *
 * Future - TODO: Add entry-update stats, and FBI/NLETS query-update stats.
 *
 */

#include "platform.h"
#include "lemslog.h"
#include "keylist.h"
#include "btcomp.h"
#include "msevlog.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ISUALPHA(c)  (strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ", (c))) ? 1 : 0
#define ISDIGIT(c)  (strchr("0123456789", (c))) ? 1 : 0

#ifdef __cplusplus
extern 'C' {
#endif

/* CrimeVue CCH Message Mke Types to Count */

// Remove specific CCH queries from consideration

char * MkeQryTable[] = {
//  "FQ",                           /* From NLETS */
//  "IQ",                           /* From NLETS */
//  "BA",                           /* SBI Maintenance */
//  "BC",                           /* SBI Maintenance */
//  "BF",                           /* SBI Maintenance */
//  "BH",                           /* SBI Maintenance */
//  "BR",                           /* SBI Maintenance */
//  "BRC",                          /* SBI Maintenance */
//  "RA",                           /* SBI Maintenance */

    "QH",                           /* OR CCH-Index */
    "QHD",                          /* OR CCH-Index + DMV */
    "QWH",                          /* OR CCH-Index + QW */
    "QWHD",                         /* OR CCH-Index + QW + DMV */
    "RR",                           /* OR CCH-Rapsheet */

    NULL
};

char * MkeUpdTable[] = {
    "EAC",
    "EAS",
    "ECS",
    "ECU",
    "EH",
    "EHN",
    "EJS",
    "EJSO",
    "BSCF",
    "MAR",
    "MAS",
    "MBN",
    "MCU",
    "MHID",
    "MJS",
    "MJU",
    "XC",
    "XCS",
    "XH",
    "XHN",
    "XHP",
    "XJS",

    NULL
};


/* Restrict Source Lines */

char * ResSrcLines[] = {
    "BYPASS",
    "NLETS",
    "NCIC",
    "BATCH",
	"IOCLI",
    NULL
};

/* Restrict Specific ORIs */
char * ResSrcOris[] = {
	"OR0SBI000",
	"OR0SBI100",
	"OR0SBI200",
	"OR0SBI300",
	"OR0SBI400",
	NULL
};

/* $.A Query Messages from NCIC */

char * QryDlrA[] = {
    "$.A.CHR.",
    "?.A.CHR.",
    NULL
};

/* Other $.A messages from NCIC set specific flags and perform SID/FBI consolidation.
 * There is no specific table to separate the flags and processing features at this time.
 */

/* Juvenile Agency ORIs */

char * JuvenileORIs[] = {
    "OR001013G",
    "OR002023G",
    "OR003023G",
    "OR004023G",
    "OR005013G",
    "OR006023G",
    "OR007013G",
    "OR008013G",
    "OR009023C",
    "OR010013C",
    "OR011013G",
    "OR012023G",
    "OR014013G",
    "OR015023G",
    "OR017023G",
    "OR018013G",
    "OR019013G",
    "OR020013G",
    "OR021013G",
    "OR022013G",
    "OR023023G",
    "OR024033C",
    "OR025013G",
    "OR026043C",
    "OR026103G",
    "OR026113G",
    "OR026123G",
    "OR026133G",
    "OR027013G",
    "OR028023G",
    "OR029023C",
    "OR030033G",
    "OR031023G",
    "OR032013G",
    "OR033023C",
    "OR033023G",
    "OR034023G",
    "OR035013G",
    "OR036013G",
    "OR036023C",
    "OR037023C",
    NULL
};

/* Indian Tribal ORIs */

char * TribalORIs[] = {
	"OR006029Q",  // CONF TRIBES OF CLUSHA
	"OR0061000",  // COQUILLE TRIBAL PD
	"OR0130300",  // PAIUTE TRIBAL PD
	"OR0160500",  // PD WARM SPRINGS TRIBAL
	"OR018KTHR",  // KLAMTH TRIBES HUMAN RESOURCES
	"OR018KTS0",  // KLAMATH TRIBAL HEALTH AND FAMILY SERVICES
	"OR020IG00",  // CONFED TRIBES GAMING COMM
	"OR021IG00",  // SILETZ IND GAM COMM
	"OR030023A",  // UMATILLA TRIBAL PROSECUTOR
	"OR030111J",  // UMATILLA TRIBAL COURT
	"OR0301200",  // UMATILLA TRIBAL PD
	"OR0SP0005",  // OSP INDIAN GAMING SECTION
	"ORDI00400",  // SILETZ TRIBAL PD
	"ORDI01300",  // COOS CONF TRIBAL PD
	"ORDI01400",  // GRANDE RONDE TRIBAL PD
	"ORDI05000",  // US BIA - WARM SPRINGS
	"ORDI05100",  // US BIA - PORTLAND
	"ORDI06000",  // US BIA - COLUMBIA RIVER INTERTRIBAL FISHERIES ENFORCEMENT
	NULL
};

/* Federal Agency ORI Prefixes */

char * FedORIs[] = {
    "ORATF",
    "ORCG0",
    "ORD0A",
    "ORDOA",
    "ORD0D",
    "ORDOD",
    "ORDEA",
    "ORDI0",
    "ORDI1",
    "ORFAA",
    "ORFBI",
    "ORFPS",
    "ORICE",
    "ORINS",
    "ORPO0",
    "ORP00",
    "ORSS0",
    "ORUSC",
    "ORUSM",
    "ORVA0",
    NULL 
};

/** About C Enum Types.
 * The enum tag_type is a grouping of enumeration values.
 * Each enumeration value is global to the program similar to a #define directive.
 * Therefore each enumaration value can be used in the program as an integer
 * value unconstrained by the enum tag_type.
 *
 * Variables with an enum tag_type can only have the specific subset of integer values
 * associated with the enum tag_type.  Remember that all enum values are global to
 * the program and represent their respective integers.
 *
 * It is therefore encumbent upon the programmer to ensure that the enumeration value
 * members have globally explicit names to avoid identifier conflict.  An enumeration
 * value in one enum tag_type cannot be re-used or re-defined in another enum except
 * as a predefined integer value.
*/


enum AgyTypeCode {
  AGY_EMPTY,
  AGY_PD,
  AGY_SO,
  AGY_OSP,
  AGY_DOC,
  AGY_ATT,
  AGY_CRT,
  AGY_JUV,
  AGY_FED,
  AGY_CJ,
  AGY_REG,
  AGY_1,
  AGY_SBI,
  AGY_2,
  AGY_NOT,
  AGY_OTH,
  AGY_UNK,
  AGY_BLANK,
  AGYMAX
};

typedef enum AgyTypeCode AGY_TYPECODE;

char * AgyTypeDesc[] = {
  "EMPTY",
  "PD",
  "SO",
  "OSP",
  "DOC",
  "DA/ATTY",
  "COURTS",
  "JUV DEPT",
  "FED",
  "CJ AGY",
  "REG",
  "CITY PERMIT",
  "AGY REC CK",
  "OPEN REC",
  "NOTARY",
  "RETIRED",
  "UNKNOWN",
  "BLANK",
  NULL 
};

/* FUTURE ANALYTICS STATS
 *
 * INCLUDE $.A.CHR and $.A.??? from NCIC
 * INCLUDE QUERIES FROM NLETS
*/


typedef int AGY_VALTBL[AGYMAX];


/** The Counters (StatsList)
 *
 *  StatsList ->
 *     k_id = yrmon
 *     k_data -> int StatValues[AGYMAX]
 *     next -> Next yrmonth node -> NULL
 */

KeyListEntry * StatsList = NULL;


/* Return 0 on success, non-zero on memory failure */

int IncrementCounter(int yrmon, AGY_TYPECODE agy)
{
  KeyListEntry * KPtr = StatsList;
  KeyListEntry * KPnext = NULL;
  int * tbl;

  KPtr = StatsList;
  if (KPtr == NULL)
  {
    KPtr = StatsList = NewKeyListEntry();
    if (KPtr == NULL)
      return 1;          // Memory allocation failure
    SetKeyEntryIdValue(KPtr, yrmon);
    if (NewKeyEntryData(KPtr, 1, sizeof(AGY_VALTBL)) == NULL)
    {
      // Memory allocation error - cleanup
      FreeKeyList(&StatsList);
      return 1;
    }
  }
  for (KPtr = StatsList; KPtr->k_id != yrmon ; KPtr = KPtr->next)
  {
    if (KPtr->next) continue;

    if ((KPnext = KPtr->next  = NewKeyListEntry()) == NULL) return 1; // Memory allocation failure;

    SetKeyEntryIdValue(KPnext, yrmon);

    if (NewKeyEntryData(KPnext, 1, sizeof(AGY_VALTBL)) == NULL)
    {
      // Memory allocation error - cleanup
      FreeKeyList(&(KPtr->next));
      return 1;
    }
  }
  
  tbl = (int *) KPtr->k_data;
  tbl[agy]++;
  return 0;  // success
} 


void ReportStats( FILE * outfile )
{
  KeyListEntry * KPtr;
  int * tbl;

  StatsList = KeyListIDValueSort(StatsList);

  /* Print the table header strings */
  fprintf( outfile, "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
    "YEAR","MONTH",
    AgyTypeDesc[  AGY_EMPTY ],
    AgyTypeDesc[  AGY_PD    ],
    AgyTypeDesc[  AGY_SO    ],
    AgyTypeDesc[  AGY_OSP   ],
    AgyTypeDesc[  AGY_DOC   ],
    AgyTypeDesc[  AGY_ATT   ],
    AgyTypeDesc[  AGY_CRT   ],
    AgyTypeDesc[  AGY_JUV   ],
    AgyTypeDesc[  AGY_FED   ],
    AgyTypeDesc[  AGY_CJ    ],
    AgyTypeDesc[  AGY_REG   ],
    AgyTypeDesc[  AGY_1     ],
    AgyTypeDesc[  AGY_SBI   ],
    AgyTypeDesc[  AGY_2     ],
    AgyTypeDesc[  AGY_NOT   ],
    AgyTypeDesc[  AGY_OTH   ],
    AgyTypeDesc[  AGY_UNK   ],
    AgyTypeDesc[  AGY_BLANK ] );

  /* Print the data value records */
  for (KPtr = StatsList; KPtr; KPtr = KPtr->next)
  {
    tbl = (int *) KPtr->k_data;
    fprintf( outfile, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
    KPtr->k_id / 100, /*year*/
    KPtr->k_id % 100, /*month*/
    tbl[  AGY_EMPTY ],
    tbl[  AGY_PD    ],
    tbl[  AGY_SO    ],
    tbl[  AGY_OSP   ],
    tbl[  AGY_DOC   ],
    tbl[  AGY_ATT   ],
    tbl[  AGY_CRT   ],
    tbl[  AGY_JUV   ],
    tbl[  AGY_FED   ],
    tbl[  AGY_CJ    ],
    tbl[  AGY_REG   ],
    tbl[  AGY_1     ],
    tbl[  AGY_SBI   ],
    tbl[  AGY_2     ],
    tbl[  AGY_NOT   ],
    tbl[  AGY_OTH   ],
    tbl[  AGY_UNK   ],
    tbl[  AGY_BLANK ] );
  }
  FreeKeyList(&StatsList);
}


AGY_TYPECODE GetAgyType (const char *ORI)
{
  int i;

  if (ORI == NULL) return AGY_EMPTY;

  if (ORI[0] == '\0') return AGY_EMPTY;

  if (ORI[0] == ' ') return AGY_BLANK;

  if (strlen(ORI) != 9)
    return AGY_EMPTY;

  if (ORI[0] != 'O' || ORI[1] != 'R')
    return AGY_UNK;

  if (strncmp("OR0000000", ORI, 9) == 0)
    return AGY_BLANK;

  /* Special Agency Tests */
  /* Secretary of State Notary */
  if (strcmp(ORI, "OR024SSNP") == 0) return AGY_NOT;

  /* Oregon DHS Criminal Records Unit */
// if (strcmp(ORI, "OR024CRU0") == 0) return AGY_UNK;

  /* General ORI Agency Tests */

  if (ORI[2] == '0' )
  {
    /* State Police ORIs */
    if (ORI[3] == 'S' && ORI[4] == 'P')
      return AGY_OSP;

    /* Oregon County ORIs */
    if (ISDIGIT(ORI[3]) && ISDIGIT(ORI[4]))
    {

      /* Regulatory Agencies */
      if (ISUALPHA(ORI[5]) || ISUALPHA(ORI[6]) || ISUALPHA(ORI[7]))
        return AGY_REG;

      /* Sheriff Offices */
      if (ORI[5] == '0' && ORI[6] == '0' && ORI[7] == '0')
        return AGY_SO;

      /* Criminal Justice Departments or Law Enforcement */
      if (ISDIGIT(ORI[5]) && ISDIGIT(ORI[6]) && ISDIGIT(ORI[7]))
      {

        /* Police Departments */
        if (ISDIGIT(ORI[8]))
          return AGY_PD;

        /* Attorneys */
        if (ORI[8] == 'A')
          return AGY_ATT;

        /* Corrections Departments, Jails, Prisons, County Juvenile Depts */
        if ((ORI[8] == 'C') || (ORI[8] == 'G'))
        {
          for (i=0; JuvenileORIs[i]; i++)
          {
            if (strncmp(JuvenileORIs[i],ORI,9) == 0) 
              return AGY_JUV;
          }
          return AGY_DOC;
        }

        /* Courts of all types */
        if (ORI[8] == 'J')
          return AGY_CRT;

        /* All other criminal justice agencies */
        /* Note that suffix 7(alpha) and 9(alpha) are treated as CJ not Federal */
        if (ISUALPHA(ORI[8]))
          return AGY_CJ;

        /* should never get here - unless ori[8] contains bad data */
        return AGY_UNK;
      }

      /* OR0nn other agencies = regulatory */
      return AGY_REG;
    }

    /* OR0 - No County Code */

    /* OR0 other agencies */
    /* State Police ID Bureau ORIs */

    if (strncmp(ORI, "OR0SBI", 6) == 0)
      return AGY_SBI;

    /* Assume other state regulatory agencies - i.e. ODOT-DMV */
    return AGY_REG;
  }

  /* OR(alpha) */
  /* Federal Agency ORIs - related to NLETS Federal Routing */
  for (i=0; FedORIs[i]; i++)
  {
    if (strncmp(FedORIs[i],ORI,5) == 0)
      return AGY_FED;
  }

  /* OR(alpha) not federal = state regulatory */
  return AGY_REG;
}



/* Return a value if this message is a candidate to be counted */

int MsgSelection(LemsLogMessageStruct * theMessage)
{
  int i;
  
  if (theMessage == NULL) return 0;
  if (theMessage->msgHdr->theIndex.Event_Id  != OUTPUTMESSAGE) return 0;
  if (theMessage->msgHdr->theIndex.ORI_Id[0] == '\0') return 0;

  /* Destination must be CrimeVue LEDS */

  if (strcmp(theMessage->msgHdr->theIndex.Dest_Dev, "LEDS")) return 0;

  /* Omit counts for County 37 */

  if (strncmp(theMessage->msgHdr->theIndex.ORI_Id, "OR037", 5) == 0) return 0;

  /* Restricted Source Lines */
  for (i=0; ResSrcLines[i]; i++)
    if (strcmp(ResSrcLines[i], theMessage->msgHdr->theIndex.InLine) == 0) return 0;

  /* ### FUTURE NCIC $.A HANDLING
   * (Not Yet Implemented)
   */

  /* Restricted Source ORIs */
  for (i=0; ResSrcOris[i]; i++)
    if (strcmp(ResSrcOris[i], theMessage->msgHdr->theIndex.ORI_Id) == 0) return 0;

  /* Must be Oregon ORI or Blank*/
  if (!((theMessage->msgHdr->theIndex.ORI_Id[0] == ' ') ||
       (strncmp(theMessage->msgHdr->theIndex.ORI_Id, "OR", 2) == 0)))  return 0;

  /* Count specific inquiry message keys */
  for (i=0; MkeQryTable[i]; i++)
    if (strcmp(MkeQryTable[i], theMessage->msgHdr->theIndex.InMke) == 0) return 1;

  /* Count specific entry-update message keys */
//  for (i=0; MkeUpdTable[i]; i++_
//    if (strcmp(MkeUpdTable[i], theMessage->msgHdr.theIndex.InMke) == 0) return 1;

  /* DEFAULT = Do Nothing */

  return 0;
}


int AddMessageCount(LemsLogMessageStruct * theMessage)
{
   time_t  t;
   struct tm ts;
   int yrmon;

   t = (time_t) theMessage->msgHdr->theIndex.LogTime.tv_sec;
   memcpy(&ts, localtime(&t), sizeof(struct tm));
   
   yrmon = (ts.tm_year + 1900) * 100;
   yrmon += (ts.tm_mon + 1);

   return IncrementCounter(yrmon, 
       GetAgyType (theMessage->msgHdr->theIndex.ORI_Id));

  /* FUTURE - SUPPORT NCIC $.A MESSAGES */
  /* FUTURE - SUPPORT COUNTING ENTRY-UPDATE MESSAGES */
  /* FUTURE - SUPPORT COUNTING NLETS QUERY MESSAGES */
}

FILE * OpenFileCsv(const char * filename)
{
  return fopen(filename, "w");
}

int CloseFileCsv(FILE * csvFile)
{
  return fclose(csvFile);
}

/*==============================================================================*/


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

//#include "platform.h"

//#include <time.h>
//#include <stdio.h>

//#include "btcomp.h"
//#include "keylist.h"
//#include "lemslog.h"
//#include "msevlog.h"

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


int main(int argc, char * argv[])
{
	int    prsErr	= 0;

	struct tm	startDateTime;
	struct tm	endDateTime;

	char * startDTime	= "ccyy-mm-dd 00:00:00";
	char * endDTime		= "ccyy-mm-dd 23:59:59";
	char * evMountPt	= "X:";
	char * eventLogs	= "";
	char * outfile		= "F:\\lems2k\\xfer\\CchQueryStats.csv";

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

					if (MsgSelection(theMessage))
					{
						if (AddMessageCount(theMessage))
						{
							// Critical Memory or Other Error
							CloseFileCsv(CsvFile);
							fprintf(stderr,"\nERROR: SEVERE PROBLEM\n");
							exit(1);
						}
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

	ReportStats(CsvFile);
	CloseFileCsv(CsvFile);

	exit(0);
}

#ifdef __cplusplus
}
#endif


