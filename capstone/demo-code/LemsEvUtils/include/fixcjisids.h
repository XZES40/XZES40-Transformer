

#ifndef FIX_CJIS_SID_ORI_FORMAT_H
#define FIX_CJIS_SID_ORI_FORMAT_H

/**
 * FixSidFormat
 *
 * The sidState gives a default state code if sidNumber is not 10 characters.
 * A sidNumber of 10 characters is assumed to have a state code prefix.
 * 
 * A sidNumber of 8 characters will have the sidState applied if found. Otherwise
 * the default "OR" state prefix will be applied.
 *
 * The state prefix will be ZeroToOh converted (MO, OR, OH, OK, IO, CO)
 * The sid number will be OhToZero converted - digits after state prefix.
 *
 * The function is not thread safe.  The result string is preallocated as static storage.
 *
 */

char * FixSidFormat(const unsigned char * sidState, const unsigned char * sidNumber);


/**
 * FixOriFormat
 *
 * Parameter theOri is assumed to be 9 characters.  The first 2 characters are ZeroToOh
 * converted.  The remaining characters are OhToZero converted.
 *
 * The function is not thread safe.  The result string is preallocated as static storage.
 */

char * FixOriFormat(const unsigned char * theOri);



#endif  /* FIX_CJIS_SID_ORI_FORMAT_H */



