/*
 * 'C' Source Library - Common Utilities - Supported by GNU and Windows VC7.1
 *
 * Copyright (c) 2008 - This is open source software in the pulbic domain.
 *
 * Developed by: Steven J. Hathaway
 *
 * For:  State of Oregon - Oregon State Police / CJIS-LEDS
 */


#include "dirlist.h"

#include <stdio.h>

#if defined (_WIN32)
#include <winerror.h>
#endif
#if defined (__GNUC__)
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#endif

/* ================================================================
 * FileNameDirEntryTest
 *
 * Return TRUE if f_name is satisfied by f_test string.
 * Microsoft does its own testing with FindNextFile.
 * GNU uses this to test directory entry names.
 */

int FileNameDirEntryTest(
	const char * f_name,		/* source filename */
	const char * f_test)		/* test string with asterisks and questions wild */
{
	size_t fn;
	size_t ft;
	char *pn, *pt, *ptr, *ptr2;
	char name[NAME_MAX +1];
	char test[NAME_MAX +1];
	char tkn;
	int found;

	if (!f_name || !f_name[0])
		return 0;
	if (!f_test || !f_test[0])
		return 1;
	
	fn = strlen(f_name);
	ft = strlen(f_test);

	if (ft == 1 && f_test[0] == '*')
		return 1;

	pn = strncpy(name, f_name, NAME_MAX);
	name[NAME_MAX] = '\0';
	pt = strncpy(test, f_test, NAME_MAX);
	test[NAME_MAX] = '\0';

	for (found = 0; ;)
	{
		if (!pt[0] && !pn[0])
		{
			found = 1;
			break;
		}

		if (!pt[0])
			break;

		if (!pn[0])
		{
			if (pt[0] == '*' && !pt[1])
				found = 1;
			break;
		}
		
		if (pt[0] != '*')
		{
			if (pt[0] == '?' || pn[0] == pt[0])
			{
				pn++;
				pt++;
				continue;
			}
			break;
		}
		if (!pt[1])
		{
			found = 1;
			break;
		}
		pt++;
		if (ptr = strpbrk(pt, "*?"))
		{
			tkn = ptr[0];
			ptr[0] = '\0';
			if (strlen(pt))
			{
				if (ptr2 = strstr(pn, ptr))
				{
					pn = ptr2;
					ptr[0] = tkn;
					continue;
				}
				break;
			}
			ptr[0] = tkn;
			continue;
		}
		if (ptr = strstr(pn, pt))
		{
			pn = ptr;
			continue;
		}
		break;
	}
	return found;
}


#if defined (_WIN32)

/* ================================================================
 * DirentIsFile		_WIN32
 */

