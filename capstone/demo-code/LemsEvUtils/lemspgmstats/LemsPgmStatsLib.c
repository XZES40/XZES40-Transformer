

/* File: LemsPgmStatsLib.c  --  8/24/2011  --  Steven J. Hathaway */

/**
 * CUSTOM MESSAGE SWITCH STATISTICS 
 *
 * Statistics by Program Type (Transaction Counts, Not Message Counts).
 *
 * Group By Month, Program Type, User Type
 *
 */

#include "platform.h"
#include "lemslog.h"
#include "keylist.h"

#include <string.h>
#include <stdio.h>
#include <time.h>

/* ORI Search Text from LemsAgyType.c */

extern const char * ATTY_list;
extern const char * CORR_list;
extern const char * COURT_list;
extern const char * MEDEX_list;
extern const char * ODOT_list;
extern const char * FHOUS_list;
extern const char * ORAGY_list;

KeyListEntry * theCounters = NULL;

/* theCounters KeyList Usage
 *
 * (StatsByMonth)   next -> ...
 *    k_name = "Month Year"
 *    k_id   = counter
 *    k_list -> (StatsByProgram)
 *
 * (StatsByProgram) next -> ...
 *    k_name = "Agency Program"
 *    k_id   = counter
 *    k_list -> (StatsByUser)
 *
 * (StatsByUser)    next-> ...
 *    k_name = "User Type"
 *    k_id   = counter
 *    k_list -> (NULL)
 */

/*
 * Output Spreadsheet (csv) Records ...
 *
 *   "Month Year" , counter ,                   ,         ,             ,
 *                ,         , "Agency Program " , counter ,             ,
 *                ,         ,                   ,         , "User Type" , counter
 * -- Empty line between "Month Year" cycles --
 *   "Month Year" , counter ,                   ,         ,             ,
 *                ,         , "Agency Program " , counter ,             ,
 *                ,         ,                   ,         , "User Type" , counter
 */

int IncrementCounters(
	  const char * MonthYear,
	  const char * AgencyPgm,
	  const char * UserType)
{
	KeyListEntry *KP1, *KP2, *KP3;

	if (!(MonthYear && AgencyPgm && UserType))
		return 1;			// Parameter Error

	if (! theCounters)
	{
		if (!(KP1 = theCounters = NewKeyListEntry()))
				return -1;		// Memory Allocation Failure
	}
	else
	{
		for (KP1 = theCounters; KP1; KP1 = KP1->next)
		{
			if (!strcmp(KP1->k_name, MonthYear))
				break;
		}
		if (!KP1)
		{
			if (!(KP1 = (FindLastKeyListEntry(theCounters))->next = NewKeyListEntry()))
				return -1;		// Memory Allocation Failure
		}
	}
	if (!KP1->k_name)
		SetKeyEntryName(KP1, MonthYear);

	KP1->k_id++;	// Increment the transaction count

	if (!KP1->k_list)
	{
		if (!(KP2 = KP1->k_list = NewKeyListEntry()))
			return -1;			// Memory Allocation Failure
	}
	else
	{
		for (KP2 = KP1->k_list; KP2; KP2 = KP2->next)
		{
			if (!strcmp(KP2->k_name, AgencyPgm))
				break;
		}
		if (!KP2)
		{
			if (!(KP2 = (FindLastKeyListEntry(KP1->k_list))->next = NewKeyListEntry()))
				return -1;		// Memory Allocation Failure
		}
	}
	if (!KP2->k_name)
		SetKeyEntryName(KP2, AgencyPgm);

	KP2->k_id++;	// Increment the transaction count

	if (!KP2->k_list)
	{
		if (!(KP3 = KP2->k_list = NewKeyListEntry()))
			return -1;			// Memory Allocation Failure
	}
	else
	{
		for (KP3 = KP2->k_list; KP3; KP3 = KP3->next)
		{
			if (!strcmp(KP3->k_name, UserType))
				break;
		}
		if (!KP3)
		{
			if (!(KP3 = (FindLastKeyListEntry(KP2->k_list))->next = NewKeyListEntry()))
				return -1;		// Memory Allocation Failure
		}
	}
	if (!KP3->k_name)
		SetKeyEntryName(KP3, UserType);

	KP3->k_id++;	// Increment the transaction count
	return 0;
}

