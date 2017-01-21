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
 *    Onsite.1  Oct 24 2007            hathaway
 * Initial revision.
 * 
 *    Rev 0.1   Oct 19 2007 12:25:36   hathaway
 * Development Started.
*/

/* The C language date conversion function is defined in "orcvdate.h".
 * The return string is in global static memory.  The ORcvdate function
 * is not thread safe.  All calls to ORcvdate overwrite the static
 * return string. The "indate" argument can be most anything that looks
 * like a date.  If "indate" cannot be parsed, its contents are returned
 * without change. This allows NCIC date codes of "NX", "NONEXP", "NOEXPR"
 * to indicate non-expiring dates, useful for special licenses, permits,
 * and registrations.
 */

/* Defines: const char * ORcvdate(
 *   char * indate,		= the input date string in ASCII
 *   int  * inlen,		= the length of the input date string
 *   char * fmt,		= the output format code
 *   int  * cwin,		= floating window offset from this year
 *   char * tzone);		= a timezone notation
 *
 * The Output Format Codes
 *   YR2   "YY"             2-digit year
 *   YR4   "CCYY"           4-digit year
 *   FD6   "YYMMDD"         6-digit date year first
 *   FD8   "CCYYMMDD"       8-digit date ccyy first
 *   FH6   "YY-MM-DD"       6-digit hyphenated year first
 *   FH8   "CCYY-MM-DD"     8-digit hyphenated ccyy first
 *   ND6   "MMDDYY"         6-digit date year last
 *   ND8   "MMDDCCYY"       8-digit date ccyy last
 *   NH6   "MM-DD-YY"       6-digit hyphenated year last
 *   NH8   "MM-DD-CCYY"     8-digit hyphenated ccyy last
 *   IDT   "CCYY-MM-DDT00:00:00"  Template XML No Timezone
 *   MYR   "MON CCYY"       abrvMonth + CCYY, no day of month
 *   MXR   "MONTH CCYY"     longMonth + CCYY, no day of month
 *   MDY   "MON DD CCYY"    abrvMonth day ccyy
 *   MDX   "MONTH DD CCYY"  longMonth day ccyy
 */

#include "orcvdate.h"


#include "OspXpathFunctionLib.hpp"

#include <xalanc/XPath/XPathEnvSupportDefault.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>

// For conditional compile - xalan version dependencies
#include <xalanc/Include/XalanVersion.hpp>

XALAN_CPP_NAMESPACE_BEGIN

//const XObjectPtr	OSPXpathConvertDate::s_nullXObjectPtr(0);


OSPXpathConvertDate::OSPXpathConvertDate():
 Function()
{
}

OSPXpathConvertDate::~OSPXpathConvertDate()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
OSPXpathConvertDate*
#endif
OSPXpathConvertDate::clone(MemoryManagerType&    theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}

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

/* XPath = "function(arg1)" or "function(arg1, arg2)" or "function(arg1, arg2, arg3)"
 * example1 = "lx:cvdate(PATH/TO/DOB)"
 * example2 = "lx:cvdate(PATH/TO/DOB, 'FMT')"
 * example3 = "lx:cvdate(PATH/TO/DOB, 'FMT', 'YRWIN')"
 *
 * The PATH/TO/DOB element can be any 2,4,6,7,8,10 character date format
 * including formats where names of months are spelled out or abbreviated.
 * Default output format name = 'FH8' {CCYY-MM-DD}
 * Default floating year window = '+10' {today plus 10 years}
 */

XObjectPtr
OSPXpathConvertDate::execute(
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


/* XPath = "function(arg1)" 
 * example = "lx:cvdate(PATH/TO/DOB)"
 * arg2 defaults to 'FH8'
 * arg3 defaults to '+10'
 */

XObjectPtr
OSPXpathConvertDate::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *			context,
	const XObjectPtr		arg1,
	const LocatorType *		locator) const
{
  assert(arg1.null() == false);
  return execute(executionContext, context, arg1,
	XObjectPtr(0), XObjectPtr(0), locator);
}

/* XPath = "function(arg1, arg2)" 
 * example = "lx:cvdate(PATH/TO/DOB, 'FMT')"
 * arg3 defaults to '+10'
 */

