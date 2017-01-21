/*
 * 'C' Source Library - Common Utilities - Supported by GNU and Windows VC7.1
 *
 * Copyright (c) 2008 - This is open source software in the pulbic domain.
 *
 * Developed by: Steven J. Hathaway
 *
 * For:  State of Oregon - Oregon State Police / CJIS-LEDS
 */

#ifndef OR_BTCOMP_LIB_H
#define OR_BTCOMP_LIB_H

#include <time.h>

/* Comparison Functions (A, B)
 *
 *    Return Values:
 *     1 = (A) Greater Than (B)
 *     0 = (A) Equal To (B)
 *    -1 = (A) Less Than (B)
 */

int intcomp (int a, int b);

int tmcomp (const struct tm *tm1, const struct tm *tm2);

#endif