char *MonthId[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

/* LogDateText()
 * Convert LogDate to "Mon CCYY" text
 */

const char * LogDateText(const time_t *LogDate)
{
	struct tm TmStruct;
	static char RtnText[20] = "";

	memcpy(&TmStruct, localtime(LogDate), sizeof(struct tm));
	sprintf(RtnText, "%s %4d", MonthId[TmStruct.tm_mon], TmStruct.tm_year + 1900);

	return RtnText;
}

/* SortTheCounters()
 * The top list (theCounters) is already sorted by month.
 * The first sublist is being sorted by project name
 * The second sublist is being sorted by user-type name
 */

void SortTheCounters()
{
	KeyListEntry *KP1, *KP2;

	for (KP1 = theCounters; KP1; KP1 = KP1->next)
	{
		KP1->k_list = KeyListNameSort(KP1->k_list);
		for (KP2 = KP1->k_list; KP2; KP2 = KP2->next)
			KP2->k_list = KeyListNameSort(KP2->k_list);
	}
}

/* Create the CSV Spreadsheet File */

FILE * OpenFileCsv(char * filename)
{
	return fopen(filename, "w");
}

/* Close the CSV Spreadsheet File */

int CloseFileCsv(FILE * csvfile)
{
	FreeKeyList(&theCounters);
	return fclose(csvfile);
}

/* Write the CSV Spreadsheet Report */

int WriteCsvReport(FILE* csvfile)
{
	int rtn;
	KeyListEntry *KP1, *KP2, *KP3;

	SortTheCounters();

	rtn = fprintf(csvfile, "\"MONTH\",\"count\",\"PROJECT\",\"count\",\"USER-TYPE\"\n");
	if (rtn < 0)
		return rtn;		// system error;

	for (KP1 = theCounters; KP1; KP1 = KP1->next)
	{
		rtn = fprintf(csvfile, "\n%s,%d,,,,\n", KP1->k_name, KP1->k_id);
		if (rtn < 0)
			return rtn;		// system error;
		for (KP2 = KP1->k_list; KP2; KP2 = KP2->next)
		{
			rtn = fprintf(csvfile, ",,%s,%d,,\n", KP2->k_name, KP2->k_id);
			if (rtn < 0)
				return rtn;		// system error;
			for (KP3 = KP2->k_list; KP3; KP3 = KP3->next)
			{
				rtn = fprintf(csvfile, ",,,,%s,%d\n", KP3->k_name, KP3->k_id);
			}
		}
	}
	return 0;	// Return SUCCESS
}

/* QUERIES TO LEDS
 *   From ID Bureau
 *   From DMV
 *   From CLETS
 *   From NLETS
 *   From Users
 *   From NCIC ($.A.CHR)
 */

int ChQ2Leds(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(!strcmp(MsgIndex->OutLine, "LEDS")))
	{
		if (strchr("BQR", MsgIndex->OutMke[0]))
		{
			if (!strcmp(MsgIndex->ORI_Id, "OR0SBI000"))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO LEDS",
					"From ID Bureau");
			}
			else if (strstr("DMVP DMVA DMVT", MsgIndex->Src_Dev))
			{
				if (!strcmp("QX", MsgIndex->OutMke))
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO LEDS",
						"From DMV Title");
				}
				else
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO LEDS",
						"From DMV Spawned");
				}
			}
			else if (!strcmp("CLETS", MsgIndex->InLine))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO LEDS",
					"From CLETS");
			}
			else if (!strcmp("NLETS", MsgIndex->InLine))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO LEDS",
					"From NLETS");
			}
			else if (strstr(ORAGY_list, MsgIndex->ORI_Id))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO LEDS",
					"From ORAGY Users");
			}
			else
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO LEDS",
					"From Users");
			}
		}
		else if ((!strcmp(MsgIndex->InLine, "NCIC") &&
			strstr(MsgData, ".A.CHR.")))
		{
			return IncrementCounters(
				LogDateText(&MsgIndex->LogDate),
				"QUERIES TO LEDS",
				"From NCIC");
		}
		else if (!strcmp(MsgIndex->InLine, "NLETS"))
		{
			return IncrementCounters(
				LogDateText(&MsgIndex->LogDate),
				"QUERIES TO LEDS",
				"From NLETS");
		}
	}
	return 0;
}