XObjectPtr
OSPXpathConvertDate::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *			context,
	const XObjectPtr		arg1,
	const XObjectPtr		arg2,
	const LocatorType *		locator) const
{
  assert(arg1.null() == false);
  if (arg2.null() == true)
	return execute(executionContext, context, arg1,
	XObjectPtr(0), XObjectPtr(0), locator);
  else
	return execute(executionContext, context, arg1, arg2,
	XObjectPtr(0), locator);
}

/* XPath = "function(arg1, arg2, arg3)"
 * example = "lx:cvdate(PATH/TO/DOB, 'FMT', '-3')" 
 *   where arg1 contains the XObject text to convert (text element)
 *   and   arg2 contains the output format name (string)
 *   and   arg3 contains the floating window offset (number)
 */

XObjectPtr
OSPXpathConvertDate::execute(
	XPathExecutionContext &		executionContext,
	XalanNode *,			/* context - not used */
	const XObjectPtr		arg1,	/* the unconverted source date */
	const XObjectPtr		arg2,   /* the conversion format */
	const XObjectPtr		arg3,   /* the +- floating year window */
	const LocatorType *		/* locator - not used */) const
{

	if (arg1.null() == true)
	{
		return XObjectPtr(0);	// not a useful XObject type
	}

	CharVectorType	theSourceDate;
	char *			charSourceDate;
	CharVectorType	theConvFormat;
	char *			charConvFormat;
	CharVectorType	theYearWindow;
	char *			charYearWindow;
	int iYearWindow;

// The XALAN/XERCES Memory Allocation Manager.
	MemoryManagerType &		theManager = executionContext.getMemoryManager();
// Now to get the arguments into 'C' string format.

	theSourceDate = TranscodeToLocalCodePage(arg1->str());
	charSourceDate = theSourceDate.begin();

	if (arg2.null() == true)
	{
		charConvFormat = "FH8";		// Default = "CCYY-MM-DD"
	}
	else
	{
		theConvFormat = TranscodeToLocalCodePage(arg2->str());
		charConvFormat = theConvFormat.begin();
		if (strlen(charConvFormat) == 0)
			charConvFormat = "FH8";	// Default = "CCYY-MM-DD"
	}

	if (arg3.null() == true)
		iYearWindow = 10; // (today + window) = previous century
	else
	{
		theYearWindow = TranscodeToLocalCodePage(arg3->str());
		charYearWindow = theYearWindow.begin();
		if (strlen(charYearWindow) > 0)
			iYearWindow = atoi(charYearWindow);
		else
			iYearWindow = 10;	// Floating Year Window
	}

  /* Call the 'C' language function to convert dates */

	const char * charResult = ORcvdate(
	  charSourceDate,		// the unconverted date string
	  strlen(charSourceDate),	// length of unconverted date string
	  charConvFormat,		// the conversion format code
	  iYearWindow);			// the floating year window

	if (charResult == charSourceDate)
	{	// no format conversion took place, so we can just return arg1.
		return arg1;	// the XObjects are count referenced.
	}

  /* Create a string structure from the 'C' function formatted result */

	XalanDOMString theResult(charResult);
   
  /* If the 'C' function allocates (charResult) via malloc(), we must free() it. */
  /* Note: ORcvdate() returns either the charUnconvDate or a static allocation. */
  /* Keep 'C' memory allocation separate from Apache XALAN/XERCES allocation. */
  
  /*
   *  if (charResult != charSourceDate)
   *	free(charResult);
   */

  /* Return a new XObject containing the converted date string */

	return executionContext.getXObjectFactory().createString(theResult);
  
  /* Note: theResult should automatically be deallocated on return */
}

/*
 *  SUPPLY A SPECIFIC ERROR IF THE FUNCTION CANNOT BE EXECUTED
 */

const XalanDOMString&
OSPXpathConvertDate::getError(XalanDOMString&    theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionTakesTwoOrThreeArguments_1Param,
				"local:cvdate(anydate,fmt,ywin)");
}


XALAN_CPP_NAMESPACE_END
