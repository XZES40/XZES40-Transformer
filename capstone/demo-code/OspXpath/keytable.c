
#include "keylist.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__cplusplus)
extern "C"
{
#endif

/*
 * Memory Allocation Note:
 * All allocated memory for these functions is owned by theXpathTableList.
 * These functions do take ownership of memory from the calling routines.
 *
 * These functions are expected to be called from XALAN-C Extension XPath functions.
 * There is separation of memory allocation management between the OSP Xpath KeyTables
 * library and the Apache XERCES-C memory allocation mechanisms used by XALAN-C.
 * The Apache XERCES-C and XALAN-C are coded in 'C++' and support a separate plug-in
 * memory management capability.  The OSP Xpath KeyTables library is coded in 'C' and
 * uses the malloc() memory management as maintained by the OSP Keylist library.
 */

KeyListEntry * theXpathTableList = NULL;

/**
 *  theXpathTableList -> First Xpath Table
 *     next ->  The next Xpath Table
 *     k_name -> The Xpath Table Name
 *     k_list -> The rows in the Xpath Table
 *        next -> The next Xpath Table Row
 *        k_id = row index integer (maintained sequential from 1)
 *        k_name -> Name of unique Xpath Table Key
 *        k_data -> The string representation of "value" associated with the Table Key
 */

void	initXpathTableDB(void)
{
	if (theXpathTableList)
		FreeKeyList(&theXpathTableList);
	return;
}

void	dropXpathTableDB(void)
{
	if (theXpathTableList)
		FreeKeyList(&theXpathTableList);
	return;
}

// returns 1 on memory allocation failure
// returns zero on success

int setXpathTableKeyValue(const char * table, const char * key, const char * value)
{
	KeyListEntry * TblPtr;
	KeyListEntry * RcdPtr;
	int	i;

	if (!table || !key || !value)
		return 0;

	if (!theXpathTableList)
	{	// create first table entry
		if (!(theXpathTableList = NewKeyListEntry()))
			return 1;		// memory allocation failure
		if (!(SetKeyEntryName(theXpathTableList, table)))
			return 1;		// memory allocation failure
	}

	for (TblPtr = theXpathTableList; TblPtr && TblPtr->next && strcmp(TblPtr->k_name, table);
		TblPtr = TblPtr->next);

	if (strcmp(TblPtr->k_name, table))
	{	// need new table entry
		if (!(TblPtr = TblPtr->next = NewKeyListEntry()))
			return 1;		// memory allocation failure
		if (!(SetKeyEntryName(TblPtr, table)))
			return 1;		// memory allocation failure
	}

	if (!TblPtr->k_list)
	{	// no key entries in the table
		if (!(RcdPtr = TblPtr->k_list = NewKeyListEntry()))
			return 1;		// memory allocation failure
		if (!(SetKeyEntryName(RcdPtr, key)))
			return 1;		// memory allocation failure
		if (!(NewKeyEntryData(RcdPtr, 1, strlen(value) +1)))
			return 1;		// memory allocation failure
		strcpy((char *) RcdPtr->k_data, value);
		RcdPtr->k_id = 1;		// first index value is 1
		return 0;			//  return success
	}

	for (RcdPtr = TblPtr->k_list; RcdPtr->next && strcmp(RcdPtr->k_name, key);
		RcdPtr = RcdPtr->next);

	if (strcmp(RcdPtr->k_name, key))
	{	// need new key entry in the table
		if (!(RcdPtr = RcdPtr->next = NewKeyListEntry()))
			return 1;		// memory allocation failure
		if (!(SetKeyEntryName(RcdPtr, key)))
			return 1;
	}

	// get buffer for value data
	if (!NewKeyEntryData(RcdPtr, 1, strlen(value) +1))
		return 1;			// memory allocation failure

	// and copy the value (i.e. UTF-8) null-terminated-octet string
	strcpy((char *)RcdPtr->k_data, value);

	// recompute the row identifiers
	for (i=1, RcdPtr = TblPtr->k_list; RcdPtr; i++, RcdPtr = RcdPtr->next)
		RcdPtr->k_id = i;

	return 0;
}	

const char * getXpathTableKeyValue(const char * table, const char * key)
{
	KeyListEntry * TblPtr;
	KeyListEntry * RcdPtr;

	for (TblPtr = theXpathTableList; TblPtr && strcmp(TblPtr->k_name, table);
		TblPtr = TblPtr->next);
	if (!TblPtr)
		return "";

	for (RcdPtr = TblPtr->k_list; RcdPtr && strcmp(RcdPtr->k_name, key);
		RcdPtr = RcdPtr->next);
	if (!RcdPtr)
		return "";

	return (const char *) RcdPtr->k_data;
}


int getXpathTableRowCount(const char * table)
{
	KeyListEntry * KPtr;

	for (KPtr = theXpathTableList; KPtr && strcmp(KPtr->k_name, table);
		KPtr = KPtr->next);
	if (!KPtr)
		return 0;

	for (KPtr = KPtr->k_list; KPtr && KPtr->next; KPtr = KPtr->next);

	if (!KPtr)
		return 0;

	return KPtr->k_id;
}


int getXpathTableKeyRow(const char * table, const char * key)
{
	KeyListEntry * TblPtr;
	KeyListEntry * RcdPtr;

	for (TblPtr = theXpathTableList; TblPtr && strcmp(TblPtr->k_name, table);
		TblPtr = TblPtr->next);
	if (!TblPtr)
		return 0;

	for (RcdPtr = TblPtr->k_list; RcdPtr && strcmp(RcdPtr->k_name, key);
		RcdPtr = RcdPtr->next);
	if (!RcdPtr)
		return 0;

	return RcdPtr->k_id;
}


const char * getXpathTableRowKey(const char * table, int row)
{
	KeyListEntry * TblPtr;
	KeyListEntry * RcdPtr;

	for (TblPtr = theXpathTableList; TblPtr && strcmp(TblPtr->k_name, table);
		TblPtr = TblPtr->next);
	if (!TblPtr)
		return "";

	for (RcdPtr = TblPtr->k_list; RcdPtr && (RcdPtr->k_id != row);
		RcdPtr = RcdPtr->next);
	if (!RcdPtr)
		return "";

	return RcdPtr->k_name;
}


const char * getXpathTableRowValue(const char * table, int row)
{
	KeyListEntry * TblPtr;
	KeyListEntry * RcdPtr;

	for (TblPtr = theXpathTableList; TblPtr && strcmp(TblPtr->k_name, table);
		TblPtr = TblPtr->next);
	if (!TblPtr)
		return "";

	for (RcdPtr = TblPtr->k_list; RcdPtr && (RcdPtr->k_id != row);
		RcdPtr = RcdPtr->next);
	if (!RcdPtr)
		return "";

	return (const char *) RcdPtr->k_data;
}


void dropXpathTableName(const char * table)
{
	KeyListEntry * KPtr;

	if (!theXpathTableList)
		return;			// nothing to drop

	if (!strcmp(theXpathTableList->k_name, table))
	{
		theXpathTableList = DropKeyListEntry(theXpathTableList);	// top entry is theTable
		return;
	}

	for (KPtr = theXpathTableList; KPtr->next && strcmp(KPtr->next->k_name, table);
		KPtr = KPtr->next);

	if (KPtr->next && !strcmp(KPtr->next->k_name, table))
		KPtr = DropKeyListEntry(KPtr->next);	// drop the table

	return;
}


void dropXpathTableKey(const char * table, const char * key)
{
	KeyListEntry * TblPtr;
	KeyListEntry * RcdPtr;
	int i;

	for (TblPtr = theXpathTableList; TblPtr  && strcmp(TblPtr->k_name, table);
		TblPtr = TblPtr->next);

	if (!TblPtr || !TblPtr->k_list)
		return;			// nothing to drop

	if (!strcmp(TblPtr->k_list->k_name, key))
	{
		TblPtr->k_list = DropKeyListEntry(TblPtr->k_list);
		for (RcdPtr = TblPtr->k_list, i = 1; RcdPtr; i++, RcdPtr = RcdPtr->next)
			RcdPtr->k_id = i;
		return;
	}

	for (RcdPtr = TblPtr->k_list; RcdPtr->next && strcmp(RcdPtr->next->k_name, key);
		RcdPtr = RcdPtr->next);

	if (RcdPtr->next && !strcmp(RcdPtr->next->k_name, key))
		RcdPtr->next = DropKeyListEntry(RcdPtr->next);

	for (RcdPtr = TblPtr->k_list, i = 1; RcdPtr; i++, RcdPtr = RcdPtr->next)
		RcdPtr->k_id = i;

	return;
}


void dropXpathTableRow(const char * table, int row)
{
	KeyListEntry * TblPtr;
	KeyListEntry * RcdPtr;
	int i;

	for (TblPtr = theXpathTableList; TblPtr  && strcmp(TblPtr->k_name, table);
		TblPtr = TblPtr->next);

	if (!TblPtr || !TblPtr->k_list)
		return;			// nothing to drop

	if (TblPtr->k_list->k_id == row)
	{
		TblPtr->k_list = DropKeyListEntry(TblPtr->k_list);
		for (RcdPtr = TblPtr->k_list, i = 1; RcdPtr; i++, RcdPtr = RcdPtr->next)
			RcdPtr->k_id = i;
		return;
	}

	for (RcdPtr = TblPtr->k_list; RcdPtr->next && RcdPtr->next->k_id != row;
		RcdPtr = RcdPtr->next);

	if (RcdPtr->next && RcdPtr->next->k_id == row)
		RcdPtr->next = DropKeyListEntry(RcdPtr->next);

	for (RcdPtr = TblPtr->k_list, i = 1; RcdPtr; i++, RcdPtr = RcdPtr->next)
		RcdPtr->k_id = i;

	return;
}


int compareKeyDataStrings(const KeyListEntry * node1, const KeyListEntry * node2)
{
	//DEBUG VISIBILITY
	const unsigned char * data1;
	const unsigned char * data2;

	if (!node1 || !node1->k_data || !node2 || !node2->k_data)
		return 0;

	data1 = (const unsigned char *) node1->k_data;
	data2 = (const unsigned char *) node2->k_data;

	return strcmp((const unsigned char *) node1->k_data, 
		(const unsigned char *) node2->k_data);

}

void sortXpathTableKeys(const char * table)
{
	KeyListEntry * TblPtr;
	KeyListEntry * RcdPtr;
	int i;

	for (TblPtr = theXpathTableList; TblPtr && strcmp(TblPtr->k_name, table);
		TblPtr = TblPtr->next);

	if (!TblPtr || !TblPtr->k_list)
		return;			// nothing to sort

	TblPtr->k_list = KeyListNameSort(TblPtr->k_list);

	for (RcdPtr = TblPtr->k_list, i = 1; RcdPtr; i++, RcdPtr = RcdPtr->next)
		RcdPtr->k_id = i;

	return;
}


void sortXpathTableValues(const char * table)
{
	KeyListEntry * TblPtr;
	KeyListEntry * RcdPtr;
	int i;

	for (TblPtr = theXpathTableList; TblPtr && strcmp(TblPtr->k_name, table);
		TblPtr = TblPtr->next);

	if (!TblPtr || !TblPtr->k_list)
		return;			// nothing to sort

	TblPtr->k_list = KeyListSort(TblPtr->k_list, &compareKeyDataStrings);

	for (RcdPtr = TblPtr->k_list, i = 1; RcdPtr; i++, RcdPtr = RcdPtr->next)
		RcdPtr->k_id = i;

	return;
}


void invertXpathTable(const char * table)
{
	KeyListEntry * TblPtr;
	KeyListEntry * RcdPtr;
	int i;

	for (TblPtr = theXpathTableList; TblPtr && strcmp(TblPtr->k_name, table);
		TblPtr = TblPtr->next);

	if (!TblPtr || !TblPtr->k_list)
		return;			// nothing to sort

	TblPtr->k_list = KeyListReverseOrder(TblPtr->k_list);

	for (RcdPtr = TblPtr->k_list, i = 1; RcdPtr; i++, RcdPtr = RcdPtr->next)
		RcdPtr->k_id = i;

	return;
}


/* dumpKeyTableToFile
 *
 * The content of a named table is saved to a temporary file so it can be
 * read by the XERCES XML parser using the XALAN Source Tree Constructors.
 *
 * The value returned is the filename of the temporary XML file
 * The returned filename is allocated via malloc()
 * The caller must call removeKeyTableFile() to cleanup resources.
 *
 * The keytable.xsd is a schema definition for the temporary file format.
 * The keytable.dtd is a doctype definition for the temporary file format.
 *
 Example:
 <ktb:table xmlns:ktb='http://www.oregon.gov/OSP/CJIS/xml/keytable'
        file='temporary-filename'  table='table-name'>
    <ktb:row table='table-name' row='1'
	    key='key-string' value='value-string/>
    <ktb:row table='table-name' row='2'
	    key='key-string' value='value-string/>
	<ktb:row table='table-name' row='3'
	    key='key-string' value='value-string/>
 </ktb:table>
 *
 * The corresponding XPATH extension is based on C++ Class: FunctionDocument
 * in the Apache XALAN-C library.  See: OspXpathFunctionLib.hpp
*/

char * dumpKeyTableToFile(const char * table)
{
	KeyListEntry * TPtr;
	KeyListEntry * RPtr;
	FILE * PFXHNDL;
	FILE * XMLHNDL;

	char * TmpFileName = NULL;

	char * TmpDirName = NULL;	

	char * TmpXmlFileName = NULL;

	if (!table)
		return NULL;				// no table name to test

	if (!strlen(table))
		return NULL;				// no table name to test

	for (TPtr = theXpathTableList; TPtr && strcmp(TPtr->k_name, table); TPtr = TPtr->next);
	if (!TPtr)
	{
		return NULL;				// virtual table not found
	}

	if (!(TmpDirName = getenv("TMPXMLDIR")))		// our tempdir may take precedence
	{
		if (!(TmpDirName = getenv("TMPDIR")))
		{
			if (!(TmpDirName  = getenv("TEMP")))
			{
				if (!(TmpDirName = getenv("TMP")))
				{
					return NULL;	// no temporary directory found
				}
			}
		}
	}

	if (!(TmpFileName = tempnam(TmpDirName, "KTB")))
	{
		return NULL;				// no usable filename string
	}

	/* Create the  temporary prefix file to claim the name */

	if (!(PFXHNDL = fopen(TmpFileName, "wb+")))
	{
		return NULL;
	}
	fclose(PFXHNDL);				// prefix tempfilename is created

	if (!(TmpXmlFileName = malloc(strlen(TmpFileName) + 5)))
	{
		return NULL;				// memory allocation failure
	}

	strcpy(TmpXmlFileName, TmpFileName);
	strcat(TmpXmlFileName, ".xml");
	free(TmpFileName);				// tempnam() uses malloc() allocation

	if (!(XMLHNDL = fopen(TmpXmlFileName, "wb+")))
	{
		return NULL;				// cannot create temporary XML file
	}

	fprintf(XMLHNDL, "<?xml version='1.0' encoding='UTF-8' ?>\n");
	fprintf(XMLHNDL, "<ktb:table xmlns:ktb='http://www.oregon.gov/OSP/CJIS/xml/keytable' ");
	fprintf(XMLHNDL, "table='%s' file='%s'>\n", table, TmpXmlFileName);

	for (RPtr = TPtr->k_list; RPtr; RPtr = RPtr->next)
	{
		fprintf(XMLHNDL,"<ktb:row table='%s' row='%d' key='%s' value='%s'/>\n",
		table, RPtr->k_id, (unsigned char *) RPtr->k_name, (unsigned char *) RPtr->k_data);
	}
	
	fprintf(XMLHNDL, "</ktb:table>\n");
	fflush(XMLHNDL);
	fclose(XMLHNDL);

	return TmpXmlFileName;
}

/* removeKeyTableFile
 *
 * The temporary files are removed and the TempFileName is deallocated.
*/

void removeKeyTableFile(char * TempFileName)
{
	if (TempFileName)
	{
		size_t	len;
		len = strlen(TempFileName);
		remove(TempFileName);
		if (len > 4)
		{
			len -= 4;
			TempFileName[len] = '\0';	// remove .xml suffix
			remove(TempFileName);
		}
		free(TempFileName);
	}
}


#if defined(__cplusplus)
}
#endif


