/*
 * THIS PROGRAM UPDATES THE MAPPING VALUES FOR LEMS-CONSOLE GRAPHIC
 */

#include <stdio.h>
#include <string.h>
#include "dirlist.h"
#include "keylist.h"

KeyListEntry * fileList = NULL;

enum {
	mapx = 1,
	mapy = 2,
	mapw = 3,
	maph = 4,
	map  = 5 };

typedef struct mapid_str {
	int	id;
	char * tag;
} mapid_str_t;

mapid_str_t mapid_tbl[] = {
	{1, "mapx"},
	{2, "mapy"},
	{3, "mapw"},
	{4, "maph"},
	{5, "map"},
	{0, NULL}};

int findmapid(char * source)
{
	int i;
	int idx = 0;
	char * tstr;
	char * p;

    for (i = 0; mapid_tbl[i].id != 0; i++)
	{
		if (tstr=strstr(source, mapid_tbl[i].tag))
		{
			for (p = source; p != tstr; p++)
			{
				if (!strchr(" \t\r\n", *p))
					return 0;
			}
			if (p == tstr)
				return mapid_tbl[i].id;
			return 0;
		}
	}
	return 0;
}

void doUpdateIni(FILE * fInp, FILE * fOut)
{
	char buffer[1000];
	char * p;
	int keyid;

	// Read a line into buffer and return its pointer

	while (p = fgets(buffer, 1000, fInp))
	{
		keyid = findmapid(p);
		switch (keyid)
		{
			case mapx:
				fprintf(fOut,"# %s", p);
				fprintf(fOut,"mapx = %d\n", 1100);// __1100__);
				break;
			case mapy:
				fprintf(fOut,"# %s", p);
				fprintf(fOut,"mapy = %d\n", 120);//__120__);
				break;
			case maph:
				fprintf(fOut,"# %s", p);
				fprintf(fOut,"maph = %d\n", 2476);//__2476__);
				break;
			case mapw:
				fprintf(fOut,"# %s", p);
				fprintf(fOut,"mapw = %d\n", 2700);//__2700__);
				break;
			case map:
				fprintf(fOut,"# %s", p);
				fprintf(fOut,"map = %s\n", "ORmapfile.bmp");//__mapfile.bmp__);
				break;
			default:
				fprintf(fOut,"%s", p);
		}
	}
}

void addFileName(const char * fileName)
{
	KeyListEntry * Kptr;
	if (!fileList)
	{
		fileList = NewKeyListEntry();
		SetKeyEntryName(fileList, fileName);
		return;
	}
	Kptr = FindLastKeyListEntry(fileList);
	Kptr->next = NewKeyListEntry();
	SetKeyEntryName(Kptr->next, fileName);
	return;
}


int main(int argc, char * argv[])
{
	DIRHANDLE	hDir;
	int found = 1;
	char lookupFileName[MAXPATHLEN +2];
	char tempFileName[MAXPATHLEN +2];

	FILE * iniFile;
	FILE * tmpFile;

	KeyListEntry * KPtr;

	if (argc != 2)
		exit(1);

	sprintf(lookupFileName,"%s\\*.ini", argv[1]);
	sprintf(tempFileName,"%s\\upd_ini.tmp", argv[1]);

	for (hDir = FindFirstDirEntry(lookupFileName); hDir && found; 
		found = FindNextDirEntry(hDir))
	{
		if (DirentIsFile(hDir))	// foreach *.ini file
		{
			addFileName(DirentGetFullPath(hDir));
		}
	}
	CloseFindDir(hDir);

	for (KPtr = fileList; KPtr; KPtr = KPtr->next)
	{
		// iniFile = Open(iniFileName,"r+");
		// tmpFile = Open(tempFileName,"w+");

		iniFile = fopen(GetKeyEntryName(KPtr), "r");
		tmpFile = fopen(tempFileName, "w");

		// Do the update(iniFile, tmpFile);
		doUpdateIni(iniFile, tmpFile);

		// Close(iniFile);
		// Close(tmpFile);

		fclose(iniFile);
		fflush(tmpFile);
		fclose(tmpFile);

		// iniFile = oldfile, tmpFile = newfile
		remove(GetKeyEntryName(KPtr));
		rename(tempFileName, GetKeyEntryName(KPtr));
	}

	// Delete tempFileName if file was created

	exit(0);
}

