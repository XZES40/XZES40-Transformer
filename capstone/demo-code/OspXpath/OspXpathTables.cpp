/*
 *
 * Copyright (c) 2007-2009 Oregon State Police / CJIS-LEDS
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
 *    1.00      Sep 24 2009            hathaway
 * Initial Version
 * getTableAsNodeset() capability not yet supported
 */

#include "OspXpathFunctionLib.hpp"

#include <xalanc/XPath/XPathEnvSupportDefault.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>

#include "keylist.h"
#include "keytable.h"

#include <math.h>

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

//
// OSPXpathInitTableDB::OSPXpathInitTableDB()
// XALAN Wrapper for initXpathTableDB()
//

OSPXpathInitTableDB::OSPXpathInitTableDB():
 Function()
{
}


OSPXpathInitTableDB::~OSPXpathInitTableDB()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathInitTableDB*
#endif
OSPXpathInitTableDB::clone(MemoryManagerType&    theManager) const
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
OSPXpathInitTableDB::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	const XObjectArgVectorType::size_type	theArgCount = args.size();

	if (theArgCount == 0)
	{
		return execute(executionContext, context, locator);
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
OSPXpathInitTableDB::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: initTableDB() Takes No Arguments");
	return theResult;
}

XObjectPtr
OSPXpathInitTableDB::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const LocatorType *			locator) const
{
	XalanDOMString theBuffer(executionContext.getMemoryManager());
	initXpathTableDB();
	return createEmptyString(executionContext);
}

//============================================================================

//
// OSPXpathDropTableDB::OSPXpathDropTableDB()
// XALAN Wrapper for dropXpathTableDB()
//

OSPXpathDropTableDB::OSPXpathDropTableDB():
 Function()
{
}


OSPXpathDropTableDB::~OSPXpathDropTableDB()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathDropTableDB*
#endif
OSPXpathDropTableDB::clone(MemoryManagerType&    theManager) const
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
OSPXpathDropTableDB::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	const XObjectArgVectorType::size_type	theArgCount = args.size();

	if (theArgCount == 0)
	{
		return execute(executionContext, context, locator);
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
OSPXpathDropTableDB::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: dropTableDB() Takes No Arguments");
	return theResult;
}

XObjectPtr
OSPXpathDropTableDB::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const LocatorType *			locator) const
{
	XalanDOMString theBuffer(executionContext.getMemoryManager());
	dropXpathTableDB();
	return createEmptyString(executionContext);
}

//============================================================================

//
// OSPXpathDropTableName::OSPXpathDropTableName()
// XALAN Wrapper for dropXpathTableName()
//

OSPXpathDropTableName::OSPXpathDropTableName():
 Function()
{
}


OSPXpathDropTableName::~OSPXpathDropTableName()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathDropTableName*
#endif
OSPXpathDropTableName::clone(MemoryManagerType&    theManager) const
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
OSPXpathDropTableName::execute(
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
OSPXpathDropTableName::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: dropTableName() Takes One Argument");
	return theResult;
}

XObjectPtr
OSPXpathDropTableName::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const LocatorType *			locator) const
{
	CharVectorType	theTableName;
	char *			charTableName;

// Convert the table name
	theTableName = TranscodeToLocalCodePage(arg1->str());
	charTableName = theTableName.begin();

	dropXpathTableName(charTableName);
	return createEmptyString(executionContext);
}

//============================================================================

//
// OSPXpathDropTableKey::OSPXpathDropTableKey()
// XALAN Wrapper for dropXpathTableKey()
//

OSPXpathDropTableKey::OSPXpathDropTableKey():
 Function()
{
}


OSPXpathDropTableKey::~OSPXpathDropTableKey()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathDropTableKey*
#endif
OSPXpathDropTableKey::clone(MemoryManagerType&    theManager) const
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
OSPXpathDropTableKey::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	const XObjectArgVectorType::size_type	theArgCount = args.size();

	if (theArgCount == 2)
	{
		return execute(executionContext, context, args[0], args[1], locator);
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
OSPXpathDropTableKey::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: dropTableKey() Takes Two Arguments");
	return theResult;
}

XObjectPtr
OSPXpathDropTableKey::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const XObjectPtr			arg2,
	const LocatorType *			locator) const
{
	CharVectorType	theTableName;
	char *			charTableName;
	CharVectorType	theTableKey;
	char *			charTableKey;

// Convert the table name and key

	theTableName = TranscodeToLocalCodePage(arg1->str());
	charTableName = theTableName.begin();
	theTableKey = TranscodeToLocalCodePage(arg2->str());
	charTableKey = theTableKey.begin();

	dropXpathTableKey(charTableName, charTableKey);
	return createEmptyString(executionContext);
}

