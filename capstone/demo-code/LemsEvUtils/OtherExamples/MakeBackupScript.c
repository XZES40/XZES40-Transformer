/*
 * Make a command file to use XCOPY for network backup activities
 *
 * The command is to be run from the eventlog directory
 *
 * This is usable only for Oregon LEMS/JX "ORLEDSMS" Message Switch
 * Running on Windows Server 2003.  The 2008 Server has deprecated XCOPY.
 *
 * Updated:    Dec 21 2009                     Hathaway
 * Place logfile updates directly into [10.70.0.12]\\BackEvents instead of
 * a subdirectory therein for additional manual intervention.  The automated off-site
 * process to the State Data Center (TSM) is now operational.  The ability for the
 * operations center to perform log searches on [10.70.0.12] is confirmed operational.
 * The old BONES [10.30.0.140] server is having a hardware fan problem.
 *
 * Updated:    Aug 26 2011                     Hathaway
 * Remove old BONES system from being a backup repository of evtlog files.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

char * weekday[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

char * lemsdir = "%LEMS%";

int main(int argc, char *argv[])
{
	time_t	epoc;
	struct tm dateTm;

	char today[11];

	epoc = time(NULL);

	memcpy((void *) &dateTm, (void *) localtime(&epoc), sizeof(struct tm));

	sprintf(today, "%4.4d-%2.2d-%2.2d", dateTm.tm_year + 1900, dateTm.tm_mon + 1, dateTm.tm_mday);

	printf("CD %s\\database\\mssql\\Backup\r\n", lemsdir);
	printf("XCOPY LEMS2000_db_*.BAK \\\\ORLEDSMSD\\Tina-Backup\\lems2k\\database\\MSSQL\\Backup /M /E /F /Y >>%s\\xfer\\xcopy-%s.log\r\n", lemsdir, today);

	printf("CD %s\\Batch\r\n", lemsdir);
	printf("XCOPY Batch*Splits.txt \\\\ORLEDSMSD\\Tina-Backup\\lems2k\\Batch /M /E /F /Y >>%s\\xfer\\xcopy-%s.log\r\n", lemsdir, today);
	printf("XCOPY Batch*Splits.txt*-out \\\\ORLEDSMSD\\Tina-Backup\\lems2k\\Batch /M /E /F /Y >>%s\\xfer\\xcopy-%s.log\r\n", lemsdir, today);
	printf("ERASE /Q Batch_*-ack\r\n");
	printf("ERASE /Q Batch_*-rsp\r\n");
	printf("ERASE /Q Batch_*-out\r\n");

	printf("CD %s\\Eventlogs\r\n", lemsdir);
//	printf("XCOPY *.* \\\\BONES\\Backevents /A /E /F /Y >>%s\\xfer\\xcopy-%s.log\r\n", lemsdir, today);

//	printf("XCOPY *.* \\\\10.70.0.12\\Backevents\\Restore /M /E /F /Y >>%s\\xfer\\xcopy-%s.log\r\n", lemsdir, today);
	printf("XCOPY *.* \\\\10.70.0.12\\Backevents /M /E /F /Y >>%s\\xfer\\xcopy-%s.log\r\n", lemsdir, today);

	printf("CD %s\\%s\r\n", weekday[dateTm.tm_wday], today);

	printf("ATTRIB +A *.log\r\n");

	printf("CD %s\\xfer\r\n", lemsdir);

	printf("COPY xcopy-%s.log \\\\ORLEDSMS\\UDS130\r\n", today);

//	printf("EXIT\r\n");

	exit(0);

}