/* UPDATES TO LEDS
 *   From ID Bureau
 *   From DMV
 *   From NCIC
 *   From EJSO
 *   From Users
 */

int ChU2Leds(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(!strcmp(MsgIndex->OutLine, "LEDS")))
	{
		if (strchr("CELMX$", MsgIndex->OutMke[0]))
		{
			if (!strcmp(MsgIndex->ORI_Id, "OR0SBI000"))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"UPDATES TO LEDS",
					"From ID Bureau");
			}
			else if (strstr("DMVP DMVA DMVT", MsgIndex->Src_Dev))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"UPDATES TO LEDS",
					"From DMV EDLR");
			}
			else if ((!strcmp("NCIC", MsgIndex->InLine)) &&
				(!strstr(MsgData, ".A.CHR.")))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"UPDATES TO LEDS",
					"From NCIC");
			}
			else if (!strcmp("EJSO", MsgIndex->OutMke))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"UPDATES TO LEDS",
					"From EJSO");
			}
			else
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"UPDATES TO LEDS",
					"From Users");
			}
		}
	}
	return 0;
}

/* QUERIES TO NCIC
 *   From LEDS/DB
 *   From DMV
 *   From CLETS
 *   From Users
 */

int ChQ2Ncic(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(!strcmp(MsgIndex->OutLine, "NCIC")))
	{
		if (strchr("QZ", MsgIndex->OutMke[0]))
		{
			if (!strcmp(MsgIndex->ORI_Id, "OR0SBI000"))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO NCIC",
					"From ID Bureau");
			}			
			else if ((!strcmp("DMVP DMVA DMVT", MsgIndex->Src_Dev)) ||
				(!strcmp(MsgIndex->ORI_Id, "OR0DMVR00")))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO NCIC",
					"From DMV");
			}
			else if (!strcmp("CLETS", MsgIndex->InLine))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO NCIC",
					"From CLETS");
			}
			else
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO NCIC",
					"From Users");
			}
		}
		if ((!strcmp("LEDS", MsgIndex->InLine) &&
			MsgData[20] == 'Q'))
		{
			return IncrementCounters(
				LogDateText(&MsgIndex->LogDate),
				"QUERIES TO NCIC",
				"From LEDS");
		}
	}
	return 0;
}

/* UPDATES TO NCIC
 *   From ID Bureau
 *   From Users
 *   From LEDS/DB
 */

int ChU2Ncic(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(!strcmp(MsgIndex->OutLine, "NCIC")))
	{
		if (strchr("CELMX", MsgIndex->OutMke[0]))
		{
			if (!strcmp(MsgIndex->ORI_Id, "OR0SBI000"))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"UPDATES TO NCIC",
					"From ID Bureau");
			}
			else if ((!strcmp("LEDS", MsgIndex->InLine)) &&
				(strchr("CELMX", MsgData[20])))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"UPDATES TO NCIC",
					"From LEDS");
			}
			else if (strcmp("LEDS", MsgIndex->InLine))		// Not LEDS
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"UPDATES TO NCIC",
					"From Users");
			}
		}
	}
	return 0;
}


/* QUERIES+UPDATES TO NICS
 *   From ID Bureau
 *   From Users
 *   From LEDS
 */

int ChQU2Nics(LemsLogMessageStruct * theMessage)
{
	return 0;
}

/* QUERIES TO DMV RECORDS
 *   From LEDS/DB
 *   From CLETS
 *   From NLETS
 *   From ATTY Users
 *   From COURT Users
 *   From MEDEX Users
 *   From FHOUS Users
 *   From ORAGY Users
 *   From CJIS Users
 */

