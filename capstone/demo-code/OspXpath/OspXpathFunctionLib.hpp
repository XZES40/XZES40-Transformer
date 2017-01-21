/*
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
 *
 */


/**
 * This header file defines the C++ classes to implement a library
 * of XPath 1.1 extension functions to greatly simplify stylesheets
 * and improve system performance for common tasks.
 *
 * Each function is an extension of class: Function in the XALAN_CPP_NAMESPACE
 *
 *
 * Function: cvdate()			string
 *
 * Function: initTableDB()		empty
 * Function: dropTableDB()		empty
 * Function: setTableKeyValue()		empty
 * Function: getTableKeyValue()		string
 * Function: getTableRowCount()		integer
 * Function: getTableKeyRow()		integer
 * Function: getTableRowKey()		string
 * Function: getTableRowValue()		string
 * Function: sortTableKeys()		empty
 * Function: sortTableValues()		empty
 * Function: invertTable()		empty
 * Function: dropTableName()		empty
 * Function: dropTableKey()		empty
 * Function: dropTableRow()		empty
 *
 * Function: getTableAsNodeset()	nodeset
 *
 */

#if !defined(OSP_XPATH_EXTENSIONS_HEADER_GUARD_20090910)
#define OSP_XPATH_EXTENSIONS_HEADER_GUARD_20090910

// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>

#include <cfloat>

// Base class header file...

#include <xalanc/Include/XalanMemoryManagement.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>

XALAN_CPP_NAMESPACE_BEGIN

/**
 * XPATH EXTENSION FUNCTION - USING THE APACHE XALAN/C LIBRARY
 *
 * Date Conversion:	STRING = lx:cvdate(string [,format [,window]])
 *
 *	Can be called with 1, 2, or 3 arguments
 *	Default format = "FHY" resulting in "CCYY-MM-DD" output format.
 *	Default window = +10 if the string value is empty.
 *
 */

class OSPXpathConvertDate : public Function
{
public:
	OSPXpathConvertDate();

	virtual
	~OSPXpathConvertDate();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const XObjectPtr			arg2,
		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const XObjectPtr			arg2,
		const XObjectPtr			arg3,
		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathConvertDate*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathConvertDate &
	operator=(const OSPXpathConvertDate&);

