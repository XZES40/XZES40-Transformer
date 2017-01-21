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
 *    Rev 1.0   Sep 17 2009            hathaway
 * Install the original cvdate() extension function and
 * the newer virtual table XPATH extension functions.
 * 
 *    Onsite.1  Oct 24 2007            hathaway
 * Initial revision.
 * 
 *    Rev 0.1   Oct 19 2007 12:25:36   hathaway
 * Development Started.
*/

/******************************************************
 * The trick here is to have 'C' functions that know
 * how to install 'C++' classes that are defined in
 * the XALAN_CPP_NAMESPACE namespace.
 *
 * I have used XalanCAPI.h and XalanCAPI.cpp as reference.
 */

/******************************************************
 * All XPath functions must be extensions of class Function
 * defined by <xalanc/XPath/Function.hpp>.
 *
 * There are actually two methods by which XPath Functions
 * can be installed into the Xalan Transformer Supervisor.
 *
 *   XalanTransformer::installExternalFunction()
 *
 *   Makes a copy of descriptors for theNamespace and functionName.
 *   Makes a clone of the function.  And installs the function just
 *   before compiling a stylesheet.
 *
 *   XalanTransformer::installExternalFunctionGlobal()
 *
 *   Assumes that theNamespace and functionName are static constants
 *   and that the function has been initialized in static memory.
 *   The function is installed without making further copies.
 *
 * There are also two methods by which XPath Functions
 * can be uninstalled.
 *
 *  XalanTransformer::uninstallExternalFunction()
 *
 *  XalanTransformer::uninstallExternalFunctionGlobal()
 *
 * --
 * We should not use the members of class XPathEnvSupportDefault
 * directly to install/uninstall custom XPath functions
 * because that bypasses the XALAN Transformer supervisor.
 */


#include "OspXpath.h"

#include <xalanc/Include/PlatformDefinitions.hpp>
#include <xalanc/XalanTransformer/XalanTransformer.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>

/**
 * The OSPXpathFunctionLib.hpp adds the class definitions for
 * custom XPATH function extensions to the XALAN_CPP_NAMESPACE.
 *
 * This OSPXpathInstaller.cpp 
 * instantiates a global static instance of each function class
 * that can be installed into instances of XalanTransformer.
 */

#include "OspXpathFunctionLib.hpp"
// Sep-28-2009
#include "OspXpathTableToXml.hpp"

/**
 * Here is where we declare the 'C++' namespace scoped items to be
 * used in the 'C' language API.  When defining a 'C' language API,
 * our default references should be outside any 'C++' namespace scope.
 */

//XALAN_USING_XALAN(XalanMemMgrs)
//XALAN_USING_XERCES(XMLPlatformUtils)

XALAN_USING_XALAN(XalanDOMChar)
XALAN_USING_XALAN(XalanDOMString)
XALAN_USING_XALAN(XalanTransformer)

//XALAN_USING_XALAN(XalanUnicode)
//--SJH-- 3/10/2008

// I am unsure whether to keep using the XALAN_CPP_NAMESPACE !!!
// It appears to be convenient at this time in my development.
// --SJH--
// The purpose of using XALAN_CPP_NAMESPACE is to supply a set of
// default namespaces for references, not impacting the scope of
// entities being defined.  This is different than the concept of
// (begin namespace / end namespace) which modifies the scope of
// entities being defined. -- OKay to keep using namespace --SJH--

using namespace XALAN_CPP_NAMESPACE;

XALAN_USING_XALAN(OSPXpathConvertDate)

/* Namespace = "http://www.oregon.gov/OSP/CJIS/xml/xpath" */

static const XalanDOMChar	s_OSPXpathNamespace[] =
{
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_p,
	XalanUnicode::charColon,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_w,
	XalanUnicode::charLetter_w,
	XalanUnicode::charLetter_w,
	XalanUnicode::charFullStop,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	XalanUnicode::charFullStop,
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_v,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_O,
	XalanUnicode::charLetter_S,
	XalanUnicode::charLetter_P,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_C,
	XalanUnicode::charLetter_J,
	XalanUnicode::charLetter_I,
	XalanUnicode::charLetter_S,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_l,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_h,
	0
};

