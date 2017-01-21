/*
 * Copyright 2007-2009 Oregon State Police / CJIS-LEDS
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
 *    Onsite.2  Nov 19 2008            hathaway
 * Accommodate xalanc-1_11 change of error handling.
 *
 *    Onsite.1  Oct 26 2007            hathaway
 * Initial revision.
 * 
 *    Rev 0.1   Oct 26 2007 12:25:36   hathaway
 * Development Started.
*/

/**
 * PROGRAMMING TASKS TO BE COMPLETED
 *
 */

#include "OspXpathFunction.hpp"

#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/XalanDOM/XalanDOMString.hpp>

#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/DOMSupport/DOMServices.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>

// For conditional compilation - i.e. error management
#include <xalanc/Include/XalanVersion.hpp>

XALAN_CPP_NAMESPACE_BEGIN

const XObjectPtr	OSPXpathFunction::s_nullXObjectPtr;


OSPXpathFunction::OSPXpathFunction():
 Function()
{
}

OSPXpathFunction::~OSPXpathFunction():
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathFunction*
#endif
OSPXpathFunction::clone(MemoryManagerType&    theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}

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

/* XPath functions always call the Format::execute() using XObjectArgVectorType& args.
 * The Format::execute() supports one, two, or three arguments.  If more than
 * three arguments are required, then the Format::execute() method must be
 * overridden in the child class.  The code shown below using XObjectArgVectorType&
 * is copied directly from the Format::execute() implementation.
 */

OSP_XPATH_EXPORT_FUNCTION(XObjectPtr)
OSPXpathFunction::execute(
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
	else if (theArgCount == 1)
	{
		return execute(executionContext, context, args[0], locator);
	}
	else if (theArgCount == 2)
	{
		return execute(executionContext, context, args[0], args[1], locator);
	}
	else if (theArgCount == 3)
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
#if (_XALAN_VERSION < 11000)
#error "XALAN Library Version must be 1_10 or newer"
#endif

		return XObjectPtr(0);
	}
}

/*
 * Execute with No Arguments
 */

OSP_XPATH_EXPORT_FUNCTION(XObjectPtr)
OSPXpathFunction::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const LocatorType *			locator) const
{
	XalanDOMString theBuffer(executionContext.getMemoryManager());
	executionContext.error(getError(theBuffer), context, locator);
	return XObjectPtr(0);
}

/*
 * Execute with One Argument
 */

OSP_XPATH_EXPORT_FUNCTION(XObjectPtr)
OSPXpathFunction::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const LocatorType *			locator) const
{
  assert(arg1.null() == false);
  return execute(executionContext, context, arg1,
	s_nullXObjectPtr, s_nullXObjectPtr, locator);
}


/*
 * Execute with Two Arguments
 */

OSP_XPATH_EXPORT_FUNCTION(XObjectPtr)
OSPXpathFunction::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *					context,
	const XObjectPtr			arg1,
	const XObjectPtr			arg2,
	const LocatorType *			locator) const
{
  assert(arg1.null() == false);
  if (arg2.null() == true)
	return execute(executionContext, context, arg1,
	s_nullXObjectPtr, s_nullXobjectPtr, locator);
  else
	return execute(executionContext, context, arg1, arg2,
	s_nullXObjectPtr, locator);
}

/*
 * Execute with Three Arguments
 */

OSP_XPATH_EXPORT_FUNCTION(XObjectPtr)
OSPXpathFunction::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *				context,
	const XObjectPtr		arg1,
	const XObjectPtr		arg2,
	const XObjectPtr		arg3,
	const LocatorType *		locator) const
{
/*
 *  RETRIEVING DATA FROM ARGUMENTS
 *
 *  const double            theNumber = arg1->num();
 *
 *  const XalanDOMString&   thePattern = arg2->str();
 *
 *  assert(length(thePattern) != 0);
 *
 *  get the string associated with the context node
 *
 *      XPathExecutionContext::GetAndReleaseCachedString    theGuard(executionContext);
 *      XalanDOMString&     theString = theGuard.get();
 *      DOMServices::getNodeData(*context, theString);
 */
	double				theNumber;

	XalanDOMString &	thePattern;
	CharVectorType &	c_vector;
	char *				c_ptr;

	theNumber = arg[n]->num();		// get the number value associated with the XObject

	thePattern = arg[n]->str();		// get the string value associated with the XObject
									// as XalanDOMString containing XalanDOMChar* data.

	thePattern.transcode(c_vector);	// transcode the XalanDOMChar* data to a char vector
									// with null termination

	c_ptr = c_vector.m_data;		// get the address of the XalanVector<char> data


/*
 * SUPPLYING AN ERROR INSIDE THE FUNCTION EXECUTION
 */

  if (error)
  {
        XPathExecutionContext::GetAndReleaseCachedString    theGuard(executionContext);
        XalanDOMString&     theResult = theGuard.get();

		// theResult is a managed XalanDOMString.
		// get a message into XalanDOMString& theResult;
		// supply the message to executionContext.error(...);

        executionContext.error(
            XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionRequiresNonNullContextNode_1Param,
                "string"),
            context,
            locator);

        // Dummy return value...
        return XObjectPtr();
  }

