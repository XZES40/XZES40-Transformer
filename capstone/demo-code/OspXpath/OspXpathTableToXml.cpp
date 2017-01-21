/*
 *
 * Copyright (c) 2007-2009 Oregon State Police / CJIS-LEDS
 *
 * This code uses the Apache Xalan/Xerces libraries that are licensed
 * under the Apache License, Version 2.0 (the "License").
 *
 * You may obtain copies of the Apache license at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Since this software is a custom extension to the Xalan-C
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
 *    1.00      Oct 03 2009            Hathaway
 * Initial Release
 *
 */

#include <xalanc/XPath/XPathEnvSupportDefault.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XSLT/FunctionDocument.hpp>

#include "OspXpathTableToXml.hpp"

#include "keylist.h"
#include "keytable.h"

// For conditional compile - xalan version dependencies
#include <xalanc/Include/XalanVersion.hpp>

#if (_XALAN_VERSION < 11000)
#error "XALAN Library Version must be 1_10 or newer"
#endif

XALAN_CPP_NAMESPACE_BEGIN

/*
 * theEmptyString is useful for supplying dummy arguments with strings
 * of length()=0 to secondary execute() methods.
 */

static const XalanDOMString		theEmptyString(XalanMemMgrs::getDummyMemMgr());


inline XObjectPtr
createEmptyString(XPathExecutionContext&	executionContext)
{
	return executionContext.getXObjectFactory().createStringReference(theEmptyString);
}

//============================================================================
//
// OSPXpathGetTableToXml::OSPXpathGetTableToXml()
// XALAN Wrapper for setXpathTableKeyValue()
//

OSPXpathGetTableToXml::OSPXpathGetTableToXml():
 FunctionDocument()
{
}


OSPXpathGetTableToXml::~OSPXpathGetTableToXml()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathGetTableToXml*
#endif
OSPXpathGetTableToXml::clone(MemoryManagerType&    theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}

/* XPath functions always call the Format::execute() using XObjectArgVectorType& args.
 * The Format::execute() supports one, two, or three arguments.  If more than
 * three arguments are required, then the Format::execute() method must be
 * overridden in the child class.  The code shown below using XObjectArgVectorType&
 * is copied directly from the Format::execute() implementation.
 */

XObjectPtr
OSPXpathGetTableToXml::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	const XObjectArgVectorType::size_type	theArgCount = args.size();

	if (theArgCount == 1)
	{
		return execute(executionContext, context, args[0], locator);
	}
	else
	{
        XalanDOMString theBuffer(executionContext.getMemoryManager());

#if (_XALAN_VERSION == 11000)
// xalanc-1_10 error handler
		executionContext.error(getError(theBuffer), context, locator);
#endif
#if (_XALAN_VERSION >= 11100)
// xalanc-1_11 error handler
		executionContext.problem(
			XPathExecutionContext::eXPath,
			XPathExecutionContext::eError,
			getError(theBuffer),
			locator, context);
#endif
		return XObjectPtr();
	}
}

/*
 * Supply an error message
 */

const XalanDOMString&
OSPXpathGetTableToXml::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: getTableAsNodeset() Takes One Argument");
	return theResult;
}

XObjectPtr
OSPXpathGetTableToXml::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const LocatorType *			locator) const
{
	CharVectorType	theTableName;
	char *			charTableName;
	char *			tempXmlFilename;

	// Convert the table name

	theTableName = TranscodeToLocalCodePage(arg1->str());
	charTableName = theTableName.begin();

	if (!(tempXmlFilename = dumpKeyTableToFile(charTableName)))
	{
		// we have a memory allocation error so tell the XPATH subsystem
        XPathExecutionContext::GetAndReleaseCachedString    theGuard(executionContext);
        XalanDOMString&     theResult = theGuard.get();

		// theResult is a persistent XalanDOMString with extended lifetime that is
		// eventually bound to the XERCES/XALAN error reporting subsystem.

		// put a message into XalanDOMString& theResult;
		// supply the message to executionContext.error(...);

		theResult.assign("OSPXpathGetTableToXml : dumpKeyTableToFile() : memory allocation or system error");

#if (_XALAN_VERSION == 11000)
// xalanc-1_10 error handler
        executionContext.error( theResult, context, locator);
#endif
#if (_XALAN_VERSION >= 11100)
// xalan-1_11 error handler
		executionContext.problem(
			XPathExecutionContext::eXPath,
			XPathExecutionContext::eError,		// an aborting error
			theResult,
			locator, context);
#endif
        // Dummy return value...
        return XObjectPtr();

		// Note: if we call executionContext.warn(...) or executionContext.problem(.. eWarning)
		// then we should return an XObjectPtr to a persistent empty string.
		//
		// return createEmptyString(executionContext);
	}

	// Convert tempXmlFilename to a stack-based local XalanDOMString
	XalanDOMString theXmlFilename(tempXmlFilename);

	// The FunctionDocument class is actually the C++ parent class of *this.
	// Call the XSLT/XPath document(uri) function to parse theXmlFilename content.
	XObjectPtr theParsedSource = FunctionDocument::execute(
		executionContext, context,
		executionContext.getXObjectFactory().createStringReference(theXmlFilename),
		locator);

	// Release/Remove temporary resources allocated by dumpKeyTableToFile()
	removeKeyTableFile(tempXmlFilename);

	// Test and report if FunctionDocument::execute() returned an error
	if (theParsedSource.null())
	{
		// We should never get here - but this will avoid a severe system abort.
		// Memory leaks are still possible because of a problem.  Other types of
		// errors are handled in the current scheme of doing business.
		return createEmptyString(executionContext);
	}

	// Return theParsedSource as an XalanSourceTree Document nodeset
	return theParsedSource;
}

//============================================================================



XALAN_CPP_NAMESPACE_END