/**
 * The XalanDOMString equivalent of XalanDOMChar *.
 */

//static const XalanDOMString s_OSPNamespace(s_oregonOSPNamespace);

/**
 * The XPATH names of functions to be installed.
 */

static const XalanDOMChar	s_OSPXpathConvertDate[] = 
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_v,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	0
};

static const XalanDOMChar	s_OSPXpathInitTableDB[] =
{
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_D,
	XalanUnicode::charLetter_B,
	0
};

static const XalanDOMChar	s_OSPXpathDropTableDB[] =
{
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_D,
	XalanUnicode::charLetter_B,
	0
};

static const XalanDOMChar	s_OSPXpathSetTableKeyValue[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_K,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_y,
	XalanUnicode::charLetter_V,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_e,
	0
};

static const XalanDOMChar	s_OSPXpathGetTableKeyValue[] =
{
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_K,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_y,
	XalanUnicode::charLetter_V,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_e,
	0
};

static const XalanDOMChar	s_OSPXpathGetTableRowCount[] =
{
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_R,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_w,
	XalanUnicode::charLetter_C,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	0
};

static const XalanDOMChar	s_OSPXpathGetTableKeyRow[] =
{
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_K,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_y,
	XalanUnicode::charLetter_R,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_w,
	0
};

static const XalanDOMChar	s_OSPXpathGetTableRowKey[] =
{
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_R,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_w,
	XalanUnicode::charLetter_K,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_y,
	0
};

static const XalanDOMChar	s_OSPXpathGetTableRowValue[] =
{
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_R,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_w,
	XalanUnicode::charLetter_V,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_e,
	0
};

static const XalanDOMChar	s_OSPXpathSortTableKeys[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_K,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_y,
	XalanUnicode::charLetter_s,
	0
};

static const XalanDOMChar	s_OSPXpathSortTableValues[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_V,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	0
};

static const XalanDOMChar	s_OSPXpathInvertTable[] =
{
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_v,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	0
};

static const XalanDOMChar	s_OSPXpathDropTableName[] =
{
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_N,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	0
};

static const XalanDOMChar	s_OSPXpathDropTableKey[] =
{
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_K,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_y,
	0
};

static const XalanDOMChar	s_OSPXpathDropTableRow[] =
{
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_R,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_w,
	0
};

static const XalanDOMChar	s_OSPXpathGetTableToXml[] =
{
	XalanUnicode::charLetter_g,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_T,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_N,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_t,
	0
};

/**
 * The XalanDOMString equivalent of XalanDOMChar *.
 */

//static const XalanDOMString s_cvdateName(s_cvdateFunctionName);

/**
 * Initialize Functions to Global (Static) Address Space.
 */

static const OSPXpathConvertDate      s_OSPXpathConvertDateFunction;
static const OSPXpathInitTableDB      s_OSPXpathInitTableDBFunction;
static const OSPXpathDropTableDB      s_OSPXpathDropTableDBFunction;
static const OSPXpathGetTableRowCount s_OSPXpathGetTableRowCountFunction;
static const OSPXpathSetTableKeyValue s_OSPXpathSetTableKeyValueFunction;
static const OSPXpathGetTableKeyValue s_OSPXpathGetTableKeyValueFunction;
static const OSPXpathGetTableKeyRow   s_OSPXpathGetTableKeyRowFunction;
static const OSPXpathGetTableRowKey   s_OSPXpathGetTableRowKeyFunction;
static const OSPXpathGetTableRowValue s_OSPXpathGetTableRowValueFunction;
static const OSPXpathSortTableKeys    s_OSPXpathSortTableKeysFunction;
static const OSPXpathSortTableValues  s_OSPXpathSortTableValuesFunction;
static const OSPXpathInvertTable      s_OSPXpathInvertTableFunction;
static const OSPXpathDropTableName    s_OSPXpathDropTableNameFunction;
static const OSPXpathDropTableKey     s_OSPXpathDropTableKeyFunction;
static const OSPXpathDropTableRow     s_OSPXpathDropTableRowFunction;
// Sep-28-2009
static const OSPXpathGetTableToXml    s_OSPXpathGetTableToXmlFunction;

