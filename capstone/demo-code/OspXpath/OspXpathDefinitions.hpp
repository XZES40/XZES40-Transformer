/*
 * Prepared for UNISYS LEMS-2000/JX baseline support by
 * Oregon State Police, Law Enforcement Data System
 * Salem, Oregon
 *
 * Copyright 2007 Oregon State Police / CJIS-LEDS
 *
 * Using Apache Xalan/Xerces libraries that are licensed
 * under the Apache License, Version 2.0 (the "License").
 *
 * You may obtain copies of the Apache license at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Since this software is a custom extension to the Xalan
 * XPATH supported library, and no change is made to the
 * distributed Xalan/Xerces sources,  this software is made
 * available for incorporating into any product.
 *
 * The software contained herein is distributed on an "AS IS"
 * basis without warranties or conditions of any kind, either
 * express or implied.
 */

/**
 * MODIFICATION HISTORY
 *
 *    Onsite.1  Oct 26 2007            hathaway
 * Initial revision.
 * 
 *    Rev 0.1   Oct 26 2007 12:25:36   hathaway
 * Development Started.
*/

#if !defined(OSP_XPATH_DEFS_HEADER_GUARD_20071026)
#define OSP_XPATH_DEFS_HEADER_GUARD_20071026

/* Xalan Platform Definitions
 *
 * The PlatformDefinitions.hpp defines the platform specific
 * import/export constructs required for shared libraries.
 * It knows the differences between GNU, Microsoft, and Intel
 * compilers that can be used to build the Apache XALAN/XERCES
 * libraries.
 *
 * XALAN_PLATFORM_EXPORT,  XALAN_PLATFORM_EXPORT_FUNCTION(T)
 * XALAN_PLATFORM_IMPORT,  XALAN_PLATFORM_IMPORT_FUNCTION(T)
 *
 * The definition of OSP_XPATH_BUILD_DLL determines if a
 * shared library is being created or referenced.
 */

#include <xalanc/Include/PlatformDefinitions.hpp>

#if defined(OSP_XPATH_BUILD_DLL)

#define OSP_XPATH_EXPORT XALAN_PLATFORM_EXPORT

#define OSP_XPATH_EXPORT_FUNCTION(T) XALAN_PLATFORM_EXPORT_FUNCTION(T)

#else

#define OSP_XPATH_EXPORT XALAN_PLATFORM_IMPORT

#define OSP_XPATH_EXPORT_FUNCTION(T) XALAN_PLATFORM_IMPORT_FUNCTION(T)

#endif

#endif //OSP_XPATH_DEFS_HEADER_GUARD_20071026