int ChQR2Dmv(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(strstr("DMVP DMVT", MsgIndex->OutLine)))
	{
		if (strstr("DL DLP DLR DLW DPP D07 D13 RDP DQW DQ DQG DNQ KQ "
			"REG QRS RGW V74 RQ RQG RQS RNQ LDB-M", MsgIndex->OutMke))
		{
			if (!strstr("DMVP DMVT", MsgIndex->InLine))		// Standard Inquiry
			{
				if (strstr("NLETS MSGMATCH", MsgIndex->InLine))
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO DMV",
						"From NLETS");
				}
				else if (!strcmp("CLETS", MsgIndex->InLine))
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO DMV",
						"From CLETS");
				}
				else if (!strcmp("LEDS", MsgIndex->InLine))
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO DMV",
						"From LEDS");
				}
				else if (!strcmp("OR0SBI000", MsgIndex->ORI_Id))
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO DMV",
						"From ID Bureau");
				}
				else if (strstr(ATTY_list, MsgIndex->ORI_Id))
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO DMV",
						"From ATTY Users");
				}
				else if (strstr(CORR_list, MsgIndex->ORI_Id))
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO DMV",
						"From CORR Users");
				}
				else if (strstr(COURT_list, MsgIndex->ORI_Id))
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO DMV",
						"From COURT Users");
				}
				else if (strstr(MEDEX_list, MsgIndex->ORI_Id))
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO DMV",
						"From MEDEX Users");
				}
				else if (strstr(ODOT_list, MsgIndex->ORI_Id))
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO DMV",
						"From ODOT Users");
				}
				else if (strstr(FHOUS_list, MsgIndex->ORI_Id))
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO DMV",
						"From FHOUS Users");
				}
				else if (strstr(ORAGY_list, MsgIndex->ORI_Id))
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO DMV",
						"From ORAGY Users");
				}
				else
				{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO DMV",
						"From CJIS Users");
				}
			}
			else		// Spawned Inquiries for RGW
			{
					return IncrementCounters(
						LogDateText(&MsgIndex->LogDate),
						"QUERIES TO DMV",
						"From DMV Spawned");
			}
		}
	}
	return 0;
}

/* QUERIES TO DMV PHOTOS
 *   From Users
 *   From NLETS
 */

int ChQP2Dmv(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(strstr("DMVP DMVT", MsgIndex->OutLine)))
	{
		if (!strcmp("DPL", MsgIndex->OutMke))
		{
			if (strstr("NLETS MSGMATCH", MsgIndex->InLine))
			{

				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO DMVPHOTO",
					"From NLETS");
			}
			else
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO DMVPHOTO",
					"From Users");
			}
		}
	}
	return 0;
}

/* QUERIES TO NLETS
 *   From Users
 *   From DMV
 *   From DMV Spawn CDLIS
 */

int ChQ2Nlets(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(!strcmp("NLETS", MsgIndex->OutLine)))
	{
		if (!strstr("YR DR KR RR FR IR LE", MsgIndex->OutMke))
		{
			if (strstr("DMVP DMVT", MsgIndex->InLine))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO NLETS",
					"From DMV Spawn");
			}
			else if (!strcmp("OR0DMVR00", MsgIndex->ORI_Id))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO NLETS",
					"From DMV");
			}
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO NLETS",
					"From Users");
			}
		}
	}
	return 0;
}

/* QUERIES TO OMMP
 *   From Users (not counting retries and multiples)
 */

int ChQ2ommp(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == INPUTMESSAGE) &&
		(!strcmp("QMAR", MsgIndex->InMke)))
	{
		return IncrementCounters(
			LogDateText(&MsgIndex->LogDate),
			"QUERIES TO OMMP",
			"From Users");
	}
	return 0;
}

/* QUERIES TO ORMH MENTAL HEALTH
 *   From ID Bureau
 *   From Users
 */