int DirentIsFile(const DIRHANDLE dirInfo)
{
	if (!dirInfo)
		return 0;
	if (dirInfo->d_dirent.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return 0;
	return 1;
}

/* ================================================================
 * DirentIsDirectory		_WIN32
 */

int DirentIsDirectory(const DIRHANDLE dirInfo)
{
	if (!dirInfo)
		return 0;
	if (dirInfo->d_dirent.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return 1;
	return 0;
}

/* ================================================================
 * DirentIsLink				_WIN32
 */

int DirentIsLink(const DIRHANDLE dirInfo)
{
	return 0;
}

/* ================================================================
 * DirentIsFIFO				_WIN32
 */

int DirentIsFIFO(const DIRHANDLE dirInfo)
{
	return 0;
}

/* ================================================================
 * DirentIsSocket			_WIN32
 */

int DirentIsSocket(const DIRHANDLE dirInfo)
{
	return 0;
}

/* ================================================================
 * DirentIsBlockDev			_WIN32
 */

int DirentIsBlockDev(const DIRHANDLE dirInfo)
{
	return 0;
}

/* ================================================================
 * DirentIsCharDev			_WIN32
 */

int DirentIsCharDev(const DIRHANDLE dirInfo)
{
	return 0;
}

/* ================================================================
 * DirentGetFileName		_WIN32
 */

char * DirentGetFileName(const DIRHANDLE dirInfo)
{
	return dirInfo->d_dirent.cFileName;
}

/* ================================================================
 * DirentGetFullPath		_WIN32
 */

char * DirentGetFullPath(const DIRHANDLE dirInfo)
{
	size_t namelength;
	size_t dirlength;
	if (dirInfo->d_full_path)
	{
		free(dirInfo->d_full_path);
		dirInfo->d_full_path = NULL;
	}
	namelength = strlen(dirInfo->d_dirent.cFileName);
	dirlength = strlen(dirInfo->d_base_dir);
	dirInfo->d_full_path = calloc((namelength + dirlength + 3), 1);
	if (!dirInfo->d_full_path)
		return NULL;
	sprintf(dirInfo->d_full_path, "%s%s%s",
		dirInfo->d_base_dir, DIRSLASH, dirInfo->d_dirent.cFileName);
	return dirInfo->d_full_path;
}

#endif /*_WIN32 */

#if defined(__GNUC__)
//#include <fcntl.h>
#include <dirent.h>

/*

 */

/* ================================================================
 * DirentIsFile				__GNUC__
 */

int DirentIsFile(const DIRHANDLE dirInfo)
{
	if (!dirInfo)
		return 0;
	if (dirInfo->d_dirent.d_type == DT_REG)
		return 1;
	return 0;
}

/* ================================================================
 * DirentIsDirectory		__GNUC__
 */

int DirentIsDirectory(const DIRHANDLE dirInfo)
{
	if (!dirInfo)
		return 0;
	if (dirInfo->d_dirent.d_type == DT_DIR)
		return 1;
	return 0;
}

/* ================================================================
 * DirentIsLink				__GNUC__
 */

int DirentIsLink(const DIRHANDLE dirInfo)
{
	if (!dirInfo)
		return 0;
	if (dirInfo->d_dirent.d_type == DT_LNK)
		return 1;
	return 0;
}

/* ================================================================
 * DirentIsFIFO				__GNUC__
 */

int DirentIsFIFO(const DIRHANDLE dirInfo)
{
	if (!dirInfo)
		return 0;
	if (dirInfo->d_dirent.d_type == DT_FIFO)
		return 1;
	return 0;
}

/* ================================================================
 * DirentIsSocket			__GNUC__
 */

int DirentIsSocket(const DIRHANDLE dirInfo)
{
	if (!dirInfo)
		return 0;
	if (dirInfo->d_dirent.d_type == DT_SOCK)
		return 1;
	return 0;
}

/* ================================================================
 * DirentIsBlockDev			__GNUC__
 */

int DirentIsBlockDev(const DIRHANDLE dirInfo)
{
	if (!dirInfo)
		return 0;
	if (dirInfo->d_dirent.d_type == DT_BLK)
		return 1;
	return 0;
}

/* ================================================================
 * DirentIsCharDev			__GNUC__
 */

int DirentIsCharDev(const DIRHANDLE dirInfo)
{
	if (!dirInfo)
		return 0;
	if (dirInfo->d_dirent.d_type == DT_CHR)
		return 1;
	return 0;
}

/* ================================================================
 * DirentGetFileName		__GNUC__
 */

char * DirentGetFileName(const DIRHANDLE dirInfo)
{
	if (!dirInfo)
		return NULL;
	return dirInfo->d_dirent.d_name;
}

/* ================================================================
 * DirentGetFullPath		__GNUC__
 */

char * DirentGetFullPath(const DIRHANDLE dirInfo)
{
	int namelength;
	int dirlength;
    if (dirInfo->d_full_path)
	{
		free(dirInfo->d_full_path);
		dirInfo->d_full_path = NULL;
	}
	namelength = strlen(dirInfo->d_dirent.d_name);
	dirlength = strlen(dirInfo->d_base_dir);
	dirInfo->d_full_path = calloc((namelength + dirlength + 3), 1);
	if (!dirInfo->d_full_path)
		return NULL;
	sprintf(dirInfo->d_full_path, "%s%s%s",
		dirInfo->d_base_dir, DIRSLASH, dirInfo->d_dirent.d_name);
	return dirInfo->d_full_path;
}

#endif /*__GNUC__*/


/* ================================================================
 * FindFirstDirEntry
 *
 * First parameter is a lookup path with possible wildcards.  If this
 * parameter is empty, the currently connected directory is used as a
 * basis and "*" is assumed for the wildcard lookup.  Only the '*' and '?'
 * wildcard characters are accepted.  General regular expressions are 
 * not parsed.
 *
 * The return value is the address of a directory search control structure
 * that is allocated in the heap.  A call to CloseFindDir will close the
 * embedded directory stream and release the resources occupied by the
 * directory search control structure.
 *
 * A return value of NULL or INVALID_HANDLE_VALUE is returned to indicate
 * an error.  (__GNUC__) returns NULL.  (_WIN32) returns ((HANDLE)(-1)).
 */

DIRHANDLE FindFirstDirEntry(
	const char * lookup_filename)
{
	DIRHANDLE	dirHandle;
	int		rc;
	int		i;
	char	* ptr;
	char	dirname[MAXPATHLEN];
	char	findinfo[NAME_MAX +1];
	char	* temp_filename;

#if defined(_WIN32)

/* Sanity check */

	if (!lookup_filename && ((strlen(lookup_filename) + 1) >= MAXPATHLEN))
	{
		SetLastError(ERROR_INVALID_PARAMETER);
		return INVALID_HANDLE_VALUE;
	}
/* copy to temp_filename */

	if ((temp_filename = strdup(lookup_filename)) == NULL)
	{
		return INVALID_HANDLE_VALUE;
	}

/* allocate a control structure for dirHandle */

	dirHandle = calloc(1, sizeof(DIRHANDLE_STRUCT));
	if (!dirHandle)
	{
		free(temp_filename);
		return INVALID_HANDLE_VALUE;
	}

/* test if we received a lookup_filename string */
/* and parse it to see if it is rooted to a unix tree (or WIN_device) */

	if (!temp_filename || !(temp_filename[0] == '\\' ||
		temp_filename[0] == '/' ||
		(temp_filename[1] == ':' && temp_filename[2] == '\\')))
	{

/* lookup_filename is not rooted to a WIN_device or unix tree */

		rc = GetCurrentDirectory(MAXPATHLEN, dirname);
		if (!temp_filename)

/* have no lookup_filename info - use standard wildcard */

			strcpy(findinfo,"*");

		else
		{	/* find last directory separator token */
			for(ptr = temp_filename, i=0; ptr; ptr++, ptr=strpbrk(ptr, "/\\:"))
			{
				i= ptr - temp_filename;
			}
			if (i) /* and append dirpart of lookup_filename */
			{
				ptr = dirname + strlen(dirname) + i +1;
				if ((ptr - dirname) >= MAXPATHLEN)
				{
/* too much data, free resources and return. */
					free(dirHandle);
					free(temp_filename);
					SetLastError(ERROR_INVALID_PARAMETER);
					return INVALID_HANDLE_VALUE;
				}
				strcat(dirname, DIRSLASH);
				strncat(dirname, temp_filename, i);
				*ptr = '\0';
				ptr = temp_filename + i + 1;
			}
			else /* no directory separator token found */
				ptr = temp_filename;
			if (!*ptr)	/* nothing exists after last directory separator */
				strcpy(findinfo, "*");
			else
			{
				if (strlen(ptr) > NAME_MAX)
				{
					free(dirHandle);
					free(temp_filename);
					SetLastError(ERROR_INVALID_PARAMETER);
					return INVALID_HANDLE_VALUE;
				}
				strcpy(findinfo, ptr);
			}
		}
	}
	else
	{
/* lookup_filename is rooted to a unix tree (or WIN_device) */
/* find last directory separator token */

		for (ptr = temp_filename, i=0; ptr; ptr++, ptr=strpbrk(ptr, "/\\:"))
		{
			i = ptr - temp_filename;
		}
		i++;
		if (i >= MAXPATHLEN)
		{
			free(dirHandle);
			free(temp_filename);
			SetLastError(ERROR_INVALID_PARAMETER);
			return INVALID_HANDLE_VALUE;
		}
		strncpy(dirname, temp_filename, i);
		dirname[i-1] = '\0';
		ptr = temp_filename + i;
		if (strlen(ptr) > NAME_MAX)
		{
			free(dirHandle);
			free(temp_filename);
			SetLastError(ERROR_INVALID_PARAMETER);
			return INVALID_HANDLE_VALUE;
		}
		strcpy(findinfo,ptr);
	}

/* Another Sanity Check - do not overflow dirname array */
/* could still happen when proccessing '*' wildcards */

	if ((strlen(dirname) + strlen(findinfo) + 2) >= MAXPATHLEN)
	{
		free(dirHandle);
		free(temp_filename);
		SetLastError(ERROR_INVALID_PARAMETER);
		return INVALID_HANDLE_VALUE;
	}
/* Allocate and initialize the Return Structure Components */
	{
		int eflag = 0;
		size_t dlen = strlen(dirname) +1;
		size_t flen = strlen(findinfo) +1;
		if (dirHandle->d_base_dir = malloc(dlen))
		{
			if (dirHandle->d_find_text = malloc(flen))
				strcpy(dirHandle->d_find_text, findinfo);
			else
				eflag = 1;
			strcpy(dirHandle->d_base_dir, dirname);
		}
		else
			eflag = 1;
		if (eflag)
		{
/* structures cannot be initialized - insufficient memory */
			if (dirHandle->d_base_dir)
				free(dirHandle->d_base_dir);
			free(dirHandle);
			free(temp_filename);
			SetLastError(ERROR_OUTOFMEMORY);
			return INVALID_HANDLE_VALUE;
		}
	}
/* prepare to call WIN32 FindFirstFile to get directory handle */
	strcat(dirname, DIRSLASH);
	strcat(dirname, findinfo);
	if ((dirHandle->d_open_dir = FindFirstFile(
		dirname, &(dirHandle->d_dirent))) == INVALID_HANDLE_VALUE)
	{
/* WIN32 failed on FindFirstFile to get directory handle */
		free(dirHandle->d_base_dir);
		free(dirHandle->d_find_text);
		free(dirHandle);
		free(temp_filename);
		return INVALID_HANDLE_VALUE;
	}
/* Success - return our own handle structure */
	free(temp_filename);
	return dirHandle;

#endif /*_WIN32*/

#if defined(__GNUC__)
	struct dirent * dirent_ptr;
	int	err;

	errno = 0;

/* Sanity Check */

	if (!lookup_filename && ((strlen(lookup_filename) + 1) >= MAXPATHLEN))
	{
		errno = EINVAL;
		return NULL;
	}
/* copy to temp_filename */

	if ((temp_filename = strdup(lookup_filename)) == NULL)
	{
		return INVALID_HANDLE_VALUE;
	}

/* allocate a control structure for dirHandle */

	dirHandle = calloc(1, sizeof(DIRHANDLE_STRUCT));
	if (!dirHandle)
	{
		free(temp_filename);
		return INVALID_HANDLE_VALUE;
	}

/* test if we received a lookup_filename string */
/* and parse it to see if it is rooted to a unix tree */

	if (!temp_filename || !(temp_filename[0] == '\\' ||
		temp_filename[0] == '/'))
	{
		/* lookup_filename is not rooted to a unix tree */
		if (!(getcwd(dirname, MAXPATHLEN -1)))
		{
			free(temp_filename);
			return NULL;
		}
		if (!temp_filename)
			strcpy(findinfo, "*");
		else
		{	/* find last directory separator token */
			for (ptr = temp_filename, i=0; ptr; ptr++, ptr=strpbrk(ptr, "\\/"))
			{
				i = ptr - temp_filename;
			}
			if (i)	/* and append dirpart of lookup_filename */
			{
				ptr = dirname + strlen(dirname) + i + 1;
				if ((ptr - dirname) >= MAXPATHLEN)
				{
/* too much data, free resources and return */
					free(dirHandle);
					free(temp_filename);
					errno = EINVAL;
					return NULL;
				}
				strcat(dirname, DIRSLASH);
				strncat(dirname, temp_filename, i);
				*ptr = '\0';
				ptr = temp_filename +i +1;
			}
			else	/* no directory separator in lookup_filename */
				ptr = temp_filename;
			if (!*ptr)	/* lookup_filename ended with directory separator */
				strcpy(findinfo, "*");
			else
			{
				if (strlen(ptr) > NAME_MAX)
				{
					free(dirHandle);
					free(temp_filename);
					errno = EINVAL;
					return NULL;
				}
				strcpy(findinfo, ptr);
			}
		}
	}
	else
	{
/* lookup_filename is rooted to unix tree */
/* find last directory separator token */
		for (ptr = temp_filename, i=0; ptr; ptr++, ptr=strpbrk(ptr,"\\/"))
		{
			i = ptr - temp_filename;
		}
		i++;
		if (i >= MAXPATHLEN)
		{
			free(dirHandle);
			free(temp_filename);
			errno = EINVAL;
			return NULL;
		}
		strncpy(dirname, temp_filename, i);
		dirname[i-1] = '\0';
		ptr = temp_filename +i;
		if (strlen(ptr) > NAME_MAX)
		{
			free(dirHandle);
			free(temp_filename);
			errno = EINVAL;
			return NULL;
		}
		strcpy(findinfo, ptr);
	}
/* Another Sanity Check - do not overflow dirname array */
	if ((strlen(dirname) + strlen(findinfo) + 1) >= MAXPATHLEN)
	{
		free(dirHandle);
		free(temp_filename);
		errno = EINVAL;
		return NULL;
	}
/* Allocate and initialize the Return Structure Components */
	{
		int eflag = 0;
		size_t dlen = strlen(dirname) +1;
		size_t flen = strlen(findinfo) +1;
		if (dirHandle->d_base_dir = malloc(dlen))
		{
			if (dirHandle->d_find_text = malloc(flen))
				strcpy(dirHandle->d_find_text, findinfo);
			else
				eflag = 1;
			strcpy(dirHandle->d_base_dir, dirname);
		}
		else
			eflag = 1;
		if (eflag)
		{
/* structures cannot be initialized - insufficient memory */
			if (dirHandle->d_base_dir)
				free(dirHandle->d_base_dir);
			free(dirHandle);
			free(temp_filename);
			errno = EFAULT;
			return NULL;
		}
	}
/* prepare to simulate WIN FindFirstFile */

/* Use dirname to get a unix directory handle */

	if (!(dirHandle->d_open_dir = opendir(dirname)))
	{
/* not a directory or too many open files */
		int err = errno;
		free(dirHandle->d_base_dir);
		free(dirHandle->d_find_text);
		free(dirHandle);
		free(temp_filename);
		errno = err;
		return NULL;
	}
/* scan directory entries until match or eof */
	for (dirent_ptr = readdir(dirHandle->d_open_dir);
		dirent_ptr; dirent_ptr = readdir(dirHandle->d_open_dir))
	{
		if (FileNameDirEntryTest(
			dirent_ptr->d_name,
			dirHandle->d_find_text))
			break;
	}
	if (dirent_ptr)
	{
		memcpy(&(dirHandle->d_dirent), dirent_ptr, sizeof(struct dirent));
		free(temp_filename);
		return dirHandle;
	}
	err = errno;
	closedir(dirHandle->d_open_dir);
	free(dirHandle->d_base_dir);
	free(dirHandle->d_find_text);
	free(dirHandle);
	free(temp_filename);
	errno = err;
	return NULL;

#endif /*__GNUC__*/
}

/* ================================================================
 * FindNextDirEntry
 */

int FindNextDirEntry(
	DIRHANDLE dirHandle)
{
#if defined(_WIN32)
	int rc;
	rc = FindNextFile(
		dirHandle->d_open_dir,
		&(dirHandle->d_dirent));
	return rc;
#endif /*_WIN32*/

#if defined(__GNUC__)
	struct dirent *dirent_ptr;
/* scan directory entries until match or eof */
	for (dirent_ptr = readdir(dirHandle->d_open_dir);
		dirent_ptr; dirent_ptr = readdir(dirHandle->d_open_dir))
	{
		if (FileNameDirEntryTest(
			dirent_ptr->d_name,
			dirHandle->d_find_text))
			break;
	}
	if (dirent_ptr)
	{
		memcpy(&(dirHandle->d_dirent), dirent_ptr, sizeof(struct dirent));
		return 1;
	}
	return 0;
#endif /*__GNUC__*/
}

/* ================================================================
 * CloseFindDir
 */

int CloseFindDir(
	DIRHANDLE dirHandle)
{
	int rc = 0;
	int err = 0;

#if defined(_WIN32)
	rc = FindClose(dirHandle->d_open_dir);
	if (!rc)
		return 1;	/*RETURN_FAILURE*/
	free(dirHandle->d_base_dir);
	free(dirHandle->d_find_text);
	if (dirHandle->d_full_path)
		free(dirHandle->d_full_path);
	free(dirHandle);
	return 0;		/*RETURN_SUCCESS*/
#endif /*_WIN32*/

#if defined(__GNUC__)
	rc = closedir(dirHandle->d_open_dir);
	if (rc)
		return 1;	/*RETURN_FAILURE*/
	free(dirHandle->d_base_dir);
	free(dirHandle->d_find_text);
	if (dirHandle->d_full_path)
		free(dirHandle->d_full_path);
	free(dirHandle);
	return 0;		/*RETURN_SUCCESS*/
#endif /*__GNUC__*/
}

