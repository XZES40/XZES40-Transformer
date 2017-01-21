/* FILE: CdlisStats.c
 * 
 * Read the message switch logs and obtain the following counts:
 *
 * CDL inquiries to CL (DQ)
 * CDL responses from CLLIC0000 (DR)
 * CDL response time for (DR) resolving (DQ)
 *
 * For computing response times, we cannot ensure we see DQ before DR within
 * any date.  Therefore we must store all DQs and DRs in lists, then sort them
 * by timestamp before making any response time decisions.
 *
 * Spreadsheets (Tab Delimited Files)
 *
 * rspfile.dat (Response Times from DRStatsList
 *   MONTH  CCYY/MM  dmv-counts  msg-counts  av-resp  norec-counts  av-resp  nolink-counts  av-resp
 *     day       DD  dmv-counts  msg-counts  av-resp  norec-counts  av-resp  nolink-counts  av-resp
 *     hour   HH:00  dmv-counts  msg-counts  av-resp  norec-counts  av-resp  nolink-counts  av-resp
 *
 * cntfile.dat (Messages Per Agency ORI)
 *   ORI   OR01234567  (FED)  tally  manual  spawned
 *   MONTH    CCYY/MM         tally  manual  spawned
 *
 * Statistics Collection KeyLists
 *
 * DQMsgList	(simple list - Information from DQ Parsing)
 *		k_id = seconds since EPOC(1970)
 *		k_data = DQInfoStruct (Message Matching Data)
 *   Memory Management:
 *		Free the element if older than 1-hour of DR timestamp.
 *		Free the element after matching DR found.
 *
 * DRMsgList	(simple list - Information for matching DQ messages)
 *		k_data = DQInfoStruct (Message Matching Data)
 *   Memory Management:
 *		Free list after end of day processing.
 *
 * DRStatsList	(grouping by Month, Day, Hour)
 *   MONTH
 *    | k_id = CCYYMM
 *    | k_name = "CCYY/MM"
 *    | k_data = RspInfoStruct (counts and accumulated response times)
 *    \DAY
 *      | k_id = Day in month
 *      | k_name = "nn" day in month
 *      | k_data = RspInfoStruct (counts and accumulated response times)
 *      \HOUR
 *        k_id = hour in day
 *        k_name = "HH:00"
 *        k_data = RspInfoStruct (counts and accumulated response times)
 *
 * DRCountsList  (grouping by ORI, Month)
 *   ORI
 *    | k_name = "Agency ORI"
 *    | k_data = DRInfoStruct (tally, manual, spawned)
 *    \MONTH
 *      k_id = CCYYMM
 *      k_name = "CCYY/MM"
 *      k_data = DRInfoStruct (tally, manual, spawned)
 *
 *
 */


//
// Template for LEMS-2000 and LEMS/JX Event Log Processing (Include Files)
//

#include "platform.h"

#include <time.h>
#include <stdio.h>

#include "keylist.h"
#include "msevlog.h"
#include "lemslog.h"

#include "btcomp.h"

/*=====================================================================*/