//============================================================================

//
// OSPXpathDropTableRow::OSPXpathDropTableRow()
// XALAN Wrapper for dropXpathTableRow()
//

OSPXpathDropTableRow::OSPXpathDropTableRow():
 Function()
{
}


OSPXpathDropTableRow::~OSPXpathDropTableRow()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathDropTableRow*
#endif
OSPXpathDropTableRow::clone(MemoryManagerType&    theManager) const
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
OSPXpathDropTableRow::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	const XObjectArgVectorType::size_type	theArgCount = args.size();

	if (theArgCount == 2)
	{
		return execute(executionContext, context, args[0], args[1], locator);
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
OSPXpathDropTableRow::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: DropTableRow() Takes Two Arguments");
	return theResult;
}

XObjectPtr
OSPXpathDropTableRow::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const XObjectPtr			arg2,
	const LocatorType *			locator) const
{
	CharVectorType	theTableName;
	char *			charTableName;
	double			theTableRow;
	int				theRow;

// Convert the table name and key

	theTableName = TranscodeToLocalCodePage(arg1->str());
	charTableName = theTableName.begin();
	theTableRow = arg2->num();
	theRow = (int)theTableRow;		// get the integer part

	dropXpathTableRow(charTableName, theRow);
	return createEmptyString(executionContext);
}

//============================================================================

//
// OSPXpathSetTableKeyValue::OSPXpathSetTableKeyValue()
// XALAN Wrapper for setXpathTableKeyValue()
//

OSPXpathSetTableKeyValue::OSPXpathSetTableKeyValue():
 Function()
{
}


OSPXpathSetTableKeyValue::~OSPXpathSetTableKeyValue()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathSetTableKeyValue*
#endif
OSPXpathSetTableKeyValue::clone(MemoryManagerType&    theManager) const
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
OSPXpathSetTableKeyValue::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	const XObjectArgVectorType::size_type	theArgCount = args.size();

	if (theArgCount == 3)
	{
		return execute(executionContext, context, args[0], args[1], args[2], locator);
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
OSPXpathSetTableKeyValue::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: setTableKeyValue() Takes Three Arguments");
	return theResult;
}

XObjectPtr
OSPXpathSetTableKeyValue::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const XObjectPtr			arg2,
	const XObjectPtr			arg3,
	const LocatorType *			locator) const
{
	CharVectorType	theTableName;
	char *			charTableName;
	CharVectorType	theTableKey;
	char *			charTableKey;
	CharVectorType	theTableValue;
	char *			charTableValue;

// Convert the table name and key and value

	theTableName = TranscodeToLocalCodePage(arg1->str());
	charTableName = theTableName.begin();
	theTableKey = TranscodeToLocalCodePage(arg2->str());
	charTableKey = theTableKey.begin();
	theTableValue = TranscodeToLocalCodePage(arg3->str());
	charTableValue = theTableValue.begin();

	if (setXpathTableKeyValue(charTableName, charTableKey, charTableValue))
	{
		// we have a memory allocation error so tell the XPATH subsystem
        XPathExecutionContext::GetAndReleaseCachedString    theGuard(executionContext);
        XalanDOMString&     theResult = theGuard.get();

		// theResult is a managed XalanDOMString.
		// put a message into XalanDOMString& theResult;
		// supply the message to executionContext.error(...);

		theResult.assign("OSPXpathSetTableKeyValue : setXpathTableKeyValue : memory allocation error");

#if (_XALAN_VERSION == 11000)
// xalanc-1_10 error handler
        executionContext.error( theResult, context, locator);
#endif
#if (_XALAN_VERSION >= 11100)
// xalan-1_11 error handler
		executionContext.problem(
			XPathExecutionContext::eXPath,
			XPathExecutionContext::eError,
			theResult,
			locator, context);
#endif
        // Dummy return value...
        return XObjectPtr();
	}
	return createEmptyString(executionContext);
}

//============================================================================

//
// OSPXpathGetTableKeyValue::OSPXpathGetTableKeyValue()
// XALAN Wrapper for getXpathTableKeyValue()
//

OSPXpathGetTableKeyValue::OSPXpathGetTableKeyValue():
 Function()
{
}


OSPXpathGetTableKeyValue::~OSPXpathGetTableKeyValue()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathGetTableKeyValue*
#endif
OSPXpathGetTableKeyValue::clone(MemoryManagerType&    theManager) const
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
OSPXpathGetTableKeyValue::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	const XObjectArgVectorType::size_type	theArgCount = args.size();

	if (theArgCount == 2)
	{
		return execute(executionContext, context, args[0], args[1], locator);
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
OSPXpathGetTableKeyValue::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: getTableKeyValue() Takes Two Arguments");
	return theResult;
}