	bool
	operator==(const OSPXpathConvertDate&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


/**
 * IN-MEMORY TABLE EMULATION FUNCTIONS - XPATH EXTENSIONS
 *
 * These XPath extension functions operate on global linked lists in the process
 * to simulate data tables for XPath expressions. 
 *
 * Function:	void = initTableDB()
 *
 *	- purges all virtual tables - identical to dropTableDB().
 *
 * Function:	void = dropTableDB()
 *
 *	- purges all virtual tables - identical to initTableDB().
 *
 * Function:	index = setTableValue("table", "key", "value")
 *
 *	- create a "table" and associate a "key" and "value" pair
 *	- returning the row number that is created or replaced.
 *	- row numbers start at 1.  Value of 0 means error or undefined.
 *
 * Function:	"value" = getTableValue("table", "key")
 *
 *	- returns the "value" from the "table" identified by "key".
 *	- the empty string is returned if no "key" "value" pair exists.
 *
 * Function:	boolean = testTableKey("table", "key")
 *
 *	- returns "true" if "key" has been created by setTableValue()
 *
 * Function:	index = getTableRowCount("table")
 *
 *	- returns the number of rows in the "table"
 *	- returns zero if the "table" does not exist.
 *
 * Function:	index = getTableKeyIndex("table", "key")
 *
 *	- returns the row number index associated with "key"
 *
 * Function:	"key" = getTableIndexKey("table", "index")
 *
 *	- returns the "key" associated with the row index number.
 *
 * Function:	"value" = getTableIndexValue("table", "index")
 *
 *	- returns the "value" from the table with the row index number.
 *
 * Function:	void = dropTableName("table")
 *
 *	- removes the table from the virtual database and its allocated resources
 *
 * Function:	void = dropTableKey("table","key")
 *
 *	- removes the row from the "table" identified by "key"
 *	- the internal "index" values are recomputed
 *
 * Function:	void = dropTableIndex("table","index")
 *
 *	- removes the row from the "table" identified by "index" row number
 *	- the internal "index" values are recomputed
 *
 * Function:	void = sortTableKeys("table")
 *
 *	- sorts the table by "key" values in ascending order.
 *	- the internal "index" values are recomputed
 *
 * Function:	void = sortTableValues("table")
 *
 *	- sorts the table by "values" assigned to the rows in ascending order.
 *	- the internal "index" values are recomputed
 *
 *
 * Function:	void = invertTable("table")
 *
 *	- inverts the row order of the "table".
 *	- An ascending sort order becomes a descending sort order.
 *	- the internal "index" values are recomputed.
 */

//==========================================================================

class OSPXpathInitTableDB : public Function
{
public:
	OSPXpathInitTableDB();

	virtual
	~OSPXpathInitTableDB();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const XObjectPtr			arg3,
//		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathInitTableDB*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathInitTableDB &
	operator=(const OSPXpathInitTableDB&);

	bool
	operator==(const OSPXpathInitTableDB&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================

class OSPXpathDropTableDB : public Function
{
public:
	OSPXpathDropTableDB();

	virtual
	~OSPXpathDropTableDB();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const XObjectPtr			arg3,
//		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathDropTableDB*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathDropTableDB &
	operator=(const OSPXpathDropTableDB&);

	bool
	operator==(const OSPXpathDropTableDB&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================

class OSPXpathSetTableKeyValue : public Function
{
public:
	OSPXpathSetTableKeyValue();

	virtual
	~OSPXpathSetTableKeyValue();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const XObjectPtr			arg2,
		const XObjectPtr			arg3,
		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathSetTableKeyValue*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathSetTableKeyValue &
	operator=(const OSPXpathSetTableKeyValue&);

	bool
	operator==(const OSPXpathSetTableKeyValue&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================

class OSPXpathGetTableKeyValue : public Function
{
public:
	OSPXpathGetTableKeyValue();

	virtual
	~OSPXpathGetTableKeyValue();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const XObjectPtr			arg2,
		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const XObjectPtr			arg3,
//		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathGetTableKeyValue*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathGetTableKeyValue &
	operator=(const OSPXpathGetTableKeyValue&);

	bool
	operator==(const OSPXpathGetTableKeyValue&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================

class OSPXpathGetTableRowCount : public Function
{
public:
	OSPXpathGetTableRowCount();

	virtual
	~OSPXpathGetTableRowCount();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const XObjectPtr			arg3,
//		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathGetTableRowCount*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathGetTableRowCount &
	operator=(const OSPXpathGetTableRowCount&);

	bool
	operator==(const OSPXpathGetTableRowCount&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================

class OSPXpathGetTableKeyRow : public Function
{
public:
	OSPXpathGetTableKeyRow();

	virtual
	~OSPXpathGetTableKeyRow();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const XObjectPtr			arg2,
		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const XObjectPtr			arg3,
//		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathGetTableKeyRow*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathGetTableKeyRow &
	operator=(const OSPXpathGetTableKeyRow&);

	bool
	operator==(const OSPXpathGetTableKeyRow&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================

class OSPXpathGetTableRowKey : public Function
{
public:
	OSPXpathGetTableRowKey();

	virtual
	~OSPXpathGetTableRowKey();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const XObjectPtr			arg2,
		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const XObjectPtr			arg3,
//		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathGetTableRowKey*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathGetTableRowKey &
	operator=(const OSPXpathGetTableRowKey&);

	bool
	operator==(const OSPXpathGetTableRowKey&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================

class OSPXpathGetTableRowValue : public Function
{
public:
	OSPXpathGetTableRowValue();

	virtual
	~OSPXpathGetTableRowValue();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const XObjectPtr			arg2,
		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const XObjectPtr			arg3,
//		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathGetTableRowValue*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathGetTableRowValue &
	operator=(const OSPXpathGetTableRowValue&);

	bool
	operator==(const OSPXpathGetTableRowValue&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================

class OSPXpathDropTableName : public Function
{
public:
	OSPXpathDropTableName();

	virtual
	~OSPXpathDropTableName();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const XObjectPtr			arg3,
//		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathDropTableName*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathDropTableName &
	operator=(const OSPXpathDropTableName&);

	bool
	operator==(const OSPXpathDropTableName&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================

class OSPXpathDropTableKey : public Function
{
public:
	OSPXpathDropTableKey();

	virtual
	~OSPXpathDropTableKey();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const XObjectPtr			arg2,
		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const XObjectPtr			arg3,
//		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathDropTableKey*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathDropTableKey &
	operator=(const OSPXpathDropTableKey&);

	bool
	operator==(const OSPXpathDropTableKey&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================

class OSPXpathDropTableRow : public Function
{
public:
	OSPXpathDropTableRow();

	virtual
	~OSPXpathDropTableRow();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const XObjectPtr			arg2,
		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const XObjectPtr			arg3,
//		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathDropTableRow*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathDropTableRow &
	operator=(const OSPXpathDropTableRow&);

	bool
	operator==(const OSPXpathDropTableRow&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================

class OSPXpathSortTableKeys : public Function
{
public:
	OSPXpathSortTableKeys();

	virtual
	~OSPXpathSortTableKeys();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const XObjectPtr			arg3,
//		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathSortTableKeys*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathSortTableKeys &
	operator=(const OSPXpathSortTableKeys&);

	bool
	operator==(const OSPXpathSortTableKeys&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================

class OSPXpathSortTableValues : public Function
{
public:
	OSPXpathSortTableValues();

	virtual
	~OSPXpathSortTableValues();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const XObjectPtr			arg3,
//		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathSortTableValues*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathSortTableValues &
	operator=(const OSPXpathSortTableValues&);

	bool
	operator==(const OSPXpathSortTableValues&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================

class OSPXpathInvertTable : public Function
{
public:
	OSPXpathInvertTable();

	virtual
	~OSPXpathInvertTable();

#if !defined(XALAN_NO_USING_DECLARATION)
	using Function::execute;
#endif

	virtual XObjectPtr
	execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context,
		const XObjectArgVectorType&		args,
		const LocatorType*				locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const LocatorType *			locator) const;

	virtual XObjectPtr
	execute(
		XPathExecutionContext &		executionContext,
		XalanNode *					context,
		const XObjectPtr			arg1,
		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const LocatorType *			locator) const;

//	virtual XObjectPtr
//	execute(
//		XPathExecutionContext &		executionContext,
//		XalanNode *					context,
//		const XObjectPtr			arg1,
//		const XObjectPtr			arg2,
//		const XObjectPtr			arg3,
//		const LocatorType *			locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual OSPXpathInvertTable*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathInvertTable &
	operator=(const OSPXpathInvertTable&);

	bool
	operator==(const OSPXpathInvertTable&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


//========================================================================




XALAN_CPP_NAMESPACE_END

#endif //OSP_XPATH_EXTENSIONS_HEADER_GUARD_20090910