/*
 * RETURNING VALUES BACK INTO XPATH AS POINTERS TO NEW XOBJECTS
 */

  return executionContext.getXObjectFactory().createBoolean(theValue);
  return executionContext.getXObjectFactory().createNull();
  return executionContext.getXObjectFactory().createNumber(theValue);
  return executionContext.getXObjectFactory().createString(theString);
  return executionContext.getXObjectFactory().createNodeSet(theList);
  return executionContext.getXObjectFactory().createNodeSet(theNode);

/* XObject returned by getType() method.
 *
 *	enum	eObjectType { eTypeNull = 0,
 *						  eTypeUnknown = 1,
 *						  eTypeBoolean = 2,
 *						  eTypeNumber = 3,
 *						  eTypeString = 4,
 *						  eTypeNodeSet = 5,
 *						  eTypeResultTreeFrag = 6,
 *						  eTypeUserDefined = 7,
 *						  // These next types are implementation-specific, and
 *						  // are never returned by getType().
 *						  eTypeStringReference = 8,
 *						  eTypeStringAdapter = 9,
 *						  eTypeStringCached = 10,
 *						  eTypeXTokenNumberAdapter = 11,
 *						  eTypeXTokenStringAdapter = 12,
 *						  eTypeNodeSetNodeProxy = 13,
 *						  eUnknown
 *						};
 */

		// Example from FunctionString.cpp with no arguments
		//
        // The XPath standard says that if there are no arguments,
        // the argument defaults to a node set with the context node
        // as the only member.  The string value of a node set is the
        // string value of the first node in the node set.
        // DOMServices::getNodeData() will give us the data.

        // Get a cached string...
        XPathExecutionContext::GetAndReleaseCachedString    theData(executionContext);
        XalanDOMString&     theString = theData.get();
        DOMServices::getNodeData(*context, theString);
        return executionContext.getXObjectFactory().createString(theData);

		// Example from FunctionString.cpp with one argument
		//
		if (arg1->getType() == XObject::eTypeString)
		{
        // Since XObjects are reference counted, just return the
        // argument.
			return arg1;
		}
		else
		{
			return executionContext.getXObjectFactory().createStringAdapter(arg1);
		}

/* XObjectFactory(). methods */
/******************************
	bool
	ReturnObject(XObject*		theObject)
		// return an XObject to the factory

	XObjectPtr
	createBoolean(bool		theValue);

	XObjectPtr
	createNodeSet(BorrowReturnMutableNodeRefList&	theList);

	XObjectPtr
	createNodeSet(XalanNode*	theNode);

	XObjectPtr
	createNull();	// a null XObject

	XObjectPtr
	createNumber(double		theValue);

	XObjectPtr
	createNumber(XToken&		theToken);
		// theXToken instance must have a lifetime of the object or longer

	XObjectPtr
	createString(XalanDOMString&	theValue);

	XObjctPtr
	createString(XalanDOMChar*	theValue);

	XObjectPtr
	createString(XalanDOMChar*	theValue,
				int		theLength);	// possibly truncated length

	XObjectPtr
	createString(XToken&		theToken);
		// theXToken instance must have a lifetime of the object or longer

	XObjectPtr
	createStringReference(XalanDOMString&	theValue)
		// the string must be in scope for lifetime of the object

	XObjectPtr
	createStringAdapter(XobjectPtr&		theValue)
		// create XObject that behaves like a string, referencing another object

	XObjectPtr
	createString(GetAndReleaseCachedString&		theCachedString)
		// create XObject from a cached string

	XObjectPtr
	createUnknown(XalanDOMString&	theValue)
		// create unknown object from a string
***/

}

/*
 *  SUPPLY A SPECIFIC ERROR IF THE FUNCTION CANNOT BE EXECUTED
 */

const XalanDOMString&
OSPXpathFunction::getError(XalanDOMString&  theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionTakesTwoOrThreeArguments_1Param,
                "TheFunctionName()");
}



XALAN_CPP_NAMESPACE_END