XObjectPtr
OSPXpathGetTableKeyValue::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const XObjectPtr			arg2,
	const LocatorType *			locator) const
{
	CharVectorType	theTableName;
	char *			charTableName;
	CharVectorType	theTableKey;
	char *			charTableKey;

// Convert the table name and key

	theTableName = TranscodeToLocalCodePage(arg1->str());
	charTableName = theTableName.begin();
	theTableKey = TranscodeToLocalCodePage(arg2->str());
	charTableKey = theTableKey.begin();

// Return character string to XPATH
	XalanDOMString theResult(getXpathTableKeyValue(charTableName, charTableKey));

	return executionContext.getXObjectFactory().createString(theResult);

}

//============================================================================

//
// OSPXpathGetTableKeyRow::OSPXpathGetTableKeyRow()
// XALAN Wrapper for getXpathTableKeyRow()
//

OSPXpathGetTableKeyRow::OSPXpathGetTableKeyRow():
 Function()
{
}


OSPXpathGetTableKeyRow::~OSPXpathGetTableKeyRow()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathGetTableKeyRow*
#endif
OSPXpathGetTableKeyRow::clone(MemoryManagerType&    theManager) const
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
OSPXpathGetTableKeyRow::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	const XObjectArgVectorType::size_type	theArgCount = args.size();

	if (theArgCount == 2)
	{
		return execute(executionContext, context, args[0], args[1], locator);
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
OSPXpathGetTableKeyRow::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: getTableKeyRow() Takes Two Arguments");
	return theResult;
}

XObjectPtr
OSPXpathGetTableKeyRow::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const XObjectPtr			arg2,
	const LocatorType *			locator) const
{
	CharVectorType	theTableName;
	char *			charTableName;
	CharVectorType  theTableKey;
	char *			charTableKey;
	int				theRow;
	double			theRowNumber;

// Convert the table name and key

	theTableName = TranscodeToLocalCodePage(arg1->str());
	charTableName = theTableName.begin();
	theTableKey = TranscodeToLocalCodePage(arg2->str());
	charTableKey = theTableKey.begin();

	theRow = getXpathTableKeyRow(charTableName, charTableKey);
	theRowNumber = theRow;

	return executionContext.getXObjectFactory().createNumber(theRowNumber);

}

//============================================================================

//
// OSPXpathGetTableRowKey::OSPXpathGetTableRowKey()
// XALAN Wrapper for getXpathTableRowKey()
//

OSPXpathGetTableRowKey::OSPXpathGetTableRowKey():
 Function()
{
}


OSPXpathGetTableRowKey::~OSPXpathGetTableRowKey()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathGetTableRowKey*
#endif
OSPXpathGetTableRowKey::clone(MemoryManagerType&    theManager) const
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
OSPXpathGetTableRowKey::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	const XObjectArgVectorType::size_type	theArgCount = args.size();

	if (theArgCount == 2)
	{
		return execute(executionContext, context, args[0], args[1], locator);
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
OSPXpathGetTableRowKey::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: getTableRowKey() Takes Two Arguments");
	return theResult;
}

XObjectPtr
OSPXpathGetTableRowKey::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const XObjectPtr			arg2,
	const LocatorType *			locator) const
{
	CharVectorType	theTableName;
	char *			charTableName;
	double			theRowNumber;
	int				theRow;

// Convert the table name and row

	theTableName = TranscodeToLocalCodePage(arg1->str());
	charTableName = theTableName.begin();
	theRowNumber = arg2->num();
	theRow = (int)theRowNumber;	// get the rounded integer

// Return character string to XPATH
	XalanDOMString theResult(getXpathTableRowKey(charTableName, theRow));

	return executionContext.getXObjectFactory().createString(theResult);
}

//============================================================================

//
// OSPXpathGetTableRowValue::OSPXpathGetTableRowValue()
// XALAN Wrapper for getXpathTableRowValue()
//

OSPXpathGetTableRowValue::OSPXpathGetTableRowValue():
 Function()
{
}


OSPXpathGetTableRowValue::~OSPXpathGetTableRowValue()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathGetTableRowValue*
#endif
OSPXpathGetTableRowValue::clone(MemoryManagerType&    theManager) const
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
OSPXpathGetTableRowValue::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	const XObjectArgVectorType::size_type	theArgCount = args.size();

	if (theArgCount == 2)
	{
		return execute(executionContext, context, args[0], args[1], locator);
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
OSPXpathGetTableRowValue::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: getTableRowValue() Takes Two Arguments");
	return theResult;
}