int ChQ2ormh(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == INPUTMESSAGE) &&
		(!strcmp("QMEN", MsgIndex->InMke)))
	{
		if (!strcmp("OR0SBI000", MsgIndex->ORI_Id))
		{
			return IncrementCounters(
				LogDateText(&MsgIndex->LogDate),
				"QUERIES TO MENT",
				"From ID Bureau");
		}
		else
		{
			return IncrementCounters(
				LogDateText(&MsgIndex->LogDate),
				"QUERIES TO MENT",
				"From Users");
		}
	}
	return 0;
}

/* QUERIES to COR-PHOTO
 *   From Users
 *   From NLETS
 */

int ChQ2Mugs(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(!strcmp("MUGS", MsgIndex->OutLine)))
	{
		if (!strcmp("NLETS", MsgIndex->InLine))
		{
			return IncrementCounters(
				LogDateText(&MsgIndex->LogDate),
				"QUERIES TO COR-PHOTO",
				"From NLETS");
		}
		else
		{
			return IncrementCounters(
				LogDateText(&MsgIndex->LogDate),
				"QUERIES TO COR-PHOTO",
				"From Users");
		}
	}
	return 0;
}

/* QUERIES TO PTLD-DB
 *   From Users
 */

int ChQ2Ptld(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(!strcmp("PTLD", MsgIndex->Dest_Dev)))
	{
		return IncrementCounters(
			LogDateText(&MsgIndex->LogDate),
			"QUERIES TO PTLD-DB",
			"From Users");
	}
	return 0;
}

/* QUERIES TO CORD-DB
 * From Users
 */

int ChQ2Cord(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(strstr("CORD CRDT", MsgIndex->Dest_Dev)))
	{
		if (!strcmp("OR0SBI000", MsgIndex->ORI_Id))
		{
			return IncrementCounters(
				LogDateText(&MsgIndex->LogDate),
				"QUERIES TO CORD-DB",
				"From ID Bureau");
		}
		else
		{
			return IncrementCounters(
				LogDateText(&MsgIndex->LogDate),
				"QUERIES TO CORD-DB",
				"From Users");
		}
	}
	return 0;
}

/* QUERIES TO CLACKAMAS-DB
 *   From Users
 */

int ChQ2Clas(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(!strcmp("CLAS", MsgIndex->Dest_Dev)))
	{
		return IncrementCounters(
			LogDateText(&MsgIndex->LogDate),
			"QUERIES TO CLASS-DB",
			"From Users");
	}
	return 0;
}

/* QUERIES TO SUN/RAIN-DB (QSRN)
 *   From Users
 */

int ChQ2SunRain(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(!strcmp("QSRN", MsgIndex->InMke)))
	{
		return IncrementCounters(
			LogDateText(&MsgIndex->LogDate),
			"QUERIES TO QSRN-DB",
			"From Users");
	}
	return 0;
}

/* QUERIES TO DMV-HELP
 *   From Users
 *   From NLETS
 */

int Ch2DmvHelp(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if (MsgIndex->Event_Id == OUTPUTMESSAGE &&
		(strstr("MVR1 MVR2 MVR3 MVR4 MVR5 MVR6 MVR7 MVR8 MVR9", MsgIndex->Dest_Dev)))
	{
		if (!strcmp("AM", MsgIndex->InMke))
		{
			if (!strcmp("NLETS", MsgIndex->InLine))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO DMV-HELP",
					"From NLETS");
			}
			else if (!strcmp("OR024015Y", MsgIndex->ORI_Id))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO DMV-HELP",
					"From LEDS-OPS");
			}
			else
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"QUERIES TO DMV-HELP",
					"From Users");
			}
		}
	}
	return 0;
}


/* ADMIN MESSAGES (AM, AML, YQ, YR, HS, AA, $.A, CCC, FCC)
 *   From Users
 *   From DMV-HELP
 *   From NLETS
 *   From LEDS/DB ($.A)
 *   From NCIC ($.x) not ($.A.CHR)
 */

