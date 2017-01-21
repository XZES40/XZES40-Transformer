/*
 *
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

#if !defined(OSPXPATH_FCN_HEADER_GUARD_20071026)
#define OSPXPATH_FCN_HEADER_GUARD_20071026

// Base header file.  Must be first.

#include "OspXpathDefinitions.hpp"

// Base class header files...

/* XalanMemoryManagement.hpp defines the following classes::
 *   XalanAllocationGuard::
 *   XalanMemMgrs::
 *
 *   and templates for
 *   XalanDestroy()
 *   XalanConstruct()
 *   XalanCopyConstruct()
 *   struct ConstructValueWithNoMemoryManager
 *   struct ConstructValueWithMemoryManager
 *   struct ConstructWithNoMemoryManager
 *   struct ConstructWithMemoryManager
 *   struct MemoryManagedConstructionTraits
 *   struct ConstructWithMemoryManagerTraits
 *   struct ConstructWithNoMemoryManagerTraits
 */
#include <xalanc/Include/XalanMemoryManagement.hpp>

/* Function.hpp includes the following files
 *   <xalanc/XPath/XPathDefinitions.hpp>
 *   <xalanc/Include/XalanMemMgrAutoPtr.hpp>
 *   <vector>
 *   <xalanc/XPath/XObject.hpp>
 *   <xalanc/XPath/XPathExecutionContext.hpp>
 *
 *   and declares usage of
 *   XALAN_DECLARE_XERCES_CLASS(Locator)
 *   XALAN_DECLARE_XALAN_CLASS(XalanNode)
 *
 *   and defines the Function:: class
 *
 *   The Function::execute(...) methods return execution errors
 *   unless overridden by the actual XPATH function classes.
 *
 *   The execute(...) method with arg vector is always called
 *   by the XPATH interpreter before dispatching to the other
 *   execute(...) methods.
 */
#include <xalanc/XPath/Function.hpp>

XALAN_CPP_NAMESPACE_BEGIN

/**
 * Template implementation of an arbitry XPATH "lx:function(...)"
 *
 * The function implementation class is OSPXpathFunction with
 * execute callbacks.  The function is given an XPATH name
 * and associated namespace when the function class is installed.
 * The XPATH name is separate from the function class name.
 */

class OSP_XPATH_EXPORT OSPXpathFunction : public Function
{
public:

	OSPXpathFunction();
	
	virtual
	~OSPXpathFunction();
	
#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

// The execute functions are inherited from parent Function class

// With Argument Vector - Always called from XPATH processor
	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

// With No Arguments
	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const LocatorType *			locator) const;

// With one Argument
	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const LocatorType *			locator) const;

// With two Arguments
	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const XObjectPtr			arg2,
		const LocatorType *			locator) const;

// With three Arguments
	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const XObjectPtr			arg2,
		const XObjectPtr			arg3,
		const LocatorType *			locator) const;

/* If more than three arguments are required, you must
 * override the 'execute' with arg vector to accommodate
 * the additional arguments.
 */

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathFunction*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathFunction &
	operator=(const OSPXpathFunction&);

	bool
	operator==(const OSPXpathFunction&);

	static const XObjectPtr			s_nullXObjectPtr;
};

XALAN_CPP_NAMESPACE_END

#endif // OSPXPATH_FCN_HEADER_GUARD_20071026

