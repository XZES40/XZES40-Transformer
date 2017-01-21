// File: MkeUseStats.c
//
// Message Switch Custom Usage Count by MKE
//   (U) = User Messages
//   (S) = Database or System Messages
// 
// "MKE(U)",incount,outcount,ledsin,ledsout,total,"MKE-Description"
// "MKE(S)",incount,outcount,ledsin,ledsout,total,"MKE-Description"
//
// Template for LEMS-2000 and LEMS/JX Event Log Processing
//

#include "platform.h"

#include <time.h>
#include <stdio.h>

#include "keylist.h"
#include "msevlog.h"
#include "lemslog.h"

#include "btcomp.h"

#include "fmtlib.h"

#ifdef __cplusplus
extern 'C' {
#endif

/* Database Device Names */

const char * 
DBASE[] = {
	"NCIC","NLETS","WEATHERG","III","COMMAND","SOLR","LDB2","LDB3",
	"MARJ","EDISP","PTLD","CRDT","EJSO","RWS9","OTO9","RWT9","ODOJ",
	"ORMH","CLET","DMVT","EJSD","ALERT","LEDS","DMVP","DMVA","OBYF",
	"MUGS","DEADLETR","EJS1","EJS2","EJS3","EJS4","CLAS","CLAS2",
	"MSGMATCH",
	NULL};

typedef struct MkeInfo_s {
const char * Mke;
const char * CM;
const char * Desc;
} MKEINFO;

MKEINFO MkeTable[] = {
{"$.10","M","NCIC FINGER PRINT MATCHER OUT OF SERVICE"},
{"$.11","M","NCIC FINGER PRINT MATCHER IN FULL SERVICE"},
{"$.1","M","ALL NCIC FILES OUT OF SERVICE"},
{"$1","M","MANUAL NCIC HOT AND III FILES OUT OF SERVICE"},
{"$2","M","MANUAL NCIC HOT FILES IN SERVICE"},
{"$.2","M","NCIC HOT FILES IN FULL SERVICE"},
{"$.3","M","NCIC MESSAGE TRANSMISSION ERROR"},
{"$4","M","MANUAL NCIC HOT FILES RESTRICTED"},
{"$.4","M","NCIC HOT FILES IN RESTRICTED SERVICE"},
{"$5","M","MANUAL NCIC HOT FILES RESTRICTED CONTINUE"},
{"$.5","M","NCIC HOT FILES CONTINUING RESTRICTED SERVICE"},
{"$6","M","MANUAL NCIC HOT FILES AND III IN SERVICE"},
{"$.6","M","NCIC LINE REOPENED - ALL NCIC FILES IN FULL SERVICE"},
{"$7","M","MANUAL NCIC HOT FILES RESTRICTED"},
{"$.7","M","NCIC LINE REOPENED - ALL NCIC FILES IN RESTRICTED SERVICE"},
{"$.8","M","NCIC OUT-OF-STATE NOTIFICATION"},
{"AA","M","NLETS Amber Alert Message"},
{"ACQ","M","NLETS CVIS query on commercial carrier"},
{"ACR","M","NLETS CVIS query on commercial carrier - Response"},
{"ADDDB","C","ADD RECORD TO TABLE"},
{"AGY","M","LEDS Query Ageny - Retired - Use QAGY"},
{"ALERT","M","ALERT MESSAGE"},
{"ALQ","M","NLETS APCO Alarm Company Messages From Alarm Central Stations"},
{"ALR","M","NLETS APCO Alarm Company Messages From 911 PSAP Servers"},
{"AML","M","NLETS Administrative Broadcast - Law Enforcement"},
{"AM","M","NLETS and Oregon Administrative Message"},
{"$.A","M","NCIC-LEDS ADMINISTRATIVE MESSAGE"},
{"AQ","M","NLETS CHRI Supplemental CCH Manual Inquiry"},
{"ARC-ON","C","SCHEDULE ON-LINE EVENT LOG ARCHIVE"},
{"AR","M","NLETS CHRI Supplemental CCH Manual Inquiry - Response"},
{"ASCN","M","LEDS Article/Gun File Scan"},
{"AVQ","M","NLETS CVIS query on commercial vehicle"},
{"AVR","M","NLETS CVIS query on commercial vehicle - Response"},
{"BA","M","LEDS Oregon CCH Query - most resent arrest by SID"},
{"BATCH","C","TBD (SUPPORT FOR RETRIEVAL FROM TAPE)"},
{"BC","M","LEDS Oregon CCH Query - custody cycle by SID"},
{"BCQ","M","NLETS Bulk Cash Inquiry - Disabled 7/2014"},
{"BCR","M","NLETS Bulk Cash Inquiry - Response - Disabled 7/2014"},
{"BF","M","LEDS Oregon CCH Query - fingerprint card number"},
{"BH","M","LEDS Oregon CCH Query - (name dob soc mnu fbi)"},
{"$B","M","MANUAL LEDS IN FULL SERVICE"},
{"$.B","M","NCIC FILE-TRANSFER-READY NOTIFICATION -- LEDS IN FULL SERVICE"},
{"BQ","M","NLETS Boat Registration Inquiry"},
{"BRC","M","LEDS Oregon CCH Query - Redacted Rap Sheet by SID"},
{"BR","M","LEDS Oregon CCH Query -- NLETS Boat Registration Response"},
{"BSCF","M","LEDS Oregon CCH - Bureau Set Flags"},
{"CAA","M","NCIC 2000 Clear Consecutively Serialized Stolen Articles = LEDS(CSAA)"},
{"CA","M","NCIC 2000 Clear Stolen Article = LEDS(CSA)"},
{"CAQ","M","NLETS Canadian Stolen Article Inquiry"},
{"CAR","M","NLETS Canadian Stolen Article Inquiry - Response"},
{"CB","M","LEDS-NCIC Clear Stolen Boat"},
{"CBP1","M","NCIC clear stolen boat add-on parts"},
{"CBP2","M","NCIC clear stolen boat add-on parts"},
{"CBP3","M","NCIC clear stolen boat add-on parts"},
{"CBP4","M","NCIC clear stolen boat add-on parts"},
{"CBP5","M","NCIC clear stolen boat add-on parts"},
{"CBP6","M","NCIC clear stolen boat add-on parts"},
{"CBP7","M","NCIC clear stolen boat add-on parts"},
{"CBQ","M","NLETS Canadian Stolen Boat/Motor Inquiry"},
{"CBR","M","NLETS Canadian Stolen Boat/Motor Inquiry - Response"},
{"CBT","M","NCIC clear stolen boat - supplemental trailer"},
{"CCC","M","LEDS Corrections Client Hit Confirmation"},
{"CC","M","NCIC 2000 Clear Convicted Person on Supervised Release = LEDS(XPR)"},
{"CDBN","M","CLETS data inquiry - California Formatted Messages"},
{"CFG","M","LEDS-NCIC Clear Felony Gun"},
{"CF","M","LEDS-NCIC Clear Felony Vehicle"},
{"CFQ","M","NLETS Canadian Criminal History Rap Sheet Inquiry (FQ/FR)"},
{"CG","M","NCIC 2000 Clear Stolen Gun = LEDS(CSG)"},
{"CGQ","M","NLETS Canadian Stolen Gun Inquiry"},
{"CGR","M","NLETS Canadian Stolen Gun Inquiry - Response"},
{"CIB","M","LEDS Clear Impounded Boat"},
{"CIQ","M","NLETS Canadian Criminal History Index Inquiry (IQ/IR)"},
{"CLAA","M","LEDS-NCIC Clear Consecutiely Numbered Lost Articles"},
{"CLA","M","LEDS-NCIC Clear Lost Article"},
{"CLG","M","LEDS-NCIC Clear Lost Gun"},
{"CL","M","LEDS-NCIC Clear Stolen License Plate"},
{"CLOS","C","LOG OFF/CLOSE SESSION"},
{"CLOSE","C","LOG OFF/CLOSE SESSION"},
{"CLTE","M","CLETS Error Message"},
{"CLTR","M","CLETS Inquiry Response"},
{"$C","M","MANUAL LEDS OUT OF SERVICE"},
{"CM","M","LEDS-NCIC Clear Missing Person"},
{"$.C","M","NCIC REQUEST FOR VALIDATION NOTIFICATION -- LEDS OUT OF SERVICE"},
{"CP","M","LEDS-NCIC Clear Stolen Vehicle-Boat Part"},
{"CPO","M","NCIC Clear Protection Order = LEDS(XPO)"},
{"CPQ","M","NLETS Corrections Photo Inquiry"},
{"CPR","M","NLETS Corrections Photo Inquiry - Response"},
{"CQUE","C","DISPLAY LINE QUEUE COUNTS"},
{"CRBD","M","NCIC 2000 Enter Benefits Data - After a Clear Transaction"},
{"CRB","M","LEDS Clear Recovered Boat"},
{"CRG","M","LEDS-NCIC Clear Recovered Gun"},
{"CR","M","NLETS III Response from NCIC III Request"},
{"CSAA","M","LEDS Clear Consecutiely Numbered Stolen Articles = NCIC(CAA)"},
{"CSA","M","LEDS Clear Stolen Article = NCIC(CA)"},
{"CSG","M","LEDS Clear Stolen Gun = NCIC(CG)"},
{"CS","M","NCIC 2000 Clear Stolen Security"},
{"CSQ","M","NLETS Canadian Stolen Securities Inquiry"},
{"CSR","M","NLETS Canadian Stolen Securities Inquiry - Response"},
{"CSS","M","NCIC 2000 Clear Consecutively Serialized Stolen Securities"},
{"CSX","M","Clear Sexual Offender = NCIC(CXS) // USE LEDS(XSX)"},
{"CT","M","LEDS-NCIC Clear Temporary Felony Warrant"},
{"CTO","M","NCIC 2000 Clear Temporary Protection Order = LEDS(XPO)"},
{"CU","M","NCIC 2000 Clear Unidentified Person"},
{"CV","M","LEDS-NCIC Clear Stolen Vehicle"},
{"CW","M","LEDS-NCIC Clear Wanted Person"},
{"CWQ","M","NLETS Query Concealed Weapons Permits"},
{"CWR","M","NLETS Concealed Weapons Permits Response"},
{"CXS","M","NCIC 2000 Clear Sexual Offender = LEDS(XSX)"},
{"D07","M","DMV Blood Alcohol Inquiry (LC by license)(NC by name)"},
{"D13","M","DMV Oregon DMV - (Picture File Transaction)"},
{"D33","M","DMV Driving Restrictions J or 9 Inquiry"},
{"DALT","C","DISPLAY ALT ROUTE FOR DEVICE"},
{"DBR","M","Regional System Database Response"},
{"DCQUE","C","DISPLAY DEVICE QUEUE COUNTS"},
{"DDBST","C","DISPLAY DATA BASE STATUS"},
{"DDEV","C","DISPLAY DEVICE"},
{"DDST","C","DISPLAY DEVICE STATUS"},
{"DEA","M","NLETS NDPIX record entry transaction - Response"},
{"DEC","M","NCIC-III Deceased Person - Remove SID from Index Record"},
{"DELBF","C","DELETE SELECTED BATCH FILE"},
{"DELDB","C","DELETE RECORD FROM TABLE"},
{"DELDL","C","DELETE DISSEMINATION LOG"},
{"DEX","M","NLETS NDPIX record entry transaction"},
{"DEXX","M","NLETS NDPIX record entry transaction - Test"},
{"DFD","C","DISPLAY FUNCTION CODES AUTHORIZED FOR DEVICE"},
{"DFNC","C","DISPLAY FUNCTION CODES AUTHORIZED FOR DEVICE"},
{"DFO","C","DISPLAY FUNCTION CODES AUTHORIZED FOR ORI"},
{"DFST","C","DISPLAY FILE STATUS"},
{"DFU","C","DISPLAY FUNCTION CODES AUTHORIZED FOR USER"},
{"DGRP","C","DISPLAY DEVICES IN BROADCAST GROUP"},
{"DLP","M","DMV Driver Record (DLP.NC/NB) by name and dob"},
{"DLR","M","DMV Driver Record (DLR.LC) by operator license"},
{"DLST","C","DISPLAY LINE STATUS"},
{"DLW","M","Multiquery - DMV edit Driver Record Response - send QW to LEDS and NCIC"},
{"DMEM","C","DISPLAY BROADCAST GROUPS IN WHICH DEVICE IS A MEMBER"},
{"$.D","M","NCIC ORIGINATING-AGENCY-DUPLICATE-VEHICLE NOTIFICATION"},
{"DMVT","M","Maintenance - Send preformatted message to DMV test system"},
{"DNQ","M","NLETS Driver Inquiry by Name Only"},
{"DNR","M","NLETS Driver Inquiry by Name Only - Response"},
{"DORI","C","DISPLAY ORI USER INFORMATION"},
{"DPL","M","DMV Query Driver License Photo (LC by License)"},
{"DPP","M","DMV Disabled Placard Query (LC by license)(NC by name)(PN by id)"},
{"DQG","M","NLETS Driver Inquiry by Region"},
{"DQ","M","NLETS Driver Inquiry"},
{"DQUE","C","DUMP ENTRIES IN QUEUE FOR SPECIFIC LINE"},
{"DRG","M","NLETS Driver Inquiry by Region - Response"},
{"DR","M","NLETS Driver Inquiry - Response"},
{"DRR","M","NLETS NDPIX record renew transaction - Response"},
{"DRS","M","NCIC-III Remove SID from Index Record - Expunge"},
{"DRX","M","NLETS NDPIX record renew transaction"},
{"DRXX","M","NLETS NDPIX record renew transaction - Test"},
{"DTRM","C","DISPLAY TERMINAL RECORD"},
{"DTR","M","NLETS NDPIX request report transaction - Response"},
{"DTST","C","DISPLAY TERMINAL STATUS"},
{"DTX","M","NLETS NDPIX request report transaction"},
{"DTXX","M","NLETS NDPIX request report transaction - Test"},
{"DUA","M","NLETS NDPIX record update transaction - Response"},
{"DUX","M","NLETS NDPIX record update transaction"},
{"DUXX","M","NLETS NDPIX record update transaction - Test"},
{"EAA","M","NCIC 2000 Enter Consecutively Serialized Stolen Articles = LEDS(ESAA)"},
{"EAC","M","LEDS Oregon CCH - Enter Arrest Count Data"},
{"EAGY","M","LEDS Enter LEDS Agency Record"},
{"EA","M","NCIC 2000 Enter Stolen Article = LEDS(ESA)"},
{"EAN","M","LEDS Enter Special Purpose Article"},
{"EANP","M","LEDS Enter Special Purpose Article - Hold for Latents"},
{"EA-P","M","NCIC 2000 Enter Stolen Article - Hold for Latents = LEDS(ESAP)"},
{"EAS","M","LEDS Oregon CCH - Enter Arrest Segment"},
{"EB-A","M","LEDS-NCIC Enter Stolen Boat - Occupants Armed"},
{"EB-F","M","LEDS-NCIC Enter Stolen Boat - Occupants Armed - Hold for Latents"},
{"EB","M","LEDS-NCIC Enter Stolen Boat"},
{"EBP1","M","NCIC Enter Stolen Boat - Add-on Part"},
{"EBP2","M","NCIC Enter Stolen Boat - Add-on Part"},
{"EBP3","M","NCIC Enter Stolen Boat - Add-on Part"},
{"EBP4","M","NCIC Enter Stolen Boat - Add-on Part"},
{"EBP5","M","NCIC Enter Stolen Boat - Add-on Part"},
{"EBP6","M","NCIC Enter Stolen Boat - Add-on Part"},
{"EBP7","M","NCIC Enter Stolen Boat - Add-on Part"},
{"EB-P","M","LEDS-NCIC Enter Stolen Boat - Hold for Latents"},
{"EBS","M","NCIC Enter Stolen Boat - Supplemental Data"},
{"EBT","M","NCIC Enter Stolen Boat - Supplemental Trailer"},
{"ECAG","M","LEDS Enter LEDS Agency Contact"},
{"EC-C","M","NCIC 2000 Enter Convicted Person On Supervised Release - Caution = LEDS(EPRP)"},
{"ECHO","M","HQ Controller test messgae"},
{"EC","M","NCIC 2000 Enter Convicted Person On Supervised Release = LEDS(EPR)"},
{"ECN","M","NCIC 2000 Enter Probation or Supervised Release Status - Supplemental = LEDS(EN)"},
{"ECNS","M","NCIC 2000 Enter Probation or Supervised Release Status - Fraudulent Identifiers = LEDS(ENS)"},
{"ECS","M","LEDS Oregon CCH - custody segment entry mask"},
{"ECU","M","LEDS Oregon CCH - enter custody segment"},
{"EDLR","M","DLR Return To LEDS Crimevue - Address Acquire Utility"},
{"ED","M","NCIC 2000 Enter Supplemental Dental Data - Missing or Unidentified Person"},
{"EDP","M","NICS - Enter NICS Index Record"},
{"EF-A","M","LEDS-NCIC Enter Felony Vehicle - Occupants Armed"},
{"EF-F","M","LEDS-NCIC Enter Felony Vehicle - Occupants Armed - Hold for Latents"},
{"EFG","M","LEDS-NCIC Enter Felony Gun"},
{"EFGP","M","LEDS-NCIC Enter Felony Gun - Hold for Latents"},
{"EF","M","LEDS-NCIC Enter Felony Vehicle"},
{"EF-P","M","LEDS-NCIC Enter Felony Vehicle - Hold for Latents"},
{"EGG","M","NCIC 2000 Enter Criminal Gang"},
{"EGGN","M","NCIC 2000 Enter Criminal Gang or Terrorist Organization - Supplemental"},
{"EGGT","M","NCIC 2000 Enter Terrorist Organization"},
{"EGM","M","NCIC 2000 Enter Criminal Gang Member - Caution"},
{"EG","M","NCIC 2000 Enter Stolen Gun = LEDS(ESG)"},
{"EGMN","M","NCIC 2000 Enter Criminal Gang or Terrorist Organization Member - Supplemental"},
{"EGMT","M","NCIC 2000 Enter Terrorist Organization Member - Caution"},
{"EG-P","M","NCIC 2000 Enter Stolen Gun - Hold for Latents = LEDS(ESGP)"},
{"EH","M","LEDS Oregon CCH - Enter CCH Criminal History Record"},
{"EHN","M","LEDS Oregon CCH - Enter CCH supplemental identifiers"},
{"EIB","M","LEDS Enter Impounded Boat"},
{"EIDC","M","LEDS-NCIC Enter Identity Theft - Caution"},
{"EID","M","LEDS-NCIC Enter Identity Theft"},
{"EII","M","LEDS-NCIC Enter Investigative Interest Indicator"},
{"EIM","M","NCIC 2000 Enter Image"},
{"EIN","M","LEDS-NCIC Enter Identity Theft Supplemental"},
{"EIP","M","LEDS Enter Person of Interest (officer safety)"},
{"EIV","M","LEDS Enter Impounded Vehicle - by police"},
{"EIVP","M","LEDS Enter Impounded Vehicle - by private party"},
{"EJS","M","LEDS Oregon CCH - Enter CCH judicial (court) segment"},
{"EJSM","M","LEDS Oregon CCH - Enter CCH judicial (court) segment - Thru FOCUS"},
{"EJSO","M","LEDS Oregon CCH - Enter CCH automated court disposition"},
{"ELAA","M","LEDS-NCIC Enter Consecutively Serialized Lost Articles"},
{"ELA","M","LEDS-NCIC Enter lost article"},
{"EL-A","M","LEDS-NCIC Enter Stolen License Plate - Occupants Armed"},
{"EL-F","M","LEDS-NCIC Enter Stolen License Plate - Occupants Armed - Hold for Latents"},
{"ELG","M","LEDS-NCIC Enter Lost Gun"},
{"EL","M","LEDS-NCIC Enter Stolen License Plate"},
{"EL-P","M","LEDS-NCIC Enter Stolen License Plate - Hold For Latents"},
{"EMCC","M","LEDS Enter missing person - disaster victim = NCIC(EMVC)"},
{"EMC","M","LEDS Enter missing person - disaster victim = NCIC(EMV)"},
{"EMDC","M","LEDS-NCIC Enter Missing Person - Disability - Caution"},
{"EMD","M","LEDS-NCIC Enter Missing Person - Disability"},
{"EMEC","M","LEDS-NCIC Enter Missing Person - Endangered - Caution"},
{"EME","M","LEDS-NCIC Enter Missing Person - Endangered"},
{"EMIC","M","LEDS-NCIC Enter Missing Person - Involuntary - Caution"},
{"EMI","M","LEDS-NCIC Enter Missing Person - Involuntary"},
{"EMJC","M","LEDS-NCIC Enter Missing Person - Juvenile - Caution"},
{"EMJ","M","LEDS-NCIC Enter Missing Person - Juvenile"},
{"$.E","M","NCIC SERIOUS ERROR NOTIFICATION"},
{"EMN","M","LEDS-NCIC Enter Missing Person - Supplemental Data"},
{"EMOC","M","LEDS-NCIC Enter Missing Person - Other - Caution"},
{"EMO","M","LEDS-NCIC Enter Missing Person - Other"},
{"EMVC","M","NCIC 2000 Enter Missing Person - Catastrophe Victim - Caution = LEDS(EMCC)"},
{"EMV","M","LEDS Enter Missing Vehicle -- NCIC 2000 Enter Missing Person - Catastrophe Victim = LEDS(EMC)"},
{"ENAD","M","LEDS - Enter Person Supplemental Address"},
{"ENCRYPT","C","ENCRYPT PASSWORD"},
{"EN","M","LEDS-NCIC Enter Person Supplemental"},
{"ENPO","M","NCIC 2000 Enter Protection Order - Supplemental Data (NCIC Only) = LEDS(EN)"},
{"ENS","M","LEDS-NCIC Enter Person Supplemental - Fraudulent Identifiers"},
{"EO","M","NCIC 2000 Enter ORI"},
{"EPAG","M","LEDS Enter LEDS Agency Contact Phone"},
{"EPA","M","LEDS Enter Pawned Article"},
{"EPG","M","LEDS Enter Pawned Gun"},
{"EP","M","LEDS-NCIC Enter Stolen Vehicle-Boat Part"},
{"EPOC","M","LEDS-NCIC Enter Protection Order - Caution"},
{"EPO","M","LEDS-NCIC Enter Protection Order"},
{"EP-P","M","LEDS-NCIC Enter Stolen Vehicle-Boat Part - Hold for Latents"},
{"EPRC","M","LEDS Enter Corrections Client - Caution"},
{"EPR","M","LEDS Enter Corrections Client"},
{"EPRP","M","LEDS Enter Corrections Client - Predatory"},
{"EPS1","M","NCIC Enter Add-on Vehicle Part Record"},
{"EPS2","M","NCIC Enter Add-on Vehicle Part Record"},
{"EPS3","M","NCIC Enter Add-on Vehicle Part Record"},
{"EPS4","M","NCIC Enter Add-on Vehicle Part Record"},
{"EPS5","M","NCIC Enter Add-on Vehicle Part Record"},
{"EPS6","M","NCIC Enter Add-on Vehicle Part Record"},
{"EPS7","M","NCIC Enter Add-on Vehicle Part Record"},
{"EPT","M","LEDS Enter Prisoner Transport Record"},
{"ERA","M","LEDS Enter Recovered Article"},
{"ERB","M","LEDS Enter Recovered Boat"},
{"ERG","M","LEDS-NCIC Enter Recovered Gun"},
{"ER","M","NLETS ERROR MESSAGE"},
{"ERROR","M","DEFAULT ERROR MKE WHEN OUTPUT MKE IS UNDEFINED"},
{"ERV","M","LEDS Enter Recovered Vehicle"},
{"ESAA","M","LEDS Enter Consecutively Serialized Stolen Articles = NCIC(EAA)"},
{"ESA","M","LEDS Enter Stolen Article = NCIC(EA)"},
{"ESAP","M","LEDS Enter Stolen Article - Hold for Latents = NCIC(EA-P)"},
{"ESG","M","LEDS Enter Stolen Gun = NCIC(EG)"},
{"ESGP","M","LEDS Enter Stolen Gun - Hold for Latents = NCIC(EG-P)"},
{"ES","M","NCIC 2000 Enter Stolen Security"},
{"ESS","M","NCIC 2000 Enter Consecutively Serialized Stolen Securities"},
{"ESX","M","LEDS Enter Sexual Offender = NCIC(EXS)"},
{"ESXP","M","LEDS Enter Sexual Offender - Predatory = NCIC(EXSC)"},
{"ET-C","M","LEDS-NCIC Enter Temporary Felony Warrant - Caution"},
{"ETH","M","LEDS Enter LEDS Training History Record"},
{"ET","M","LEDS-NCIC Enter Temporary Felony Warrant"},
{"ETOC","M","NCIC 2000 Enter Temporary Protection Order - Caution = LEDS(EPOC)"},
{"ETO","M","NCIC 2000 Enter Temporary Protection Order = LEDS(EPO)"},
{"ETR","M","LEDS Enter LEDS base training record (limited)"},
{"EUD","M","NCIC 2000 Enter Unidentified Deceased Person"},
{"EUL","M","NCIC 2000 Enter Unidentified Living Person"},
{"EUN","M","NCIC 2000 Enter Unidentified Person - Supplemental"},
{"EUV","M","NCIC 2000 Enter Unidentified Catastrophe Victim"},
{"EV-A","M","LEDS-NCIC Enter Stolen Vehicle - Occupants Armed"},
{"EV-F","M","LEDS-NCIC Enter Stolen Vehicle - Occupants Armed - Hold for Latents"},
{"EV","M","LEDS-NCIC Enter Stolen Vehicle"},
{"EV-P","M","LEDS-NCIC Enter Stolen Vehicle - Hold for Latents"},
{"EVS1","M","NCIC Enter Add-on Vehicle Record"},
{"EVS2","M","NCIC Enter Add-on Vehicle Record"},
{"EW-C","M","LEDS-NCIC Enter Wanted Person - Caution"},
{"EWJC","M","LEDS-NCIC Enter Wanted Juvenile Person - Caution"},
{"EWJ","M","LEDS-NCIC Enter Wanted Juvenile Person"},
{"EW","M","LEDS-NCIC Enter Wanted Person"},
{"EXEC","C","Execute Unix Command"},
{"EXSC","M","NCIC 2000 Enter Sexual Offender - Caution = LEDS(ESXP)"},
{"EXS","M","NCIC 2000 Enter Sexual Offender = LEDS(ESX)"},
{"EXSN","M","NCIC 2000 Enter Sexual Offender - Supplemental = LEDS(EN)"},
{"EZ","M","LEDS Enter Investigative Locator"},
{"FCC","M","LEDS - Notify Local Agency of Federal Prisoner Release - used by federal ORIs-7G"},
{"$.F","M","NCIC FAILURE-TO-VALIDATE NOTIFICATION"},
{"FPQ","M","Interpol Wanted Person Full Inquiry"},
{"FPR","M","Interpol Wanted Person Full Response"},
{"FQC","M","NLETS IFTA International Fuel Tax Assoc Inquiry"},
{"FQ","M","NLETS CHRI Full CCH Record Inquiry"},
{"FRC","M","NLETS IFTA International Fuel Tax Assoc Response"},
{"FR","M","NLETS CHRI Full CCH Record Inquiry - Response"},
{"FT","M","NCIC 2000 File transfer decision"},
{"FTQ","M","Interpol Stolen Travel Document Full Inquiry"},
{"FTR","M","Interpol Stolen Travel Document Full Response"},
{"FVQ","M","Interpol Stolen Vehicle Full Inquiry"},
{"FVR","M","Interpol Stolen Vehicle Full Response"},
{"GDB-1","M","General Database MKE type 1"},
{"GDB-2","M","General Database MKE type 2"},
{"GDB-3","M","General Database MKE type 3"},
{"GETDBCON","C","Get database connection string"},
{"GETFILE","C","GETFILE - Retrieve File From LEMS To Console"},
{"GETPWD","C","Get Password for attaching xfer directory"},
{"GETVER","C","Get LEMS 2000 version number"},
{"$.G","M","NCIC VGTOF REFERENCE-GROUP-OWNERSHIP NOTIFICATION"},
{"GQ","M","NLETS FAA Aircraft Registration Inquiry"},
{"GR","M","NLETS FAA Aircraft Registration Inquiry - Response"},
{"$.H","M","NCIC ORI AGENCY NOTIFICATION - DELAYED HIT SYSTEM"},
{"HOLD","C","HOLD OUTPUTS TO DEVICE"},
{"HQ","M","NLETS Road and Weather Inquiry"},
{"HR","M","NLETS Road and Weather Inquiry - Response"},
{"HSL","M","NLETS Homeland Security Advisory - Law Enforcement"},
{"HS","M","NLETS Homeland Security Advisory"},
{"IAQ","M","NLETS INS (LESC) Inquiry"},
{"IAR","M","NLETS INS (LESC) Inquiry - Response"},
{"III","M","Maintenance - Send preformatted message to NCIC-III using 2L01 header."},
{"$.I","M","NCIC IMAGE OWNERSHIP NOTIFICATION"},
{"INIT-TP","C","INITIALIZE TAPE"},
{"IPO","M","NCIC 2000 Clear Expired Protection Order = LEDS(XPO)"},
{"IPQ","M","Interpol Wanted Person Inquiry"},
{"IPR","M","Interpol Wanted Person Response"},
{"IQ","M","NLETS CHRI Name CCH Index Inquiry"},
{"IR","M","NLETS CHRI Name CCH Index Inquiry - Response"},
{"ITO","M","NCIC 2000 Clear Expired Temporary Protection Order = LEDS(XPO)"},
{"ITQ","M","Interpol Stolen Travel Document Inquiry"},
{"ITR","M","Interpol Stolen Travel Document Response"},
{"IVQ","M","Interpol Stolen Vehicle Inquiry"},
{"IVR","M","Interpol Stolen Vehicle Response"},
{"IVS","M","LEDS Impounded Vehicle Scan"},
{"IXS","M","NCIC 2000 Cancel Expired Sexual Offender = LEDS(XSX)"},
{"$.J","M","NCIC EMANCIPATION NOTIFICATION"},
{"JQ","M","NLETS FAA Tracking Inquiry"},
{"JR","M","NLETS FAA Tracking Inquiry - Response"},
{"$.K","M","NCIC MISSING INFORMATION NOTIFICATION"},
{"KQ","M","NLETS Driver History Inquiry"},
{"KR","M","NLETS Driver History Inquiry - Response"},
{"LAA","M","NCIC 2000 Locate Consecutively Serialized Stolen Articles = LEDS(LSAA)"},
{"LACTIVAT","C","ACTIVATE LINE"},
{"LA","M","NCIC 2000 Locate Stolen Article = LEDS(LSA)"},
{"LB","M","LEDS-NCIC Locate Stolen Boat"},
{"LBP1","M","NCIC Locate Stolen Boat - Add-on Part"},
{"LBP2","M","NCIC Locate Stolen Boat - Add-on Part"},
{"LBP3","M","NCIC Locate Stolen Boat - Add-on Part"},
{"LBP4","M","NCIC Locate Stolen Boat - Add-on Part"},
{"LBP5","M","NCIC Locate Stolen Boat - Add-on Part"},
{"LBP6","M","NCIC Locate Stolen Boat - Add-on Part"},
{"LBP7","M","NCIC Locate Stolen Boat - Add-on Part"},
{"LBT","M","NCIC Locate Stolen Boat - Supplemental Trailer"},
{"LDB-1","M","LEDS/DB Hot File Response"},
{"LDB-2","M","LEDS/DB Unsolicited message to user terminal"},
{"LDB-4","M","LEDS/DB Mask response to LEMS/MS CRT"},
{"LDB-B","M","LEDS/DB CCH Response"},
{"LDB-C","M","LEDS/DB Inquiry to CLETS"},
{"LDB-D","M","LEDS/DB DMV Inquiry Response"},
{"LDB-F","M","LEDS/DB to NICS Firearms"},
{"LDB-H","M","LEDS/DB NCIC CCH Transaction (Type 2)"},
{"LDB-I","M","LEDS/DB III Response to NLETS"},
{"LDB-M","M","LEDS/DB DMV"},
{"LDB-N","M","LEDS/DB NCIC Hot File Transaction (Type 1) - NCIC Updates"},
{"LDB-Q","M","LEDS/DB NCIC Hot File Transaction (Type 1) - NCIC Inquiries"},
{"LDBS","M","Maintenance - Send preformatted message to LEDS CrimeVue"},
{"LDBST","C","LIST DATA BASE STATUS"},
{"LDB-T","M","LEDS/DB Hot File Error"},
{"LDG","C","LIST DEVICE GROUPS"},
{"LDO","C","LIST DEVICE ORIS"},
{"LE","M","NLETS Generic Database Entry"},
{"LEOFA","M","NLETS TSA Law Enforcement Offecer Flying Armed"},
{"LFG","M","LEDS-NCIC Locate Felony Gun"},
{"LF","M","LEDS-NCIC Locate Felony Vehicle"},
{"LFST","C","LIST FILE STATUS"},
{"LFUN","C","LIST FUNCTIONS"},
{"LGD","C","LIST GROUP DEVICES"},
{"LG","M","NCIC 2000 Locate Stolen Gun = LEDS(LSG)"},
{"LID","M","NCIC 2000 Transfer Image from MIU to Workstation Only (Return Error)"},
{"LISTBF","C","LIST BATCH FILES"},
{"LISTDL","C","RETRIEVE LIST OF DISSEMINATION LOGS"},
{"LISTSF","C","DISPLAY LIST OF SIMULATION FILES"},
{"LJAK","M","LOJACK-NLETS User Interface (LE..MA.LOJACK) - QUIET CHECK REPLY - radio control"},
{"LJC","M","LOJACK-NLETS Check - Radio Code (activate transmitter if stolen)"},
{"LJQ","M","LOJACK-NLETS Quiet - Radio Code (stop transmitter)"},
{"LJR","M","LOJACK-NLETS Reply - Radio Code (speedup transmitter if stolen)"},
{"LLA","M","LEDS Locate lost article"},
{"LLG","M","LEDS-NCIC Locate Lost Gun"},
{"LL","M","LEDS-NCIC Locate Stolen License Plate"},
{"LLST","C","LIST LINE STATUS"},
{"LM","M","LEDS-NCIC Locate Missing Person"},
{"$.L","M","NCIC LOCATE NOTIFICATION"},
{"LO","C","LIST ORIS"},
{"LOGOFF","C","LOG OFF/CLOSE SESSION"},
{"LOGON","C","LOG ON/OPEN SESSION"},
{"LOPR","C","LIST OPERATOR RECORDS"},
{"LORI","C","LIST ORIS"},
{"LP","M","LEDS-NCIC Locate Vehicle/Boat Part"},
{"LPS1","M","NCIC Locate Add-on Vehicle Part Record"},
{"LPS2","M","NCIC Locate Add-on Vehicle Part Record"},
{"LPS3","M","NCIC Locate Add-on Vehicle Part Record"},
{"LPS4","M","NCIC Locate Add-on Vehicle Part Record"},
{"LPS5","M","NCIC Locate Add-on Vehicle Part Record"},
{"LPS6","M","NCIC Locate Add-on Vehicle Part Record"},
{"LPS7","M","NCIC Locate Add-on Vehicle Part Record"},
{"LQ","M","NLETS Generic Database Inquiry"},
{"LQUE","C","LIST ENTRIES IN QUEUE FOR SPECIFIC LINE"},
{"LQUED","C","LIST ENTRIES IN QUEUE FOR SPECIFIC LINE"},
{"LRBD","M","NCIC 2000 Enter Benefits Data - After a Locate Transaction"},
{"LRG","M","LEDS-NCIC Locate Recovered Gun"},
{"LR","M","NLETS Generic Database Response"},
{"LSAA","M","LEDS Locate Consecutively Serialized Stolen Articles = NCIC(LAA)"},
{"LSA","M","LEDS Locate Stolen Article = NCIC(LA)"},
{"LSG","M","LEDS Locate Stolen Gun = NCIC(LG)"},
{"LS","M","NCIC 2000 Locate Stolen Security"},
{"LSS","M","NCIC 2000 Locate Consecutively Serialized Stolen Securities"},
{"LSUSPEND","C","SUSPEND LINE"},
{"LT","M","LEDS-NCIC Locate Temporary Felony Warrant"},
{"LTST","C","LIST DEVICES NOT LOGGED ON"},
{"LV","M","LEDS-NCIC Locate Stolen Vehicle"},
{"LVS1","M","NCIC Locate Add-on Vehicle Record"},
{"LVS2","M","NCIC Locate Add-on Vehicle Record"},
{"LW","M","LEDS-NCIC Locate Wanted Person"},
{"MAA","M","NCIC 2000 Modify Consecutively Serialized Stolen Articles = LEDS(MSAA)"},
{"MAGF","M","LEDS Mass Modify Article/Gun File"},
{"MAGY","M","LEDS Modify LEDS Agency Record"},
{"MALT","C","MODIFY ALT ROUTE"},
{"MA","M","NCIC 2000 Modify Single Stolen Article = LEDS(MSA)"},
{"MAR","M","LEDS Oregon CCH - modify arrest segment"},
{"MAS","M","LEDS Oregon CCH - get mask for modify arrest segment"},
{"MB","M","LEDS-NCIC Modify Stolen Boat"},
{"MBN","M","LEDS Oregon CCH - modify base name"},
{"MC","M","NCIC 2000 Modify Probation or Supervised Release Status = LEDS(MPR)"},
{"MCU","M","LEDS Oregon CCH - modify custody segment"},
{"MDBST","C","MODIFY DATA BASE STATUS"},
{"MD","M","NCIC 2000 Modify Missing-Unidentified Person - Dental"},
{"MDP","M","NICS - Modify NICS Index Record"},
{"MFG","M","LEDS-NCIC Modify Felony Gun"},
{"MF","M","LEDS-NCIC Modify Felony Vehicle"},
{"MFST","C","MODIFY FILE STATUS"},
{"MGG","M","NCIC 2000 Modify Criminal Gang or Terrorist Organization"},
{"MGM","M","NCIC 2000 Modify Criminal Gang or Terrorist Organization Member"},
{"MG","M","NCIC 2000 Modify Stolen Gun = LEDS(MSG)"},
{"MHID","M","LEDS Oregon CCH - modify identification data"},
{"MIB","M","LEDS Modify Impounded Boat"},
{"MID","M","LEDS-NCIC Modify Identity Theft"},
{"MII","M","NCIC 2000 Modify Image"},
{"MIP","M","LEDS Modify person of interest (officer safety)"},
{"MIV","M","LEDS Modify impounded vehicle (police)"},
{"MIVP","M","LEDS Modify impounded vehicle (private party)"},
{"MJS","M","LEDS Oregon CCH - get modify judicial segment mask"},
{"MJU","M","LEDS Oregon CCH - modify judicial count data"},
{"MLAA","M","LEDS-NCIC Modify Consecutively Serialized Lost Articles"},
{"MLA","M","LEDS-NCIC Modify Lost Article"},
{"MLG","M","LEDS-NCIC Modify Lost Gun"},
{"ML","M","LEDS-NCIC Modify Stolen License Plate"},
{"MMBX","M","LEDS Modify Parole Officer Mailbox on Common Records"},
{"MM","M","LEDS-NCIC Modify Missing Person"},
{"$.M","M","NCIC UNIDENTIFIED/MISSING PERSON MATCH NOTIFICATION"},
{"MMV","M","LEDS Modify Missing Vehicle"},
{"MODDB","C","MODIFY RECORD IN TABLE"},
{"MODSF","C","MODIFY SELECTED SIMULATION FILE"},
{"MO","M","NCIC 2000 Modify ORI Record - Agency Modifiable Fields"},
{"MPA","M","LEDS Modify pawned article"},
{"MPG","M","LEDS Modify pawned gun"},
{"MP","M","LEDS-NCIC Modify Stolen Vehicle-Boat Part"},
{"MPO","M","LEDS-NCIC Modify Protection Order"},
{"MPR","M","LEDS Modify Corrections Client"},
{"MPT","M","LEDS Modify Prisoner Transport Record"},
{"MQ","M","NLETS Hazardous Material Inquiry"},
{"MRA","M","LEDS Modify Recovered Article"},
{"MRB","M","LEDS Modify Recovered Boat"},
{"MRG","M","LEDS-NCIC Modify Recovered Gun"},
{"MR","M","NLETS Hazardous Material Inquiry - Response"},
{"MRS","M","NCIC-III Add SID to Index Record"},
{"MRV","M","LEDS Modify Recovered Vehicle"},
{"MSAA","M","LEDS Modify Consecutively Serialized Stolen Articles = NCIC(MAA)"},
{"MSA","M","LEDS Modify stolen article NCIC(MA)"},
{"MSG","M","LEDS Modify stolen gun = NCIC(MG)"},
{"MS","M","NCIC 2000 Modify Stolen Security"},
{"MSS","M","NCIC 2000 Modify Consecutively Serialized Stolen Securities"},
{"MSX","M","LEDS Modify Sexual Offender = NCIC(MXS)"},
{"MTH","M","LEDS Modify LEDS Training History Record"},
{"MT","M","LEDS-NCIC Modify Temporary Felony Warrant"},
{"MTO","M","NCIC 2000 Modify Temporary Protection Order = LEDS(MPO)"},
{"MTR","M","LEDS Modify LEDS Training Base Record"},
{"MU","M","NCIC 2000 Modify Unidentified Person"},
{"MV","M","LEDS-NCIC Modify Stolen Vehicle"},
{"MW","M","LEDS-NCIC Modify Wanted Person"},
{"MWXW","M","LEDS Modify EW to SW - Federal to State Warrant"},
{"MXS","M","NCIC 2000 Modify Sexual Offender = LEDS(MSX)"},
{"NAQ","M","NLETS NICB query all NICB files"},
{"NAR","M","NLETS NICB query all NICB files - Response"},
{"NCA","M","NLETS NICB cancel impound record - Response"},
{"NCI1","M","Maintenance - Send preformatted message to NCIC Legacy using 1L01 header."},
{"NCI2","M","Maintenance - Send preformatted message to NCIC 2000 using 1N01 header."},
{"NCIC-HOT","M","NCIC-HOT BYPASS: All message text after first dot sent to NCIC-HOT without further processing"},
{"NCIC-III","M","NCIC-III BYPASS: All message text after first dot sent to NCIC-III without further processing"},
{"NCIC","M","Maintenance - Send preformatted message to NCIC 2000 using 1N01 header."},
{"NCI","M","NLETS NICB cancel impound record"},
{"NCR","M","NCIC-HOT OR NCIC-III RESPONSE"},
{"NCT1","M","Maintenance - Send preformatted message to NCIC Test using TL01 header."},
{"NCT2","M","Maintenance - Send preformatted message to NCIC 2000 Test using TN01 header."},
{"NDN","M","NICS - Enter NICS Denial Notification"},
{"NDO","M","NICS - Enter NICS Denial Overturned Notification"},
{"NEA","M","NLETS NICB enter impound record - Response"},
{"NEI","M","NLETS NICB enter impound record"},
{"NFQ","M","NLETS Retrieve Full Text Message - Request"},
{"NFR","M","NLETS Response Retrieve Full Text Message"},
{"NICS","M","Maintenance - Send preformatted message to NCIC-NICS using 6L01 header"},
{"$.NICS","M","NICS SERVICE UP-DOWN NOTIFICATION"},
{"NIII","M","Maintenance - Send preformatted message to NCIC III using 2L01 header"},
{"NIQ","M","NLETS NICB query impound/export file only"},
{"NIR","M","NLETS NICB query impound/export file only - Response"},
{"NLETS","M","Maintenance - NLETS BYPASS: All message text after first dot sent to NLETS without further processin"},
{"NLN","M","NICS - Enter NICS Delay of Search Notification"},
{"NLQ","M","NLETS Search Random Access to NLETS Database (RAND)"},
{"NLR","M","NLETS Response Search Random Access to NLETS Database (RAND)"},
{"$.N","M","NCIC UNIDENTIFIED/MISSING PERSON NO MATCH NOTIFICATION"},
{"NPN","M","NICS - Enter NICS Proceed With Search Notification"},
{"NUA","M","NLETS NICB update impound record - Response"},
{"NUI","M","NLETS NICB update impound record"},
{"NWS","M","National Weather Service Message"},
{"$.O","M","NCIC CONVICTED PERSON ON SUPERVISED RELEASE NOTIFICATION"},
{"OPEN","C","LOG ON/OPEN SESSION"},
{"ORBYP","M","Oregon Bypass Database System"},
{"PAQ","M","NLETS Parole Probation Corrections - Inquiry"},
{"PAR","M","NLETS Parole Probation Corrections - Inquiry - Response"},
{"PBQ","M","NLETS Probation Inquiry"},
{"PBR","M","NLETS Probation Inquiry - Response"},
{"PCQ","M","NLETS Corrections Inquiry"},
{"PCR","M","NLETS Corrections Inquiry - Response"},
{"$.P","M","NCIC PURGE NOTIFICATION"},
{"PPQ","M","NLETS Parole Inquiry"},
{"PPR","M","NLETS Parole Inquiry - Response"},
{"PPT","M","LEDS Cancel Prisoner Transport Record - purge"},
{"PSCN","M","LEDS Vehicle and Vehicle Parts Scan - Purged Records"},
{"PUR-EV","C","PURGE EVENT LOG"},
{"PUR-ON","C","SCHEDULE ON-LINE EVENT LOG PURGE"},
{"QAB","M","NCIC 2000 Batch Query Articles (NCIC Only)"},
{"QAGY","M","LEDS Query LEDS Agency Record"},
{"QA","M","LEDS-NCIC Query Articles"},
{"QBB","M","NCIC 2000 Batch Query Stolen Boat (NCIC Only)"},
{"QB","M","LEDS-NCIC Query Stolen Boat (also repossessed and impounded)"},
{"QCD","M","CORD Query Oregon Corrections System Database"},
{"QCDT","M","CORD Query Oregon Corrections System Database - TEST SYSTEM"},
{"QCF","M","LEDS - Query Property Vehicles & Persons files by ORI  OCA = NCIC(QI)"},
{"QCLN","M","CLASS Query Clackamas CO Police Records Database"},
{"QCLT","M","CLASS Query Clackamas CO Police Records Database - TEST"},
{"QDP","M","NICS - Query NICS Record"},
{"QDW","M","Multiquery - DMV Driving Record + Wanted"},
{"QFC","M","LEDS Query Fingerprint Card - Arrest Segment"},
{"QGB","M","NCIC 2000 Batch Query Gun File (NCIC Only)"},
{"QGG","M","NCIC 2000 Query Criminal Gang or Terrorist Organization"},
{"QG","M","LEDS-NCIC Query Gun Files"},
{"QGM","M","NCIC 2000 Query Criminal Gang or Terrorist Organization Member"},
{"QHD","M","Multiquery - DMV Driving Record + Criminal History Check"},
{"QH","M","LEDS-NCIC III - Inquire Existence of CCH Index Code"},
{"QIC","M","NCIC 2000 Image Inquiry - Cold Search - Fingerprints"},
{"QID","M","LEDS Query BPSST file by identifier - NCIC Query ID Theft = LEDS(ZID)"},
{"QII","M","NCIC 2000 Image Inquiry - All Image Types"},
{"QI","M","NCIC 2000 One-Crime Inquiry Transaction - All Data From Selected FBI Files = LEDS(QCF)"},
{"QIS","M","NCIC 2000 Image Inquiry - Verification Fingerprint Match"},
{"QLDP","M","LEDS - Query NICS Record (CrimeVue Only)"},
{"QLN","M","LEDS - Query LEDS Only Brady Protection Orders (See NICS: QN)"},
{"QLW","M","LEDS - Query LEDS Only Persons Files (see QW)"},
{"QMAR","M","MARJ Query Oregon Medical Marijuana Registry"},
{"QMEN","M","ORMH Query Oregon Mental Health Database"},
{"QMENT","M","ORMHT Query Oregon Mental Health Database (TEST EE)"},
{"QM","M","LEDS-NCIC Query Missing Person - Nonunique Identifier Inquiry"},
{"$.Q","M","NCIC SEXUAL OFFENDER NOTIFICATION"},
{"Q","M","Query Vehicle Short Form"},
{"QN","M","NICS - Query NICS Record including Brady Protection Orders"},
{"QNP","M","NICS - Query NICS Records including all Protection Orders"},
{"QNR","M","NICS - Query NICS State Agency Record"},
{"QO","M","NCIC 2000 Query ORI Table - Full ORI Record"},
{"QPDB","M","PTLD Query Portland Records Database"},
{"QPO","M","NCIC 2000 Inquire on Protection Order (NCIC Only)"},
{"QPR","M","CORD Query EPR Mask Info from Corrections Database"},
{"QPT","M","LEDS Query Prisoner Transport Record"},
{"QPX","M","CORD Query EPR Mask Info from Corrections Database - TEST"},
{"QR","M","NCIC III - Request CCH record from FBI and participating States"},
{"QRS","M","Multiquery - Query vehicle/boat registry and stolen files (LC=LIC)(IC=VIN)(LB=BRN)(IB=BHN)"},
{"QRW","M","ODOT Get current Oregon State Police road conditions report .SP1 .SP2 .SP3 to .SP9"},
{"QSB","M","NCIC 2000 Batch Query Securities"},
{"QSLR","M","SOLR Query Test Database - CORD Prototype"},
{"QS","M","NCIC 2000 Query Securities"},
{"QSRV","C","RESTART QUEUE SERVICE"},
{"QTR","M","LEDS Query LEDS Training Record"},
{"QUEAD","C","ADD DEVICE TO QUEUE ENTRY"},
{"QUECP","C","CHANGE PRIORITY OF QUEUE ENTRY"},
{"QUEDD","C","DELETE DEVICE FROM QUEUE ENTRY"},
{"QUEDEALL","C","DELETE ALL ENTRIES FROM QUEUE"},
{"QUEDE","C","DELETE QUEUE ENTRY"},
{"QUEDELD","C","DELETE ALL QUES ON A LINE FOR A DEVICE"},
{"QUEDELL","C","DELETE ALL QUES ON A LINE"},
{"QUEGE","C","GET SINGLE QUEUE ENTRY"},
{"QU","M","NCIC 2000 Unidentified Person Inquiry"},
{"QVB","M","NCIC 2000 Batch Query Vehicle File (NCIC Only)"},
{"QVC","M","LEDS Check VIN Validity"},
{"QV","M","LEDS-NCIC Query Vehile File"},
{"QWB","M","NCIC 2000 Batch Query Persons Files (NCIC Only)"},
{"QWHD","M","Multiquery - DMV Driving Record + Wanted + Criminal History"},
{"QWH","M","Multiquery - Wanted Persons + Criminal History"},
{"QWI","M","NCIC 2000 Combined III and QW Inquiry (NCIC Only) = LEDS(QWH)"},
{"QW","M","LEDS-NCIC Query Persons Files - And Associated Vehicles"},
{"QX","M","LEDS DMV Title Check Vehicle Query"},
{"QXS","M","NCIC 2000 Query Sexual Offender (NCIC Only)"},
{"QZ","M","LEDS Query persons - investigation locator (eip with rtp/sus)"},
{"RALT","C","REMOVE ALT ROUTING"},
{"RA","M","LEDS Oregon CCH - query recent arrest record"},
{"RBED","M","NCIC 2000 Request Benefits and Effectiveness Analysis Report - For Previous Month"},
{"RDP","M","DMV Duplicate Photo Request (LC by OLN license) (NC by name)"},
{"REG","M","Vehicle/Boat Registrations (LC=LIC)(LB=BRN)(IC=VIN)(IB=BHN)"},
{"RELEASE","C","RELEASE DEVICE HOLD"},
{"REQUE","M","REQUE MESSAGE BY ISN OSN OR MSN - ISN AND OSN DEFAULT TO SOURCE DEVICE"},
{"RESTART","C","RESTART DEVICE"},
{"RETA","M","RETRIEVE BY ISN OSN MSN OR DATE RANGE"},
{"RET","M","RETRIEVE BY ISN OSN MSN OR DATE RANGE"},
{"RET-OFF","C","RETRIEVE FROM OFF-LINE EVENT LOG FOR DISPLAY"},
{"RET-ON","C","RETRIEVE FROM ON-LINE EVENT LOG FOR DISPLAY"},
{"RET-R","M","RETRIEVE COMMAND RESPONSE"},
{"RET-TAPE","C","TBD (SUPPORT FOR RETRIEVAL FROM TAPE)"},
{"RGW","M","Multiquery - Vehicle Registration - Supplemental Checks - QV and QWs and DLPs"},
{"$.R","M","NCIC TRANSMIT BENEFITS DATA NOTIFICATION"},
{"RNQ","M","NLETS Vehicle Registration by Name Only"},
{"RNR","M","NLETS Vehicle Registration by Name Only - Response"},
{"RPDQ","M","NCIC 2000 Request Error Trend Analysis Report - For Previous Month"},
{"RQG","M","NLETS Vehicle Region Registration Inquiry"},
{"RQ","M","NLETS Vehicle Registration Inquiry"},
{"RQS","M","NLETS Vehicle Registration + Stolen Check = NLETS(RQ) and LEDS(QV)"},
{"RRG","M","NLETS Vehicle Region Registration Inquiry - Response"},
{"RR","M","LEDS Oregon CCH - Request Rap Sheet -- NLETS Vehicle Registration Inquiry - Response"},
{"RSDU","M","NCIC 2000 Usage Statistics Request - For Previous Month"},
{"RTST","C","RESET TERMINAL STATUS"},
{"RW-C","M","LEDS Enter restricted county warraant (ICO/MUN) with cautions"},
{"RWI","M","REQUEST FOR WEATHER INFORMATION"},
{"RW","M","LEDS Enter restricted county warrant (ICO/MUN)"},
{"RWR","M","RESPONSE FOR REQUEST FOR WEATHER INFORMATION"},
{"SALT","C","SET ALT ROUTE"},
{"SCAN","C","SCAN QUEUES"},
{"SDP","M","NICS - Enter NICS Supplement Index Record"},
{"SELECT","C","SELECT EVENT LOG FOR DISPLAY"},
{"SEND","M","LEMS Send Message"},
{"SETPW","C","CHANGE PASSWORD"},
{"SF","C","DISPLAY GENERAL INFORMATION"},
{"SHRTDVST","C","SHORT DEVICE STATUS"},
{"SIM","C","SIMULATOR CONTROL"},
{"SM","M","NLETS STATUS MESSAGE"},
{"$.S","M","NCIC STATISTICS MODE NOTIFICATION"},
{"SON","M","NLETS Sex Offender Notification"},
{"SOQ","M","NLETS Sex Offender Registration Inquiry"},
{"SOR","M","NLETS Sex Offender Registration Inquiry - Response"},
{"SORT","C","SELECT EVENT LOG FOR DISPLAY"},
{"SPRQ","M","NCIC 2000 Global Inquiry - Restricted Use - perform searches of active or purged records"},
{"SQ","M","NLETS Snowmobile Registration Inquiry"},
{"SR","M","NLETS Snowmobile Registration Inquiry - Response"},
{"STATD","C","DEVICE STATUS"},
{"STATL","C","LINE STATUS"},
{"STATS","C","Statistics Command"},
{"STST","C","SET TERMINAL STATUS"},
{"SW-C","M","LEDS Enter State-wide Warrant (CIT/MIS/MUN/SFW) with cautions"},
{"SWI","M","SET WEATHER INFORMATION"},
{"SW","M","LEDS Enter State-wide Warrant (CIT/MIS/MUN/SFW)"},
{"SWQ","M","NLETS State Warrants Inquiry"},
{"SWR","M","NLETS State Warrants Inquiry - Response"},
{"TA","M","NLETS ORION add ORI transaction"},
{"TD","M","NLETS ORION delete ORI transaction"},
{"TERM","C","TERMINAL STATUS REQUEST"},
{"TI$","M","NCIC-III Generate Test $.A Messages From Index Test Records"},
{"TMON","C","ENABLE/DISABLE MONITOR"},
{"TQ","M","NLETS ORION inquire ORI transaction"},
{"TR","M","NLETS ORION transaction response"},
{"TU","M","NLETS ORION update ORI transaction"},
{"TXTBF","C","DISPLAY TEXT OF SELECTED BATCH FILE"},
{"TXTDL","C","RETRIEVE TEXT FROM DISSEMINATION LOG"},
{"TXTSF","C","DISPLAY TEXT OF SELECTED SIMULATION FILE"},
{"UCR","M","NCIC 2000 Enter UCR - NIBRS - National Incident-Based Reporting System Data"},
{"$U","M","MANUAL NCIC III FILES OUT OF SERVICE"},
{"$.U","M","NCIC III FILE OUT OF SERVICE"},
{"UQ","M","NLETS Canadian Driver License Inquiry"},
{"UR","M","NLETS Canadian Driver License Inquiry - Response"},
{"V74","M","DMV Vehicles Owned By Name"},
{"VHS","M","LEDS Vehicle Hot Sheet Scan - By County"},
{"$V","M","MANUAL NCIC III FILES IN FULL SERVICE"},
{"$.V","M","NCIC III FILE IN FULL SERVICE"},
{"VQ","M","NLETS Canadian Stolen Vehicle Inquiry"},
{"VR","M","NLETS Canadian Stolen Vehicle Inquiry - Response"},
{"VSCN","M","LEDS Vehicle and Vehicle Parts Scan - Current Records"},
{"WLQ","M","NLETS Query State Wildlife Violator Files"},
{"WLR","M","NLETS Query State Wildlife Violator Files - Response"},
{"$W","M","MANUAL NCIC III FILES RESTRICTED"},
{"$.W","M","NCIC III FILE IN RESTRICTED SERVICE"},
{"W","M","Wanted Person Query Short Form"},
{"WQ","M","NLETS Canadian Wanted Person Inquiry"},
{"WR","M","NLETS Canadian Wanted Person Inquiry - Response"},
{"XAA","M","NCIC 2000 Cancel Consecutively Serialized Stolen Articles = LEDS(XSAA)"},
{"XAGY","M","LEDS Cancel LEDS Agency Record"},
{"XA","M","NCIC 2000 Cancel Stolen Article = LEDS(XSA)"},
{"XAS","M","LEDS Oregon CCH - cancel arrest segment"},
{"XB","M","LEDS-NCIC Cancel Stolen Boat"},
{"XBP1","M","NCIC Cancel Stolen Boat - Add-on Part"},
{"XBP2","M","NCIC Cancel Stolen Boat - Add-on Part"},
{"XBP3","M","NCIC Cancel Stolen Boat - Add-on Part"},
{"XBP4","M","NCIC Cancel Stolen Boat - Add-on Part"},
{"XBP5","M","NCIC Cancel Stolen Boat - Add-on Part"},
{"XBP6","M","NCIC Cancel Stolen Boat - Add-on Part"},
{"XBP7","M","NCIC Cancel Stolen Boat - Add-on Part"},
{"XBT","M","NCIC Cancel Stolen Boat - Supplemental Trailer"},
{"XCAG","M","LEDS Cancel LEDS Agency Contact"},
{"XC","M","LEDS Oregon CCH - cancel arrest cycle -- NCIC 2000 Cancel Probation or Supervised Release=LEDS(XPR)"},
{"XCN","M","NCIC 2000 Cancel Probation or Supervised Release Status - Supplemental = LEDS(XN)"},
{"XCNS","M","NCIC 2000 Cancel Probation or Supervised Release Status - Fraudulent Identifiers = LEDS(XNS)"},
{"XCS","M","LEDS Oregon CCH - cancel custody segment"},
{"XD","M","NCIC 2000 Cancel Missing-Unidentified Person - Dental"},
{"XDP","M","NICS - Cancel NICS Record"},
{"XFG","M","LEDS-NCIC Cancel Felony Gun"},
{"XF","M","LEDS-NCIC Cancel Felony Vehicle"},
{"XGG","M","NCIC 2000 Cancel Criminal Gang or Terrorist Organization"},
{"XGGN","M","NCIC 2000 Cancel Criminal Gang or Terrorist Organization - Supplemental"},
{"XGM","M","NCIC 2000 Cancel Criminal Gang or Terrorist Organization Member"},
{"XG","M","NCIC 2000 Cancel Stolen Gun = LEDS(XSG)"},
{"XGMN","M","NCIC 2000 Cancel Criminal Gang or Terrorist Organization Member - Supplemental"},
{"XH","M","LEDS Oregon CCH - Cancel entire record by SID/DOB"},
{"XHN","M","LEDS Oregon CCH - Cancel supplemental identifiers"},
{"XHP","M","LEDS Oregon CCH - Cancel CCH entire record (80 yr plus purge)"},
{"XIB","M","LEDS Cancel Impounded Boat"},
{"XID","M","LEDS-NCIC Cancel Identity Theft"},
{"XII","M","LEDS-NCIC Cancel Investigative Interest Indicator"},
{"XIM","M","NCIC 2000 Cancel Image"},
{"XIN","M","LEDS-NCIC Cancel Identity Theft Supplemental"},
{"XIP","M","LEDS Cancel Person of Interest (officer safety)"},
{"XIV","M","LEDS Cancel Impounded Vehicle (police)"},
{"XIVP","M","LEDS Cancel Impounded Vehicle (private party)"},
{"XJS","M","LEDS Oregon CCH - cancel judicial segment"},
{"XLAA","M","LEDS-NCIC Cancel Consecutively Serialized Lost Articles"},
{"XLA","M","LEDS-NCIC Cancel Lost Article"},
{"XLG","M","LEDS-NCIC Cancel Lost Gun"},
{"XL","M","LEDS-NCIC Cancel Stolen License Plate"},
{"$X","M","MANUAL NCIC III FILES RESTRICTED CONTINUE"},
{"XM","M","LEDS-NCIC Cancel Missing Person"},
{"$.X","M","NCIC III FILE CONTINUING RESTRICTED SERVICE"},
{"XMN","M","LEDS-NCIC Cancel Missing Person - Supplemental Data"},
{"XMV","M","LEDS Cancel Missing Vehicle"},
{"XNAD","M","LEDS - Cancel Person Supplemental Address"},
{"XN","M","LEDS-NCIC Cancel Person Supplemental"},
{"XNPO","M","NCIC 2000 Cancel Protection Order - Supplemental Data (NCIC Only)"},
{"XNS","M","LEDS-NCIC Cancel Person Supplemental - Fraudulent Identifiers"},
{"XPAG","M","LEDS Cancel LEDS Agency Contact Phone"},
{"XPA","M","LEDS Cancel Pawned Article"},
{"XPG","M","LEDS Cancel Pawned Gun"},
{"XP","M","LEDS-NCIC Cancel Stolen Vehicle-Boat Part"},
{"XPO","M","LEDS-NCIC Cancel Protection Order"},
{"XPR","M","LEDS Cancel Corrections Client"},
{"XPS1","M","NCIC Cancel Add-On Vehicle Part Record"},
{"XPS2","M","NCIC Cancel Add-On Vehicle Part Record"},
{"XPS3","M","NCIC Cancel Add-On Vehicle Part Record"},
{"XPS4","M","NCIC Cancel Add-On Vehicle Part Record"},
{"XPS5","M","NCIC Cancel Add-On Vehicle Part Record"},
{"XPS6","M","NCIC Cancel Add-On Vehicle Part Record"},
{"XPS7","M","NCIC Cancel Add-On Vehicle Part Record"},
{"XPT","M","LEDS Cancel Prisoner Transport Record"},
{"XQ","M","NLETS Canadian Province Vehicle Registration Inquiry"},
{"XRA","M","LEDS Cancel Recovered Article"},
{"XRB","M","LEDS Cancel Recovered Boat"},
{"XRG","M","LEDS-NCIC Cancel Recovered Gun"},
{"XR","M","NLETS Canadian Province Vehicle Registration Response"},
{"XRV","M","LEDS Cancel Recovered Vehicle"},
{"XSAA","M","LEDS Cancel Consecutively Serialized Stolen Articles = NCIC(XAA)"},
{"XSA","M","LEDS Cancel Stolen Article = NCIC(XA)"},
{"XSG","M","LEDS Cancel Stolen Gun = NCIC(XG)"},
{"XS","M","NCIC 2000 Cancel Stolen Security"},
{"XSS","M","NCIC 2000 Cancel Consecutively Serialized Stolen Securities"},
{"XSX","M","LEDS Cancel Sexual Offender = NCIC(XXS)"},
{"XTH","M","LEDS Cancel LEDS Training History Record"},
{"XT","M","LEDS-NCIC Cancel Temporary Felony Warrant"},
{"XTO","M","NCIC 2000 Cancel Temporary Protection Order = LEDS(XPO)"},
{"XTR","M","LEDS Cancel LEDS Training Base Record"},
{"XU","M","NCIC 2000 Cancel Unidentified Person"},
{"XUN","M","NCIC 2000 Cancel Unidentified Person - Supplemental"},
{"XV","M","LEDS-NCIC Cancel Stolen Vehicle"},
{"XVS1","M","NCIC Cancel Add-on Vehicle Record"},
{"XVS2","M","NCIC Cancel Add-on Vehicle Record"},
{"XW","M","LEDS-NCIC Cancel Wanted Person"},
{"XXS","M","NCIC 2000 Cancel Sexual Offender = LEDS(XSX)"},
{"XXSN","M","NCIC 2000 Cancel Sexual Offender - Supplemental = LEDS(XN)"},
{"XZ","M","LEDS Cancel Investigative Locator"},
{"$Y","M","MANUAL NCIC III FILES IN SERVICE - HOT FILES RESTRICTED"},
{"$.Y","M","NCIC LINE REOPENED - NCIC III FULL SERVICE - HOT FILES RESTRICTED"},
{"YQ","M","NLETS Hit Confirmation Request"},
{"YR","M","NLETS Hit Confirmation Request - Response"},
{"ZA","M","NCIC 2000 Query Articles - Administrative (LEDS not checked)"},
{"ZB","M","NCIC 2000 Query Stolen Boat - Administrative (LEDS not checked)"},
{"ZE","M","NCIC 2000 Inquire Date and Time of Record Entry (LEDS not checked)"},
{"ZG","M","NCIC 2000 Query Gun File - Administrative (LEDS not checked)"},
{"ZID","M","NCIC 2000 Inquire Identity Theft - Administrative (LEDS not checked) = NCIC(QID)"},
{"ZI","M","NCIC-III Maintenance - Inquire Existence of FBI or SID - Returning Record"},
{"$Z","M","MANUAL NCIC III FILES RESTRICTED - HOT FILES IN FULL SERVICE"},
{"$.Z","M","NCIC LINE REOPENED - NCIC III RESTRICTED - HOT FILES IN FULL SERVICE"},
{"ZO","M","NCIC 2000 Query ORI Table - Agency Translation Only (LEDS not checked)"},
{"ZR","M","NCIC III - Inquire if FBI or SID is indexed in the III System - Returning Existence"},
{"ZRS","M","NCIC III - Inquire MSO SSO Status of Index Record - Returning $.A"},
{"ZS","M","NCIC 2000 Query Securities - Administrative (LEDS not checked)"},
{"ZV","M","NCIC 2000 Query Vehile File - Administrative (LEDS not checked)"},
{"ZW","M","NCIC 2000 Query Persons Files - Administrative (LEDS not checked)"},
{NULL,NULL,""}};

typedef struct counters_s {
	int INMSG;
	int OUTMSG;
	int INCVU;
	int OUTCVU;
} COUNTERS;

/* SUMMATION KEYLIST
 *
 * KeyListEntry Node
 *    next -> Next Mke(DU)
 *    k_name = Mke(DU) Name
 *    k_id   = Total Counter
 *    k_data = COUNTERS
*/

KeyListEntry * statsList = NULL;

// AddToCount - Accumulate counters to KeyList (statsList)
//
// @Param mke = MKE(U) for user message key, MKE(S) for database system message key
// @Param inout = message direction
// @Param cvue = true if crimevue in/out message
// @Return = 1 on failure, 0 on success

int AddToCount(const char * mke, char inout, int cvue )
{
	KeyListEntry * statsNode;
	KeyListEntry * tmpNode;

	if (statsList == NULL)
	{
		if (statsList = NewKeyListEntry())
		{
			if (SetKeyEntryName(statsList, mke) == NULL) 
				return 1; // memory allocation failure
			if (NewKeyEntryData(statsList, 1, sizeof(COUNTERS)) == NULL)
				return 1; // memory allocation failure
		}
		else
		{
			return 1;	// memory allocation failure
		}
	}

	// find or create Stats Node
	for (statsNode = statsList; statsNode; statsNode = statsNode->next)
	{
		tmpNode = statsNode;
		if (strcmp(statsNode->k_name, mke) == 0) break;
	}
	if (statsNode == NULL)
	{
		if ((tmpNode->next = NewKeyListEntry()) == NULL) 
			return 1;	// memory allocation failure
		statsNode = tmpNode->next;
		if (SetKeyEntryName(statsNode, mke) == NULL)
			return 1;	// memory allocation failure
		if (NewKeyEntryData(statsNode, 1, sizeof(COUNTERS)) == NULL)
			return 1;	// memory allocation failure
	}

	// Increment the counters
	statsNode->k_id++;
	if ((inout == 'I') || (inout == 'C'))
	{
		((COUNTERS *) (statsNode->k_data))->INMSG++;
		if (cvue)
			((COUNTERS *) (statsNode->k_data))->INCVU++;
	}
	if (inout == 'O')
	{
		((COUNTERS *) (statsNode->k_data))->OUTMSG++;
		if (cvue)
			((COUNTERS *) (statsNode->k_data))->OUTCVU++;
	}
	return 0;	// success
}

KeyListEntry	* dirlist = NULL;

KeyListEntry	* fileList = NULL;

//KeyListEntry	* msgList = NULL;

struct tm	startDateTime;

struct tm	endDateTime;

char outFile[256];

// Select Messages to Count,
// Return 1 on system failure

int FindMkeId(const char * mke)
{
	int i;
	for (i = 0;MkeTable[i].Mke; i++)
	{
		if (strcmp(MkeTable[i].Mke,mke) == 0) break;
	}
	return i;
}

int IsDbase(const char * device)
{
	int i;
	for (i = 0; DBASE[i]; i++)
	{
		if (strcmp(DBASE[i], device) == 0) break;
	}
	return (DBASE[i] != NULL);
}

int SelectMessage(const LemsLogMessageStruct * theMessage)
{
	char reportMKE[16];
	int i;

	if (INPUTEVENT(theMessage->msgHdr->theIndex.Event_Id))
	{
		i = FindMkeId(theMessage->msgHdr->theIndex.InMke);
		if (MkeTable[i].Mke == NULL) return 0; // Nothing to report
		strcpy(reportMKE, MkeTable[i].Mke);
		if (IsDbase(theMessage->msgHdr->theIndex.Src_Dev))
			strcat(reportMKE, "(S)");
		else
			strcat(reportMKE, "(U)");
		return AddToCount(reportMKE, 'I', (strcmp(theMessage->msgHdr->theIndex.Src_Dev, "LEDS") == 0));
	}
	else if (OUTPUTEVENT(theMessage->msgHdr->theIndex.Event_Id))
	{
		i = FindMkeId(theMessage->msgHdr->theIndex.OutMke);
		if (MkeTable[i].Mke == NULL) return 0; // Nothing to report
		strcpy(reportMKE, MkeTable[i].Mke);
		if (IsDbase(theMessage->msgHdr->theIndex.Dest_Dev))
			strcat(reportMKE, "(S)");
		else
			strcat(reportMKE, "(U)");
		return AddToCount(reportMKE, 'O', (strcmp(theMessage->msgHdr->theIndex.Dest_Dev, "LEDS") == 0));
	}
	return 0;	// Nothing to report
}

main(int argc, char * argv[])
{

	char  startDTime[22];  // = "2009-02-04 13:00:00";
	char  endDTime[22];    // = "2009-02-05 08:59:59";
	char  lemsHome[50];    // = "C:\\lems2k";
	char  eventlogs[50];   // = "BackEvents";

	FILE * oFile;

	struct tm logtime_tm;
	time_t logtime_sec;

	char * evFileName = NULL;
	char * evDirName = NULL;

	KeyListEntry	* dirList = NULL;
	KeyListEntry	* fileList = NULL;

	KeyListEntry	* DListPtr = NULL;
	KeyListEntry	* FListPtr = NULL;

	if (argc != 6)
	{
		printf("\nUSAGE: %s LEMSHOME EVENTDIR STARTDTIME ENDDTIME OUTFILE\n", argv[0]);
		exit(1);
	}
	strcpy(lemsHome, argv[1]);
	strcpy(eventlogs, argv[2]);
	strcpy(startDTime, argv[3]);
	strcpy(endDTime, argv[4]);
	strcpy(outFile, lemsHome);
	strcat(outFile, "\\xfer\\");
	strcat(outFile, argv[5]);

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
//				printf("\nLEMS LOG FILE: %s\\%s\n", evDirName, evFileName);

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

					if ((tmcomp(&logtime_tm, &startDateTime) >= 0) &&
						(tmcomp(&logtime_tm, &endDateTime) <= 0))
					{
						SelectMessage(theMessage);
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

	if (!statsList)	exit(0);	// nothing to report

	// Sort our findings
	statsList = KeyListNameSort(statsList);

	// Create output report file

	oFile = fopen(outFile, "w");
	if (oFile == NULL) exit(1) ; //fatal - cannot open report file

	// Print the stats
	{
		KeyListEntry * kPtr;
		char trimMKE[16];
		
		fprintf(oFile, "Message Switch MKE Usage Report\n");
		fprintf(oFile, "FROM: %s\n", startDTime);
		fprintf(oFile, "TO: %s\n", endDTime);
		fprintf(oFile, "\nUsage records are tab delimited for spreadsheet import.\n");
		fprintf(oFile, "\nMKE\tINMSG\tOUTMSG\tINCVU\tOUTCVU\tTOTAL\tDESCRIPTION\n");

		for (kPtr = statsList; kPtr; kPtr = kPtr->next)
		{
			char * p;
			int i;
			strcpy(trimMKE, kPtr->k_name);
			if (p = strchr(trimMKE, '('))
				*p = '\0';
			i = FindMkeId(trimMKE);
			fprintf(oFile, "%s\t%d\t%d\t%d\t%d\t%d\t%s\n",
				kPtr->k_name,
				((COUNTERS *) (kPtr->k_data))->INMSG,
				((COUNTERS *) (kPtr->k_data))->OUTMSG,
				((COUNTERS *) (kPtr->k_data))->INCVU,
				((COUNTERS *) (kPtr->k_data))->OUTCVU,
				kPtr->k_id,
				MkeTable[i].Desc);
		}
		// output some blank lines
		fprintf(oFile, "\n\nMessages With CrimeVue Component\n");
		fprintf(oFile, "\nMKE\tINMSG\tOUTMSG\tINCVU\tOUTCVU\tTOTAL\tDESCRIPTION\n");
		// create filtered output for CrimeVue I/O messages
				for (kPtr = statsList; kPtr; kPtr = kPtr->next)
		{
			char * p;
			int i;
			strcpy(trimMKE, kPtr->k_name);
			if (p = strchr(trimMKE, '('))
				*p = '\0';
			i = FindMkeId(trimMKE);
			if ((((COUNTERS *) (kPtr->k_data))->INCVU) ||
				(((COUNTERS *) (kPtr->k_data))->OUTCVU))
			{
				fprintf(oFile, "%s\t%d\t%d\t%d\t%d\t%d\t%s\n",
					kPtr->k_name,
					((COUNTERS *) (kPtr->k_data))->INMSG,
					((COUNTERS *) (kPtr->k_data))->OUTMSG,
					((COUNTERS *) (kPtr->k_data))->INCVU,
					((COUNTERS *) (kPtr->k_data))->OUTCVU,
					kPtr->k_id,
					MkeTable[i].Desc);
			}
		}
	}

	fflush(oFile);
	fclose(oFile);

	// Cleanup

	FreeKeyList(&statsList);
	exit(0);
}

#ifdef __cplusplus
}
#endif
