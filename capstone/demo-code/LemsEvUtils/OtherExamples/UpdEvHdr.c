


#include "dirlist.h"
#include <stdio.h>
#include <string.h>

typedef struct lems_ev_log_hdr
{
	char	Lems2000[12];		// Message Switch Product Name: "LEMS 2000"
	char	Version[7];			// Message Switch Software Version:
								//   i.e "2.3", "3.0.2", "3.2.2", 3.2.4", "3.3" "3.4"
	char	FileName[16];		// Log File Name: "/EVnnn_n.log"
	char	CreationDate[19];	// Log File Create Date Time: "CCYY-MM-DD HH:MM"
								//   Version 3.4 has seconds: "CCYY-MM-DD HH:MM:SS"
	char	CreatingPgm[11];	// Program Name without extension, with SID appended,
								//   and truncated to 11 characters "ioclient148"
	char	ProgramVersion[7];	// null characters.
} LemsEvLogHdrType;


int main(int argc, char * argv[])
{
	DIRHANDLE theDirH;
	FILE	* theFile;
	LemsEvLogHdrType	evHdr;
	size_t	rtn;
	char	* cptr;
	char	fName[260];

	for (theDirH = FindFirstDirEntry("EV*.log"); theDirH;)
	{

		if (DirentIsFile(theDirH))
		{
			strcpy(fName,DirentGetFileName(theDirH));

			if (theFile = fopen(fName, "rb+"))
			{
				fseek(theFile, 0, SEEK_SET);
				rtn = fread(&evHdr, sizeof(evHdr), 1, theFile);
				cptr = evHdr.FileName;
				if ((*cptr == '/') || (*cptr == '\\'))
					cptr++;
				strcpy(cptr, fName);
				fseek(theFile, 19, SEEK_SET);
				fwrite(evHdr.FileName, 16, 1, theFile);
				fclose(theFile);
			}

		}
		if (!FindNextDirEntry(theDirH))
		{
			CloseFindDir(theDirH);
			break;
		}
	}
	exit (0);
}