int Ch4AM(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if (MsgIndex->Event_Id == INPUTMESSAGE)
	{
		if (strstr("AM AML YQ YR AA HS CC FCC", MsgIndex->InMke))
		{
			if (!strcmp("NLETS", MsgIndex->InLine))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"ADMIN MSG",
					"From NLETS");
			}
			else if (!strcmp("OR0DMVR00", MsgIndex->ORI_Id))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"ADMIN MSG",
					"From DMV-HELP");
			}
			else
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"ADMIN MSG",
					"From Users");
			}
		}
		else if (MsgIndex->InMke[0] == '$')
		{
			if (!strcmp("LEDS", MsgIndex->InLine))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"ADMIN MSG",
					"From LEDS");
			}
			else if ((!strcmp("NCIC", MsgIndex->InLine)) &&
				(MsgIndex->InMke[2] != 'A'))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"ADMIN MSG",
					"From NCIC");
			}
		}
	}
	return 0;
}

/* WEATHER MESSAGES (HR)
 *   From NLETS
 */

int Ch4Weather(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if ((MsgIndex->Event_Id == INPUTMESSAGE) &&
		(!strcmp("HR", MsgIndex->InMke)) &&
		(!strcmp("NLETS", MsgIndex->InLine)))

	{
		return IncrementCounters(
			LogDateText(&MsgIndex->LogDate),
			"WEATHER MSG",
			"From NLETS");
	}
	return 0;
}

/* HOMELAND SECURITY MESSAGES (HS, HSL)
 *   From NLETS
 *   From LEDS-CTR
 *   From Users
 */

int Ch4HSec(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	if (MsgIndex->Event_Id == INPUTMESSAGE)
	{
		if (strstr("HS HSL", MsgIndex->InMke))
		{
			if (!strcmp("NLETS", MsgIndex->InLine))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"HOMELAND SECURITY",
					"From NLETS");
			}
			else if (!strcmp("OR024015Y", MsgIndex->ORI_Id))
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"HOMELAND SECURITY",
					"From LEDS-CTR");
			}
			else
			{
				return IncrementCounters(
					LogDateText(&MsgIndex->LogDate),
					"HOMELAND SECURITY",
					"From Users");
			}
		}
	}
	return 0;
}

/* OTHER SUMMARY TALLY
 */

int OtherSums(LemsLogMessageStruct * theMessage)
{
	EventsIndexType * MsgIndex;
	unsigned char * MsgData;
	int rtn;

	MsgIndex = &theMessage->msgHdr->theIndex;
	MsgData  = theMessage->msgData->msgBuffer;

	return 0;	/* --- Remove following statistics --- */

	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(!strcmp("NCIC", MsgIndex->OutLine)) &&
		(strstr(MsgData, ".QW.")))
	{
		const char *ptr;
		char oribuf[10];

		ptr = strstr(MsgData,".QW.");
		strncpy(oribuf, ptr+4, 9);
		oribuf[9] = '\0';

		if ((strstr(ATTY_list, oribuf)) ||
			(strstr(CORR_list, oribuf)) ||
			(strstr(COURT_list, oribuf)) ||
			(strstr(MEDEX_list, oribuf)) ||
			(strstr(ODOT_list, oribuf)) ||
			(strstr(FHOUS_list, oribuf)) ||
			(strstr(ORAGY_list, oribuf)))
		{
			rtn = IncrementCounters(
				LogDateText(&MsgIndex->LogDate),
				"OTHER NCIC QW",
				"From Others");
		}
		else
		{
			rtn = IncrementCounters(
				LogDateText(&MsgIndex->LogDate),
				"OTHER NCIC QW",
				"From CJIS");
		}
		if (rtn)
			return rtn;
	}
	if ((MsgIndex->Event_Id == OUTPUTMESSAGE) &&
		(!strcmp("NCIC", MsgIndex->OutLine)) &&
		(strstr(MsgData, ".QV.")))
	{
		if (strstr("DMVP DMVA DMVT", MsgIndex->InLine))  /* Title Checks */
		{
			rtn = IncrementCounters(
				LogDateText(&MsgIndex->LogDate),
				"OTHER NCIC QV",
				"From DMV");
		}
		else
		{
			rtn = IncrementCounters(
				LogDateText(&MsgIndex->LogDate),
				"OTHER NCIC QV",
				"From Users");
		}
		if (rtn)
			return rtn;
	}
	return 0;
}