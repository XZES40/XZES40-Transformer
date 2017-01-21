/*
 * Copyright (c) 2007-2009 Oregon State Police / LEDS
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
 *    Onsite.1  Oct 24 2007            hathaway
 * Initial revision.
 * 
 *    Rev 0.1   Oct 19 2007 12:25:36   hathaway
 * Development Started.
 */

/**
 * Future Directions:
 *
 *  Add a parameter to distinguish Local/Global install.
 *
 *  Local install makes copies of namespace and function names, and
 *  makes a clone of the XPath function classes, and is used for one
 *  instance of the Xalan Transformer.  The local memory is associated
 *  with the Xalan Transformer and not global to the program process.
 *
 *  Global install assumes static allocation of namespace and function
 *  names and uses a static instance of the XPath function classes.
 *  All instances of XalanTransformer associated with a process share
 *  the process-global address space.
 *
 *  Add a parameter for namespace.
 *  Search the SQL database to find XPath function names to install.
 *  This parameter can be used to distinguish functions that support
 *  the needs of separate OSP-2000-JX installations.  Hopefully
 *  the installer function will know the XPath function classes.
 *
 *  OSP Custom XPath Function Library.
 *  Similar to the custom parsers and custom idinputs in Oregon,
 *  a special library interface can be constructed to invoke functions
 *  in a custom XPath Function Library maintained at user sites.
 */

#if !defined(OSP_XPATH_HEADER_GUARD_20071019)
#define OSP_XPATH_HEADER_GUARD_20071019

/**
 * XalanHandle should be defined by
 *  #include <xalanc/XalanTransformer/XalanCAPI.h>
 *
 *  The actual underlying XalanHandle type in 'C' is a pointer to void.
 *  Inside the C++ Xalan libraries the type is cast to the internal
 *  class that implements the XSLT transformation environment.
 */

#include <xalanc/XalanTransformer/XalanCAPI.h>
// typedef void*			XalanHandle;

/**
 * This header supplies only the 'C' language API interface for
 * installing custom XPATH functions.  The actual implementation
 * is bound in a C++ wrapper to resolve passing information between
 * the XALAN C++ and 'C' language programming environments.
 */

#if defined(__cplusplus)
extern "C"
{
#endif

/**
 * Memory Management Notes: local vs global.
 *
 * The XALAN transformer libraries are thread-safe.  Only the first
 * instance of the transformation environment is bound to the process
 * address space and assumes (global) address scope for all stylesheet
 * transformations.  Threads can declare a transformation environment
 * for their own use and the thread-specific memory assumes (local)
 * address scope for stylesheets processed by that thread.
 *
 * In OSP/JX, many of the extension functions are NOT THREAD SAFE
 * so transformations always occur in the context of the (global)
 * process.  OSP/JX should not create and use thread-based XALAN
 * transformations until all unsafe references have been resolved.
 */

typedef enum {local, global} MemLocal;

/**
 * Install OSP Custom XPath Functions for use by XalanTranslator
 *
 * We may want to add namespace and function-name arguments.
 * The current prototype implementation has hard-coded namespaces,
 * function names, and function class references.
 */

void
OSPXpathInstall(XalanHandle theHandle, MemLocal insMem);

/**
 * Uninstall OSP Custom XPath Functions from XalanTranslator
 *
 * We may want to add namespace and function-name arguments.
 * The current prototype implementation has hard-coded namespaces
 * and function names.
 */

void
OSPXpathUninstall(XalanHandle theHandle, MemLocal insMem);


#if defined(__cplusplus)
}
#endif

#endif // OSP_XPATH_HEADER_GUARD_20071019
