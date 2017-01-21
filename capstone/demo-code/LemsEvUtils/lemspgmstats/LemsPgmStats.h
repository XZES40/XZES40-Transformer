

/* File: LemsPgmStats.h  --  8/24/2011  --  Steven J. Hathaway */

/**
 * CUSTOM MESSAGE SWITCH STATISTICS 
 *
 * Statistics by Program Type (Transaction Counts, Not Message Counts).
 *
 * Group By Month, Program Type, User Type
 *
 */

#ifndef LEMS_PGM_STATS_H
#define LEMS_PGM_STATS_H

#include "platform.h"
#include "lemslog.h"
#include "keylist.h"

/* ORI Search Text from LemsAgyType.c */

extern const char * ATTY_list;
extern const char * CORR_list;
extern const char * COURT_list;
extern const char * MEDEX_list;
extern const char * ODOT_list;
extern const char * FHOUS_list;
extern const char * ORAGY_list;

//extern KeyListEntry * theCounters = NULL;

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
	  const char * UserType);

/* LogDateText()
 * Convert LogDate to "Mon CCYY" text
 */

const char * LogDateText(const time_t *LogDate);

/* SortTheCounters()
 * The top list (theCounters) is already sorted by month.
 * The first sublist is being sorted by project name
 * The second sublist is being sorted by user-type name
 */

void SortTheCounters(void);

/* Create the CSV Spreadsheet File */

FILE * OpenFileCsv(char * filename);

/* Close the CSV Spreadsheet File */

int CloseFileCsv(FILE * csvfile);

/* Write the CSV Spreadsheet Report */

int WriteCsvReport(FILE* csvfile);

/* QUERIES TO LEDS
 *   From ID Bureau
 *   From DMV
 *   From CLETS
 *   From NLETS
 *   From Users
 *   From NCIC ($.A.CHR)
 */

int ChQ2Leds(LemsLogMessageStruct * theMessage);

/* UPDATES TO LEDS
 *   From ID Bureau
 *   From DMV
 *   From NCIC
 *   From EJSO
 *   From Users
 */

int ChU2Leds(LemsLogMessageStruct * theMessage);

/* QUERIES TO NCIC
 *   From LEDS/DB
 *   From DMV
 *   From CLETS
 *   From Users
 */

int ChQ2Ncic(LemsLogMessageStruct * theMessage);

/* UPDATES TO NCIC
 *   From ID Bureau
 *   From Users
 *   From LEDS/DB
 */

int ChU2Ncic(LemsLogMessageStruct * theMessage);

/* QUERIES+UPDATES TO NICS
 *   From ID Bureau
 *   From Users
 *   From LEDS
 */

int ChQU2Nics(LemsLogMessageStruct * theMessage);

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

int ChQR2Dmv(LemsLogMessageStruct * theMessage);

/* QUERIES TO DMV PHOTOS
 *   From Users
 *   From NLETS
 */

int ChQP2Dmv(LemsLogMessageStruct * theMessage);

/* QUERIES TO NLETS
 *   From Users
 *   From DMV
 */

int ChQ2Nlets(LemsLogMessageStruct * theMessage);

/* QUERIES TO OMMP
 *   From Users (not counting retries and multiples)
 */

int ChQ2ommp(LemsLogMessageStruct * theMessage);

/* QUERIES TO ORMH MENTAL HEALTH
 *   From ID Bureau
 *   From Users
 */

int ChQ2ormh(LemsLogMessageStruct * theMessage);

/* QUERIES to COR-PHOTO
 *   From Users
 *   From NLETS
 */

int ChQ2Mugs(LemsLogMessageStruct * theMessage);

/* QUERIES TO PTLD-DB
 *   From Users
 */

int ChQ2Ptld(LemsLogMessageStruct * theMessage);

/* QUERIES TO CORD-DB
 * From Users
 */

int ChQ2Cord(LemsLogMessageStruct * theMessage);

/* QUERIES TO CLACKAMAS-DB
 *   From Users
 */

int ChQ2Clas(LemsLogMessageStruct * theMessage);

/* QUERIES TO SUN/RAIN-DB (QSRN)
 *   From Users
 */

int ChQ2SunRain(LemsLogMessageStruct * theMessage);

/* QUERIES TO DMV-HELP
 *   From Users
 *   From NLETS
 */

int Ch2DmvHelp(LemsLogMessageStruct * theMessage);

/* ADMIN MESSAGES (AM, AML, YQ, YR, HS, AA, $.A, CCC, FCC)
 *   From Users
 *   From DMV-HELP
 *   From NLETS
 *   From LEDS/DB ($.A)
 *   From NCIC ($.x) not ($.A.CHR)
 */

int Ch4AM(LemsLogMessageStruct * theMessage);

/* WEATHER MESSAGES (HR)
 *   From NLETS
 */

int Ch4Weather(LemsLogMessageStruct * theMessage);

/* HOMELAND SECURITY MESSAGES (HS, HSL)
 *   From NLETS
 *   From LEDS-CTR
 *   From Users
 */

int Ch4HSec(LemsLogMessageStruct * theMessage);

/* OTHER SUMMARY TALLY
 */

int OtherSums(LemsLogMessageStruct * theMessage);

#endif /* LEMS_PGM_STATS_H*/