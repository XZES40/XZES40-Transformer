
#include "keylist.h"
#include "keytable.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//
// Dump KeyTable to Temporary XML File Returning Filename
// The filename is allocated with malloc() and must be destroyed with free();
// The calling application will also want to delete the Temporary (prefix.xml) file
// and delete the Temporary (prefix) file.
//




int main(int argc, char * argv[])
{
	int rtn = 0;
	char * TempFileName;

	initXpathTableDB();
	rtn = setXpathTableKeyValue("MYTABLE","KEY-A","VALUE-A");
	if (rtn)
		exit(rtn);
	rtn = setXpathTableKeyValue("MYTABLE","KEY-B","VALUE-B");
	if (rtn)
		exit(rtn);
	rtn = setXpathTableKeyValue("MYTABLE","KEY-C","VALUE-C");
	if (rtn)
		exit(rtn);
	rtn = setXpathTableKeyValue("MYTABLE","KEY-D","VALUE-D");
	if (rtn)
		exit(rtn);
	rtn = setXpathTableKeyValue("MYTABLE","KEY-E","VALUE-E");
	if (rtn)
		exit(rtn);
	rtn = setXpathTableKeyValue("MYTABLE","KEY-F","VALUE-F");
	if (rtn)
		exit(rtn);
	rtn = setXpathTableKeyValue("MYTABLE","KEY-G","VALUE-G");
	if (rtn)
		exit(rtn);
	rtn = setXpathTableKeyValue("MYTABLE","KEY-H","VALUE-H");
	if (rtn)
		exit(rtn);
	rtn = setXpathTableKeyValue("MYTABLE","KEY-I","VALUE-I");
	if (rtn)
		exit(rtn);
	rtn = setXpathTableKeyValue("MYTABLE","KEY-J","VALUE-J");
	if (rtn)
		exit(rtn);
	rtn = setXpathTableKeyValue("MYTABLE","KEY-K","VALUE-K");
	if (rtn)
		exit(rtn);
	rtn = setXpathTableKeyValue("MYTABLE","KEY-L","VALUE-L");
	if (rtn)
		exit(rtn);
	rtn = setXpathTableKeyValue("MYTABLE","KEY-M","VALUE-M");
	if (rtn)
		exit(rtn);

	TempFileName = dumpKeyTableToFile("MYTABLE");
	printf("Temp XML File = %s\n", TempFileName);

	removeKeyTableFile(TempFileName);

	exit(rtn);
}

