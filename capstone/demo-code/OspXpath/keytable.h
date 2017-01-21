
#ifndef OSP_XPATH_TABLE_LIB_H
#define OSP_XPATH_TABLE_LIB_H


#include "keylist.h"


#if defined(__cplusplus)
extern "C"
{
#endif


extern KeyListEntry * theXpathTableList;

void initXpathTableDB(void);

void dropXpathTableDB(void);

int setXpathTableKeyValue(const char * table, const char * key, const char * value);

const char * getXpathTableKeyValue(const char * table, const char * key);

int getXpathTableRowCount(const char * table);

int getXpathTableKeyRow(const char * table, const char * key);

const char * getXpathTableRowKey(const char * table, int row);

const char * getXpathTableRowValue(const char * table, int row);

int compareKeyDataStrings(const KeyListEntry * node1, const KeyListEntry * node2);

void sortXpathTableKeys(const char * table);

void sortXpathTableValues(const char * table);

void invertXpathTable(const char * table);

void dropXpathTableName(const char * table);

void dropXpathTableKey(const char * table, const char * key);

void dropXpathTableRow(const char * table, int row);

/* dumpKeyTableToFile
 *
 * The value returned is the filename of the temporary XML file
 * The returned filename is allocated via malloc()
 * The caller must call removeKeyTableFile() to cleanup resources
*/

char * dumpKeyTableToFile(const char * table);

/* removeKeyTableFile
 *
 * The temporary files are removed and the TempFileName is deallocated.
*/
void removeKeyTableFile(char * TempFileName);

#if defined(__cplusplus)
}
#endif

#endif	/* OSP_XPATH_TABLE_LIB_H */

