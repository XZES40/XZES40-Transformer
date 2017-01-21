

#if !defined(OSP_XPATH_TABLE_TO_XML_HEADER_GUARD_20090929)
#define OSP_XPATH_TABLE_TO_XML_HEADER_GUARD_20090929

// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>

// #include <cfloat>

// Base class header file...

#include <xalanc/Include/XalanMemoryManagement.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/XSLT/FunctionDocument.hpp>

XALAN_CPP_NAMESPACE_BEGIN

class OSPXpathGetTableToXml : public FunctionDocument
{
public:
	OSPXpathGetTableToXml();

	virtual
	~OSPXpathGetTableToXml();

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
	virtual OSPXpathGetTableToXml*
#endif

	clone(MemoryManagerType& theManager) const;

protected:

	virtual const XalanDOMString&
	getError(XalanDOMString& theResult) const;

private:

	// Not implemented...

	OSPXpathGetTableToXml &
	operator=(const OSPXpathGetTableToXml&);

	bool
	operator==(const OSPXpathGetTableToXml&);

//	static const XObjectPtr			s_nullXObjectPtr;
};


XALAN_CPP_NAMESPACE_END

#endif //OSP_XPATH_TABLE_TO_XML_HEADER_GUARD_20090929