XObjectPtr
OSPXpathGetTableRowValue::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const XObjectPtr			arg2,
	const LocatorType *			locator) const
{
	CharVectorType	theTableName;
	char *			charTableName;
	double			theRowNumber;
	int				theRow;

// Convert the table name and row

	theTableName = TranscodeToLocalCodePage(arg1->str());
	charTableName = theTableName.begin();
	theRowNumber = arg2->num();
	theRow = (int)theRowNumber;	// get the rounded integer

// Return character string to XPATH
	XalanDOMString theResult(getXpathTableRowValue(charTableName, theRow));

	return executionContext.getXObjectFactory().createString(theResult);
}

//============================================================================

//
// OSPXpathGetTableRowCount::OSPXpathGetTableRowCount()
// XALAN Wrapper for getXpathTableRowCount()
//

OSPXpathGetTableRowCount::OSPXpathGetTableRowCount():
 Function()
{
}


OSPXpathGetTableRowCount::~OSPXpathGetTableRowCount()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathGetTableRowCount*
#endif
OSPXpathGetTableRowCount::clone(MemoryManagerType&    theManager) const
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
OSPXpathGetTableRowCount::execute(
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
OSPXpathGetTableRowCount::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: getTableRowCount() Takes One Argument");
	return theResult;
}

XObjectPtr
OSPXpathGetTableRowCount::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const LocatorType *			locator) const
{
	CharVectorType	theTableName;
	char *			charTableName;
	double			theRowCount;
	int				theCount;

// Convert the table name and row

	theTableName = TranscodeToLocalCodePage(arg1->str());
	charTableName = theTableName.begin();

	theCount = getXpathTableRowCount(charTableName);
	theRowCount = theCount;
	return executionContext.getXObjectFactory().createNumber(theRowCount);
}

//============================================================================

//
// OSPXpathSortTableKeys::OSPXpathSortTableKeys()
// XALAN Wrapper for sortXpathTableKeys()
//

OSPXpathSortTableKeys::OSPXpathSortTableKeys():
 Function()
{
}


OSPXpathSortTableKeys::~OSPXpathSortTableKeys()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathSortTableKeys*
#endif
OSPXpathSortTableKeys::clone(MemoryManagerType&    theManager) const
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
OSPXpathSortTableKeys::execute(
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
OSPXpathSortTableKeys::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: sortTableKeys() Takes One Argument");
	return theResult;
}

XObjectPtr
OSPXpathSortTableKeys::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const LocatorType *			locator) const
{
	CharVectorType	theTableName;
	char *			charTableName;
//	double			theRowCount;
//	int				theCount;

// Convert the table name

	theTableName = TranscodeToLocalCodePage(arg1->str());
	charTableName = theTableName.begin();

	sortXpathTableKeys(charTableName);

	return createEmptyString(executionContext);
}

//============================================================================

//
// OSPXpathSortTableValues::OSPXpathSortTableValues()
// XALAN Wrapper for sortXpathTableValues()
//

OSPXpathSortTableValues::OSPXpathSortTableValues():
 Function()
{
}


OSPXpathSortTableValues::~OSPXpathSortTableValues()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathSortTableValues*
#endif
OSPXpathSortTableValues::clone(MemoryManagerType&    theManager) const
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
OSPXpathSortTableValues::execute(
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
OSPXpathSortTableValues::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: sortTableValues() Takes One Argument");
	return theResult;
}

XObjectPtr
OSPXpathSortTableValues::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const LocatorType *			locator) const
{
	CharVectorType	theTableName;
	char *			charTableName;
//	double			theRowCount;
//	int				theCount;

// Convert the table name

	theTableName = TranscodeToLocalCodePage(arg1->str());
	charTableName = theTableName.begin();

	sortXpathTableValues(charTableName);

	return createEmptyString(executionContext);
}

//============================================================================

//
// OSPXpathInvertTable::OSPXpathInvertTable()
// XALAN Wrapper for invertXpathTable()
//

OSPXpathInvertTable::OSPXpathInvertTable():
 Function()
{
}


OSPXpathInvertTable::~OSPXpathInvertTable()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathInvertTable*
#endif
OSPXpathInvertTable::clone(MemoryManagerType&    theManager) const
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
OSPXpathInvertTable::execute(
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
OSPXpathInvertTable::getError(XalanDOMString&    theResult) const
{
	theResult.assign("Function: invertTable() Takes One Argument");
	return theResult;
}

XObjectPtr
OSPXpathInvertTable::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const LocatorType *			locator) const
{
	CharVectorType	theTableName;
	char *			charTableName;
//	double			theRowCount;
//	int				theCount;

// Convert the table name

	theTableName = TranscodeToLocalCodePage(arg1->str());
	charTableName = theTableName.begin();

	invertXpathTable(charTableName);

	return createEmptyString(executionContext);
}

//============================================================================



XALAN_CPP_NAMESPACE_END