#ifdef __cplusplus
extern 'C' {
#endif

typedef struct DQInfo_st {
	struct timeval LogTime;
	char ORI[10];		// ORI of agency issuing DQ, DLP, DLR, ...
	char SrcDev[10];	// If SrcDev == "DMVT", automatic spawn
	char Suppl[12];		// Effective User Device
	char OLN[20];		// OLN: driver license
	char OLS[4];		// OLS: driver license state
	int  RspType;		// 0=stdrecord, 1=norecord, 2=nolink
}DQInfoStruct;

typedef struct RspInfo_st {
	int  DmvCount;		// auto-generated message count
	int  MsgCount;		// messages responses except NoRec, NoLink
	int  MsgRspTime;	// milliseconds (10^-3)
	int  NoRecord;		// NoRecord responses
	int  NoRecTime;		// milliseconds (10^-3)
	int  NoLink;		// NoLink responses
	int  NoLinkTime;	// milliseconds (10^-3)
}RspInfoStruct;

typedef struct DRInfo_st {
	int  tally;			// message group tally
	int  manual;		// message group manual inquiry
	int  spawned;		// message group automatic inquiry
}DRInfoStruct;


KeyListEntry * DQMsgList = NULL;

KeyListEntry * DRMsgList = NULL;

KeyListEntry * DRStatsList = NULL;

KeyListEntry * DRCountsList = NULL;

void RecordDQMessage(DQInfoStruct * info)
{
	KeyListEntry * KPtr;
	DQInfoStruct * KData;

	if (!info)
		return;

	if (!DQMsgList)
		KPtr = DQMsgList = NewKeyListEntry();
	else
	{
		KPtr = FindLastKeyListEntry(DQMsgList);
		KPtr->next = NewKeyListEntry();
		KPtr = KPtr->next;
	}

	KData = NewKeyEntryData(KPtr, 1, sizeof(DQInfoStruct));
	memcpy(KData, info, sizeof(DQInfoStruct));
	SetKeyEntryIdValue(KPtr, KData->LogTime.tv_sec);
}

void RecordDRMessage(DQInfoStruct * info)
{
	KeyListEntry * KPtr;
	DQInfoStruct * KData;

	if (!info)
		return;

	if (!DRMsgList)
		KPtr = DRMsgList = NewKeyListEntry();
	else
	{
		KPtr = FindLastKeyListEntry(DRMsgList);
		KPtr->next = NewKeyListEntry();
		KPtr = KPtr->next;
	}

	KData = NewKeyEntryData(KPtr, 1, sizeof(DQInfoStruct));
	memcpy(KData, info, sizeof(DQInfoStruct));
}

/* Helper Function for KeyList Sort */

int CompareDQTime(const KeyListEntry * KP1, const KeyListEntry * KP2)
{
	DQInfoStruct *P1, *P2;
	P1 = (DQInfoStruct *) KP1->k_data;
	P2 = (DQInfoStruct *) KP2->k_data;
	if (P1->LogTime.tv_sec < P2->LogTime.tv_sec)
		return -1;
	if (P1->LogTime.tv_sec > P2->LogTime.tv_sec)
		return 1;
	if (P1->LogTime.tv_usec < P2->LogTime.tv_usec)
		return -1;
	if (P1->LogTime.tv_usec > P2->LogTime.tv_usec)
		return 1;
	return 0;
}

/* Find DQ Message :: selective examination of MatchData */

KeyListEntry * FindDQMessage(DQInfoStruct * MatchData)
{
	KeyListEntry * KPtr;
	DQInfoStruct * DQdata;

	for (KPtr = DQMsgList; KPtr; KPtr = KPtr->next)
	{
		/* Examine search criteria for match */
		DQdata = (DQInfoStruct *) GetKeyEntryData(KPtr);

		if (DQdata->LogTime.tv_sec > MatchData->LogTime.tv_sec)
			return NULL;

		if (DQdata->LogTime.tv_sec == MatchData->LogTime.tv_sec &&
			DQdata->LogTime.tv_usec > MatchData->LogTime.tv_usec)
			return NULL;

		if (strcmp(DQdata->ORI, MatchData->ORI) == 0 &&
			strcmp(DQdata->Suppl, MatchData->Suppl) == 0)
		{
			if (DQdata->OLN[0] && MatchData->OLN[0] &&
				(strcmp(DQdata->OLN, MatchData->OLN) == 0))
			{
				if (DQdata->OLS[0] && MatchData->OLS[0] &&
					(strcmp(DQdata->OLS, MatchData->OLS) == 0))
					break;
				else if (!MatchData->OLS[0])
					break;
			}
			else if (!MatchData->OLN[0])
				break;
		}
	}
	return KPtr;
}

void FreeDQMessage(KeyListEntry * mptr)
{
	KeyListEntry * KPtr;

	if (!mptr)
		return;
	if (mptr == DQMsgList)
	{
		DQMsgList = DropKeyListEntry(DQMsgList);
		return;
	}

	for (KPtr = DQMsgList; KPtr; KPtr = KPtr->next)
	{
		if (KPtr->next == mptr)
			break;
	}
	if (KPtr)
		KPtr->next = DropKeyListEntry(KPtr->next);
}

/* Parse DQ Message and write record to DQMsgList */

void ProcessDqMessage(LemsLogMessageStruct * theMessage)
{
	DQInfoStruct DQInfoData;
	char *ptr, *p2, c;

	if (!theMessage)
		return;

	if (theMessage->msgData->msgLogLen <= 0)
		return;

	memset(&DQInfoData, 0, sizeof(DQInfoStruct));

	if ((ptr = strchr(theMessage->msgData->msgBuffer, '*')) == NULL)
		return;

	ptr++;
	strncpy(DQInfoData.Suppl, ptr, 10);
	strncpy(DQInfoData.ORI, theMessage->msgHdr->theIndex.ORI_Id, 9);
	strncpy(DQInfoData.SrcDev, theMessage->msgHdr->theIndex.Src_Dev, 8);
	if (ptr = strstr(ptr, "OLN/"))
	{
		ptr +=4;
		p2 = strpbrk(ptr, ".\n");
		if (p2)
		{
			c=*p2;
			*p2 = '\0';
		}
		strncpy(DQInfoData.OLN, ptr, 19);
		if (p2) *p2 = c;
	}
	if (ptr && (ptr = strstr(ptr, "OLS/")))
	{
		ptr += 4;
		p2 = strpbrk(ptr, ".\n");
		if (p2)
		{
			c=*p2;
			*p2 = '\0';
		}
		strncpy(DQInfoData.OLS, ptr, 2);
		if (p2) *p2 = c;
	}

	memcpy(&DQInfoData.LogTime, &theMessage->msgHdr->theIndex.LogTime, sizeof(struct timeval));

	RecordDQMessage(&DQInfoData);
	return;
}




/* Parse DR Message and write record to DRMsgList */

void ProcessDrMessage(LemsLogMessageStruct * theMessage)
{
	// "NOT ON FILE"

	// "STATE IS TEMPORARILY UNAVAILABLE"

	// Cannot count on the 2-char integrity of .ncicLSTA code from "CLLIC0000"
	// CDLIS reports "OREGON" instead of "OR" for this code.
	// NLETS InquiryData does not return the OLS field.

	DQInfoStruct DRInfoData;
	char *ptr, *p1, c;

	if (!theMessage)
		return;

	if (theMessage->msgData->msgLogLen <= 0)
		return;

	memset(&DRInfoData, 0, sizeof(DQInfoStruct));

	/* The Message LogTime */

	memcpy(&DRInfoData.LogTime, &theMessage->msgHdr->theIndex.LogTime, sizeof(struct timeval));

	/* The Message "*suppl-address" = <device>OR<trno> */

	ptr = strstr(theMessage->msgData->msgBuffer, "DocumentControlFieldText");
	ptr = strchr(ptr,'>');
	ptr++;
	strncpy(DRInfoData.Suppl, ptr, 10);

	/* The Message "Oregon Agency ORI" */

	ptr = strstr(theMessage->msgData->msgBuffer, "DocumentSubject.Organization");
	if (!ptr) return;
	ptr = strstr(ptr, "OrganizationORIID");
	if (!ptr) return;
	ptr = strchr(ptr,'>');
	ptr = strstr(ptr, "ID>");
	ptr = strchr(ptr,'>');
	ptr++;
	strncpy(DRInfoData.ORI, ptr, 9);

	/* The Driver License Number OLN/___ */

	ptr = strstr(theMessage->msgData->msgBuffer, "NLETSInquiryData");
	if (ptr)
	{
		ptr = strstr(ptr, "PersonDriverLicenseID");
		ptr = strchr(ptr,'>');
		ptr = strstr(ptr, "ID>");
		ptr = strchr(ptr,'>');
		ptr++;
		p1 = strchr(ptr,'<');
		c = *p1;
		*p1 = '\0';
	}
	else if (strstr(theMessage->msgData->msgBuffer, "NLETSResponseData"))
	{
		ptr = strstr(ptr, "DriverAuthorizationID");
		ptr = strchr(ptr,'>');
		ptr = strstr(ptr, "ID>");
		ptr = strchr(ptr,'>');
		ptr++;
		p1 = strchr(ptr,'<');
		c = *p1;
		*p1 = '\0';
	}
	if (ptr)
	{
		strncpy(DRInfoData.OLN, ptr, 19);
		*p1 = c;
	}

	/* Note: The driver license state OLS/__ is not coded, expanded */
	/* i.e.  OLS/OR is expanded to be "OREGON" */
	/*       OLS/MT is expanded to be "MONTANNA" */

	/* "NOT ON FILE" = Not Found */

	if (strstr(theMessage->msgData->msgBuffer, "NOT ON FILE"))
		DRInfoData.RspType = 1;

	/* "STATE IS TEMPORARILY UNAVAILABLE" = No Link */

	if (strstr(theMessage->msgData->msgBuffer, " TEMPORARILY UNAVAILABLE"))
		DRInfoData.RspType = 2;
	
	RecordDRMessage(&DRInfoData);
	return;

}

int AddToCDLStatsTrees(int rspTime, DQInfoStruct * RcdData)
{
	KeyListEntry	*KPtr1, *KPtr2, *KPtr3;
	RspInfoStruct	RspInfo;
	DRInfoStruct	CountInfo;

	RspInfoStruct	*RPtr;
	DRInfoStruct	*CPtr;

	int    ccyymm_i;
	char   ccyymm_s[8];		// "CCYY-MM"
	int    dd_i;
	char   dd_s[4];			// "DD"
	int    hh_i;
	char   hh_s[6];			// "HH:00"

	struct tm	time_tm;

	if (!RcdData)
		return 1;	//error

	memcpy(&time_tm, localtime(&RcdData->LogTime.tv_sec), sizeof(struct tm));

	ccyymm_i = ((time_tm.tm_year + 1900) * 100) + time_tm.tm_mon + 1;
	sprintf(ccyymm_s, "%04d-%02d", time_tm.tm_year + 1900, time_tm.tm_mon + 1);
	dd_i = time_tm.tm_mday;
	sprintf(dd_s, "%02d", time_tm.tm_mday);
	hh_i = time_tm.tm_hour;
	sprintf(hh_s, "%02d:00", time_tm.tm_hour);

	memset(&RspInfo, 0, sizeof(RspInfoStruct));
	memset(&CountInfo, 0, sizeof(DRInfoStruct));

	CountInfo.tally = 1;
	if (strcmp(RcdData->SrcDev, "DMVT") == 0)
	{
		RspInfo.DmvCount = 1;
		CountInfo.spawned = 1;
	}
	else
		CountInfo.manual = 1;

	switch(RcdData->RspType)
	{
	case 0:	// STD CDL RESPONSE
		RspInfo.MsgCount = 1;
		RspInfo.MsgRspTime = rspTime;
		break;
	case 1:	// NO CDL RECORD
		RspInfo.NoRecord = 1;
		RspInfo.NoRecTime = rspTime;
		break;
	case 2:	// NO LINK
		RspInfo.NoLink = 1;
		RspInfo.NoLinkTime = rspTime;
		break;
	}
	
	/* Update the DRStatsList */

	if (!DRStatsList)
	{
		KPtr1 = DRStatsList = NewKeyListEntry();
	}
	else
	{
		for (KPtr1 = DRStatsList; KPtr1 ; KPtr1=KPtr1->next)
		{
			if (strcmp(KPtr1->k_name, ccyymm_s) == 0)
				break;
		}
		if (!KPtr1)
		{
			KPtr1 = FindLastKeyListEntry(DRStatsList);
			KPtr1->next = NewKeyListEntry();
			KPtr1 = KPtr1->next;
		}
	}
	if (!KPtr1)
		return 1;	// memory error
	if (!KPtr1->k_name)
	{
		SetKeyEntryIdValue(KPtr1, ccyymm_i);
		SetKeyEntryName(KPtr1, ccyymm_s);
		NewKeyEntryData(KPtr1, 1, sizeof(RspInfoStruct));
	}

	RPtr = (RspInfoStruct *) KPtr1->k_data;
	RPtr->DmvCount += RspInfo.DmvCount;
	RPtr->MsgCount += RspInfo.MsgCount;
	RPtr->MsgRspTime += RspInfo.MsgRspTime;
	RPtr->NoRecord += RspInfo.NoRecord;
	RPtr->NoRecTime += RspInfo.NoRecTime;
	RPtr->NoLink += RspInfo.NoLink;
	RPtr->NoLinkTime += RspInfo.NoLinkTime;

	if (!KPtr1->k_list)
	{
		KPtr2 = NewKeySubList(KPtr1);
	}
	else
	{
		for (KPtr2 = GetKeyEntrySubList(KPtr1); KPtr2 ; KPtr2=KPtr2->next)
		{
			if (strcmp(KPtr2->k_name, dd_s) == 0)
				break;
		}
		if (!KPtr2)
		{
			KPtr2 = FindLastKeyListEntry(KPtr1->k_list);
			KPtr2->next = NewKeyListEntry();
			KPtr2 = KPtr2->next;
		}
	}
	if (!KPtr2)
		return 1;	// memory error
	if (!KPtr2->k_name)
	{
		SetKeyEntryIdValue(KPtr2, dd_i);
		SetKeyEntryName(KPtr2, dd_s);
		NewKeyEntryData(KPtr2, 1, sizeof(RspInfoStruct));
	}

	RPtr = (RspInfoStruct *) KPtr2->k_data;
	RPtr->DmvCount += RspInfo.DmvCount;
	RPtr->MsgCount += RspInfo.MsgCount;
	RPtr->MsgRspTime += RspInfo.MsgRspTime;
	RPtr->NoRecord += RspInfo.NoRecord;
	RPtr->NoRecTime += RspInfo.NoRecTime;
	RPtr->NoLink += RspInfo.NoLink;
	RPtr->NoLinkTime += RspInfo.NoLinkTime;
	
	if (!KPtr2->k_list)
	{
		KPtr3 = NewKeySubList(KPtr2);
	}
	else
	{
		for (KPtr3 = GetKeyEntrySubList(KPtr2); KPtr3 ; KPtr3=KPtr3->next)
		{
			if (strcmp(KPtr3->k_name, hh_s) == 0)
				break;
		}
		if (!KPtr3)
		{
			KPtr3 = FindLastKeyListEntry(KPtr2->k_list);
			KPtr3->next = NewKeyListEntry();
			KPtr3 = KPtr3->next;
		}
	}
	if (!KPtr3)
		return 1;	// memory error
	if (!KPtr3->k_name)
	{
		SetKeyEntryIdValue(KPtr3, hh_i);
		SetKeyEntryName(KPtr3, hh_s);
		NewKeyEntryData(KPtr3, 1, sizeof(RspInfoStruct));
	}

	RPtr = (RspInfoStruct *) KPtr3->k_data;
	RPtr->DmvCount += RspInfo.DmvCount;
	RPtr->MsgCount += RspInfo.MsgCount;
	RPtr->MsgRspTime += RspInfo.MsgRspTime;
	RPtr->NoRecord += RspInfo.NoRecord;
	RPtr->NoRecTime += RspInfo.NoRecTime;
	RPtr->NoLink += RspInfo.NoLink;
	RPtr->NoLinkTime += RspInfo.NoLinkTime;

	/* Update the DRCountsList */

	if (!DRCountsList)
	{
		KPtr1 = DRCountsList = NewKeyListEntry();
	}
	else
	{
		for (KPtr1 = DRCountsList; KPtr1 ; KPtr1=KPtr1->next)
		{
			if (strcmp(KPtr1->k_name, RcdData->ORI) == 0)
				break;
		}
		if (!KPtr1)
		{
			KPtr1 = FindLastKeyListEntry(DRCountsList);
			KPtr1->next = NewKeyListEntry();
			KPtr1 = KPtr1->next;
		}
	}
	if (!KPtr1)
		return 1;	// memory error
	if (!KPtr1->k_name)
	{
		SetKeyEntryName(KPtr1, RcdData->ORI);
		NewKeyEntryData(KPtr1, 1, sizeof(DRInfoStruct));
	}

	CPtr = (DRInfoStruct *) KPtr1->k_data;
	CPtr->tally += CountInfo.tally;
	CPtr->spawned += CountInfo.spawned;
	CPtr->manual += CountInfo.manual;

	if (!KPtr1->k_list)
	{
		KPtr2 = NewKeySubList(KPtr1);
	}
	else
	{
		for (KPtr2 = GetKeyEntrySubList(KPtr1); KPtr2 ; KPtr2=KPtr2->next)
		{
			if (strcmp(KPtr2->k_name, ccyymm_s) == 0)
				break;
		}
		if (!KPtr2)
		{
			KPtr2 = FindLastKeyListEntry(KPtr1->k_list);
			KPtr2->next = NewKeyListEntry();
			KPtr2 = KPtr2->next;
		}
	}
	if (!KPtr2)
		return 1;	// memory error
	if (!KPtr2->k_name)
	{
		SetKeyEntryIdValue(KPtr2, ccyymm_i);
		SetKeyEntryName(KPtr2, ccyymm_s);
		NewKeyEntryData(KPtr2, 1, sizeof(DRInfoStruct));
	}

	CPtr = (DRInfoStruct *) KPtr2->k_data;
	CPtr->tally += CountInfo.tally;
	CPtr->spawned += CountInfo.spawned;
	CPtr->manual += CountInfo.manual;

	return 0;	// success
}

/* Process the DRMsgList and DQMsgList, Creating Statistics Trees */

void CreateCdlStats()
{
	KeyListEntry *KPtrQ = NULL;
	KeyListEntry *KPtrR = NULL;

	DQInfoStruct * DQdata;
	DQInfoStruct * DRdata;

	time_t trimTime;

	DQMsgList = KeyListSort(DQMsgList,CompareDQTime);
	DRMsgList = KeyListSort(DRMsgList,CompareDQTime);

	for (KPtrR = DRMsgList; KPtrR; KPtrR = KPtrR->next)
	{
		int rspTime = 0;

		DRdata = (DQInfoStruct *) KPtrR->k_data;
		/* Memory Management */
		trimTime = DRdata->LogTime.tv_sec - 3600;  // don't match anyting older(1 Hr)
		while (1)
		{
			if (!DQMsgList)
				return;
			DQdata = (DQInfoStruct *) DQMsgList->k_data;
			if (DQdata->LogTime.tv_sec <= trimTime)
			{
				DQMsgList = DropKeyListEntry(DQMsgList);
				continue;
			}
			break;
		}
		if (!DQMsgList)
		{
			FreeKeyList(&DRMsgList);
			return;
		}
		if ((KPtrQ = FindDQMessage(DRdata)) == NULL)
			continue;
		DQdata = (DQInfoStruct *) KPtrQ->k_data;
		if (DQdata->LogTime.tv_sec > DRdata->LogTime.tv_sec)
			continue;
		if (DQdata->LogTime.tv_sec == DRdata->LogTime.tv_sec &&
			DQdata->LogTime.tv_usec > DRdata->LogTime.tv_usec)
			continue;
		rspTime = (DRdata->LogTime.tv_sec - DQdata->LogTime.tv_sec) * 1000;
		rspTime = rspTime + ((
			DRdata->LogTime.tv_usec - DQdata->LogTime.tv_usec) / 1000);

		DQdata->RspType = DRdata->RspType;
		AddToCDLStatsTrees(rspTime, DQdata);
		FreeDQMessage(KPtrQ);
	}

}

/* Append KeyList(DRStatsList) as records to filename */

void PrintRespTimes(const char * filename)
{
	FILE * FP;
	KeyListEntry *KPtr1, *KPtr2, *KPtr3;
	RspInfoStruct * RPtr;

	if (!DRStatsList)
		return;

	if ((FP = fopen(filename,"w")) == NULL)
		return;

	/* Sort the Statistics (Month, Day, Hour) */

	DRStatsList = KeyListIDValueSort(DRStatsList);
	for (KPtr1 = DRStatsList; KPtr1; KPtr1 = KPtr1->next)
	{
		KPtr1->k_list = KeyListIDValueSort(KPtr1->k_list);
		for (KPtr2 = KPtr1->k_list; KPtr2; KPtr2 = KPtr2->next)
		{
			KPtr2->k_list = KeyListIDValueSort(KPtr2->k_list);
		}
	}

	/* Report File Header */

	fprintf(FP, "\n=========================================\n"
		"OREGON - CDLIS - RESPONSE TIME STATISTICS\n"
		"=========================================\n"
		"\t\tSPAWNED\tRETURNS\tAVG-RSP\tNOFILE\tAVG-RSP\tNOLINK\tAVG-RSP\n");

	for (KPtr1 = DRStatsList; KPtr1; KPtr1 = KPtr1->next)
	{
		RPtr = (RspInfoStruct *) KPtr1->k_data;
		fprintf(FP, "\nMONTH\t%s\t%d\t%d\t%d.%03d\t%d\t%d.%03d\t%d\t%d.%03d",
			KPtr1->k_name,
			RPtr->DmvCount,
			RPtr->MsgCount,		/* (Avoid Divide By Zero (test)?(test):1) */
			((RPtr->MsgRspTime) / (RPtr->MsgCount?RPtr->MsgCount:1)) / 1000,
			((RPtr->MsgRspTime) / (RPtr->MsgCount?RPtr->MsgCount:1)) % 1000,
			RPtr->NoRecord,
			((RPtr->NoRecTime) / (RPtr->NoRecord?RPtr->NoRecord:1)) / 1000,
			((RPtr->NoRecTime) / (RPtr->NoRecord?RPtr->NoRecord:1)) % 1000,
			RPtr->NoLink,
			((RPtr->NoLinkTime) / (RPtr->NoLink?RPtr->NoLink:1)) / 1000,
			((RPtr->NoLinkTime) / (RPtr->NoLink?RPtr->NoLink:1)) % 1000);

		for (KPtr2 = KPtr1->k_list; KPtr2; KPtr2 = KPtr2->next)
		{
			RPtr = (RspInfoStruct *) KPtr2->k_data;
			fprintf(FP, "\nday\t%s\t%d\t%d\t%d.%03d\t%d\t%d.%03d\t%d\t%d.%03d\n",
				KPtr2->k_name,
				RPtr->DmvCount,
				RPtr->MsgCount,		/* (Avoid Divide By Zero (test)?(test):1) */
				((RPtr->MsgRspTime) / (RPtr->MsgCount?RPtr->MsgCount:1)) / 1000,
				((RPtr->MsgRspTime) / (RPtr->MsgCount?RPtr->MsgCount:1)) % 1000,
				RPtr->NoRecord,
				((RPtr->NoRecTime) / (RPtr->NoRecord?RPtr->NoRecord:1)) / 1000,
				((RPtr->NoRecTime) / (RPtr->NoRecord?RPtr->NoRecord:1)) % 1000,
				RPtr->NoLink,
				((RPtr->NoLinkTime) / (RPtr->NoLink?RPtr->NoLink:1)) / 1000,
				((RPtr->NoLinkTime) / (RPtr->NoLink?RPtr->NoLink:1)) % 1000);

			for (KPtr3 = KPtr2->k_list; KPtr3; KPtr3 = KPtr3->next)
			{
				RPtr = (RspInfoStruct *) KPtr3->k_data;
				fprintf(FP, "hour\t%s\t%d\t%d\t%d.%03d\t%d\t%d.%03d\t%d\t%d.%03d\n",
					KPtr3->k_name,
					RPtr->DmvCount,
					RPtr->MsgCount,		/* (Avoid Divide By Zero (test)?(test):1) */
					((RPtr->MsgRspTime) / (RPtr->MsgCount?RPtr->MsgCount:1)) / 1000,
					((RPtr->MsgRspTime) / (RPtr->MsgCount?RPtr->MsgCount:1)) % 1000,
					RPtr->NoRecord,
					((RPtr->NoRecTime) / (RPtr->NoRecord?RPtr->NoRecord:1)) / 1000,
					((RPtr->NoRecTime) / (RPtr->NoRecord?RPtr->NoRecord:1)) % 1000,
					RPtr->NoLink,
					((RPtr->NoLinkTime) / (RPtr->NoLink?RPtr->NoLink:1)) / 1000,
					((RPtr->NoLinkTime) / (RPtr->NoLink?RPtr->NoLink:1)) % 1000);
			}
		}
	}

	fflush(FP);
	fclose(FP);
	FreeKeyList(&DRStatsList);
}

/* Append KeyList(DRCountsList) as records to filename */

void PrintOriCounts(const char * filename)
{
	FILE * FP;
	KeyListEntry *KPtr1, *KPtr2;
	DRInfoStruct * RPtr;
	char FedAgy[6];

	FedAgy[5] = '\0';

	if (!DRCountsList)
		return;

	if ((FP = fopen(filename,"w")) == NULL)
		return;

	/* Sort the Statistics (ORI, Month) */
	/* Note: financial audit of $0.04 per transaction */

	DRCountsList = KeyListNameSort(DRCountsList);
	for (KPtr1 = DRCountsList; KPtr1; KPtr1 = KPtr1->next)
	{
		KPtr1->k_list = KeyListIDValueSort(KPtr1->k_list);
	}

	/* Report File Header */

	fprintf(FP, "\n======================================\n"
		"OREGON - CDLIS - AGENCY ORI STATISTICS\n"
		"======================================\n"
		"\t\tFEDERAL\tTALLY\tMANUAL\tSPAWNED\n");
	
	for (KPtr1 = DRCountsList; KPtr1; KPtr1 = KPtr1->next)
	{
		if (isalpha(KPtr1->k_name[2]))
		{
			strcpy(FedAgy,"(");
			strncat(FedAgy,&(KPtr1->k_name[2]), 3);
			strcat(FedAgy,")");
		}
		else
			FedAgy[0] = '\0';
		RPtr = (DRInfoStruct *) KPtr1->k_data;
		fprintf(FP,"\nORI\t%s\t%s\t%d\t%d\t%d\n",
			KPtr1->k_name,
			FedAgy,
			RPtr->tally,
			RPtr->manual,
			RPtr->spawned);

		for (KPtr2 = KPtr1->k_list; KPtr2; KPtr2 = KPtr2->next)
		{
			RPtr = (DRInfoStruct *) KPtr2->k_data;
			fprintf(FP,"month\t%s\t%s\t%d\t%d\t%d\n",
				KPtr2->k_name,
				FedAgy,
				RPtr->tally,
				RPtr->manual,
				RPtr->spawned);
		}
	}

	fflush(FP);
	fclose(FP);
	FreeKeyList(&DRCountsList);
}



/*===========================================================*/
//
// Template for LEMS-2000 and LEMS/JX Event Log Processing
//

KeyListEntry	* dirlist = NULL;

KeyListEntry	* fileList = NULL;

struct tm	startDateTime;

struct tm	endDateTime;

main(int argc, char * argv[])
{

//	char * startDTime = "2009-02-04 13:00:00";
//	char * endDTime = "2009-02-05 08:59:59";
//	char * lemsHome = "C:\\lems2k";
//	char * eventlogs = "BackEvents";

	char startDTime[22];
	char endDTime[22];
	char lemsHome[260];
	char eventlogs[260];
    char respfile[260];
	char countfile[260];

	struct tm logtime_tm;
	time_t logtime_sec;

	char * evFileName = NULL;
	char * evDirName = NULL;

	KeyListEntry	* dirList = NULL;
	KeyListEntry	* fileList = NULL;

	KeyListEntry	* DListPtr = NULL;
	KeyListEntry	* FListPtr = NULL;

	/* PROGRAM lemshome evtlogs start-time end-time [rspfile.dat [cntfile.dat]] */

	if (argc > 7 || argc < 5)
	{
		printf("\nUSAGE: %s lemshome evtlogs start-time end-time [rspfile.dat [cntfile.dat]]\n\n", argv[0]);
		exit(1);
	}

	/* SAVE Program Arguments */

	strcpy(lemsHome, argv[1]);
	strcpy(eventlogs, argv[2]);
	strcpy(startDTime, argv[3]);
	strcpy(endDTime, argv[4]);
	if (argc > 5)
		strcpy(respfile, argv[5]);
	else
	{
		strcpy(respfile, lemsHome);
		strcat(respfile, "\\xfer\\rspfile.dat");
	}
	if (argc > 6)
		strcpy(countfile, argv[6]);
	else
	{
		strcpy(countfile, lemsHome);
		strcat(countfile, "\\xfer\\cntfile.dat");
	}

	/* The Search Limits */

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

					// MESSAGE DATA INFORMATION

//					(int)			= theMessage->msgData->msgLogLen;
//					(char *)		= theMessage->msgData->msgBuffer;

					logtime_sec = theMessage->msgHdr->theIndex.LogTime.tv_sec;
					memcpy(&logtime_tm, localtime(&logtime_sec), sizeof(struct tm));

					if ((tmcomp(&logtime_tm, &startDateTime) >= 0) &&
						(tmcomp(&logtime_tm, &endDateTime) <= 0) &&
						((theMessage->msgHdr->theIndex.Event_Id == OUTPUTMESSAGE &&
						  strcmp(theMessage->msgHdr->theIndex.OutMke, "DQ") == 0 &&
						  strcmp(theMessage->msgHdr->theIndex.Dest_Dev, "NLETS") == 0
						 ) ||
					     (theMessage->msgHdr->theIndex.Event_Id == INPUTMESSAGE &&
						  strcmp(theMessage->msgHdr->theIndex.InMke, "DR") == 0 &&
						  strcmp(theMessage->msgHdr->theIndex.Src_Dev, "NLETS") == 0 
					   )))
					{	
						/* May have O/DQ or I/DR messages of interest */
						if (theMessage->msgHdr->theIndex.Event_Id == OUTPUTMESSAGE &&
							strstr(theMessage->msgData->msgBuffer,".CL."))
						{
							ProcessDqMessage(theMessage);
						}

						if (theMessage->msgHdr->theIndex.Event_Id == INPUTMESSAGE &&
							strstr(theMessage->msgData->msgBuffer,">CLLIC0000<"))
						{
							ProcessDrMessage(theMessage);
						}
					}

					// GET RID OF THE MESSAGE COPY

					FreeLemsLogMessage(theMessage);
				}
				CloseLemsLogFile (logFileHandle);
			}
		}
		FreeKeyList(&fileList);
		CreateCdlStats();			// This can be done for each daily directory
	}

	FreeKeyList(&dirList);

/*  Report the Statistics */

//	CreateCdlStats();				// This can be done for each daily directory

	PrintRespTimes(respfile);		// Create "rspfile.dat"
	PrintOriCounts(countfile);		// Create "cntfile.dat"

/*  Free the reporting KeyLists */

    FreeKeyList(&DQMsgList);
	FreeKeyList(&DRMsgList);
	FreeKeyList(&DRStatsList);
	FreeKeyList(&DRCountsList);
 
	exit(0);
}

#ifdef __cplusplus
}
#endif