/**
 * Note: OSPXpathInstall is a 'C' language API reference and is therefore
 * outside any 'C++' namespace scope.
 */

void
OSPXpathInstall(XalanHandle theHandle, MemLocal insMem)
{
	XalanTransformer * xalan = (XalanTransformer *) theHandle;

	switch (insMem)
	{
	case local:
		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathConvertDate),
			s_OSPXpathConvertDateFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathInitTableDB),
			s_OSPXpathInitTableDBFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableDB),
			s_OSPXpathDropTableDBFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableRowCount),
			s_OSPXpathGetTableRowCountFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathSetTableKeyValue),
			s_OSPXpathSetTableKeyValueFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableKeyValue),
			s_OSPXpathGetTableKeyValueFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableKeyRow),
			s_OSPXpathGetTableKeyRowFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableRowKey),
			s_OSPXpathGetTableRowKeyFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableRowValue),
			s_OSPXpathGetTableRowValueFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathSortTableKeys),
			s_OSPXpathSortTableKeysFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathSortTableValues),
			s_OSPXpathSortTableValuesFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathInvertTable),
			s_OSPXpathInvertTableFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableName),
			s_OSPXpathDropTableNameFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableKey),
			s_OSPXpathDropTableKeyFunction);

		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableRow),
			s_OSPXpathDropTableRowFunction);

// Sep-28-2009
		xalan->installExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableToXml),
			s_OSPXpathGetTableToXmlFunction);

		break;
	case global:
		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathConvertDate),
			s_OSPXpathConvertDateFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathInitTableDB),
			s_OSPXpathInitTableDBFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableDB),
			s_OSPXpathDropTableDBFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableRowCount),
			s_OSPXpathGetTableRowCountFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathSetTableKeyValue),
			s_OSPXpathSetTableKeyValueFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableKeyValue),
			s_OSPXpathGetTableKeyValueFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableKeyRow),
			s_OSPXpathGetTableKeyRowFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableRowKey),
			s_OSPXpathGetTableRowKeyFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableRowValue),
			s_OSPXpathGetTableRowValueFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathSortTableKeys),
			s_OSPXpathSortTableKeysFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathSortTableValues),
			s_OSPXpathSortTableValuesFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathInvertTable),
			s_OSPXpathInvertTableFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableName),
			s_OSPXpathDropTableNameFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableKey),
			s_OSPXpathDropTableKeyFunction);

		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableRow),
			s_OSPXpathDropTableRowFunction);

// Sep-28-2009
		xalan->installExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableToXml),
			s_OSPXpathGetTableToXmlFunction);

		break;
	}
}

/**
 * Note: OSPXpathUninstall is a 'C' language API reference and is therefore
 * outside any 'C++' namespace scope.
 */

void
OSPXpathUninstall(XalanHandle theHandle, MemLocal insMem)
{
	XalanTransformer * xalan = (XalanTransformer *) theHandle;

	switch (insMem)
	{
	case local:
		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathConvertDate));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathInitTableDB));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableDB));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableRowCount));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathSetTableKeyValue));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableKeyValue));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableKeyRow));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableRowKey));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableRowValue));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathSortTableKeys));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathSortTableValues));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathInvertTable));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableName));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableKey));

		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableRow));

// Sep-28-2009
		xalan->uninstallExternalFunction(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableToXml));

		break;
	case global:
		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathConvertDate));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathInitTableDB));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableDB));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableRowCount));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathSetTableKeyValue));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableKeyValue));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableKeyRow));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableRowKey));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableRowValue));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathSortTableKeys));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathSortTableValues));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathInvertTable));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableName));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableKey));

		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathDropTableRow));

// Sep-28-2009
		xalan->uninstallExternalFunctionGlobal(
			XalanDOMString(s_OSPXpathNamespace),
			XalanDOMString(s_OSPXpathGetTableToXml));

		break;
	}
}

