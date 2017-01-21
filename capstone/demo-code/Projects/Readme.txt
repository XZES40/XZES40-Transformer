
			COMMAND-LINE XSLT TRANSFORMER
		    COMPILED WITH LOCAL XPATH EXTENSIONS

File:	exslt.exe

XPath Extensions Namespace:	'http://www.oregon.gov/OSP/CJIS/xml/xpath'

XPath Virtual Table XML:	'http://www.oregon.gov/OSP/CJIS/xml/keytable'


Virtual Table Dump as Temporary XML File and imported as a nodeset document


XPath-1.1 Local Extensions

  Namespace = 'http://www.oregon.gov/OSP/CJIS/xml/xpath'
  Prefix (lx:)  xmlns:lx='http://www.oregon.gov/OSP/CJIS/xml/xpath'


XPATH - GENERAL DATE TRANSFORMATION FUNCTION


XPathFunction: lx:cvdate

  Recognize most any standard date string and format an appropriate output
  according to a specified format identifier.

  arg1  required	xs:string	year and/or date to be formatted
  arg2  optional	xs:token	3-char output formatting identifier
			default = 'FH8'		format = CCYY-MM-DD
  arg3	optional	xs:integer	floating century window from TODAY +2days +2hours
			default = '+10'
  return		xs:string	the formatted date


Here are the format identifier codes that are currently implemented by the cvdate function.

	"YR2"  YY
	"YR4"  CCYY
	"FD6"  YYMMDD
	"FD8"  CCYYMMDD
	"FH6"  YY-MM-DD
	"FH8"  CCYY-MM-DD
	"ND6"  MMDDYY
	"ND8"  MMDDCCYY
	"NH6"  MM-DD-YY
	"NH8"  MM-DD-CCYY
	"FS6"  YY/MM/DD
	"FS8"  CCYY/MM/DD
	"NS6"  MM/DD/YY
	"NS8"  MM/DD/CCYY
	"MYR"  MON CCYY
	"MXR"  MONTH CCYY
	"MDY"  MON DD CCYY
	"MDX"  MONTH DD, CCYY
	"DM6"  DD MON YY
	"DM8"  DD MON CCYY
	"DX6"  DD MONTH YY
	"DX8"  DD MONTH CCYY 
	"IDT"  CCYY-MM-DDT00:00:00
	"LYR"  Mon CCYY
	"LXR"  Month CCYY
	"LDY"  Mon DD CCYY
	"LDX"  Month DD, CCYY
	"DL6"  DD Mon YY
	"DL8"  DD Mon CCYY
	"XL6"  DD Month YY
	"XL8"  DD Month CCYY
	"DH6"  DD-MM-YY
	"DH8"  DD-MM-CCYY
	"DS6"  DD/MM/YY
	"DS8"  DD/MM/CCYY

The input string uses pattern recognition rules to determine the input string format.
Most of the common date strings are properly interpreted.  Here are some examples.
A string of six digits can be MMDDYY or YYMMDD noting that MMDDYY is assumed if the
first 2 digits are in the range of {0 .. 12}.  The month of January can be
recognized as input in any of these forms {01 JAN Jan JANUARY January}.  Items that
cannot be interpreted as dates are returned unchanged to the output string.
An eight digit date on input can be MMDDCCYY or CCYYMMDD with a test of the first
two characters being in the range of {0 .. 12}.

Some bizzare situations can arrise because there is no attempt to ensure that the
days in a month are valid and consistent.  Month of zero and Day of zero are allowed.
A three-digit year is also interpreted such that the most-significant digit is an
offset added to century 19.  Characters of punctuation can separate parts of a date
on input and can be any of several common characters.  Special dates of YESTERDAY,
TODAY, and TOMORROW are determined by the system clock when the cvdate function is
invoked.


XPATH - GENERAL VIRTUAL DATABASE KEY-VALUE TABLE FUNCTIONS

XPath Function: lx:initTableDB

  Remove all resorces owned by the Virtual Database subsystem.

  arg[]	none
  return		empty-string


XPath Function: lx:dropTableDB

  Remove all resources owned by the Virtual Database subsystem.

  arg[] none
  return		empty-string


XPath Function: lx:setTableKeyValue

  Create a virtual table and assign or update a key-value pair.

  arg1	required	xs:string	theTable name-string
  arg2	required	xs:string	theKey name-string
  arg3	required	xs:string	theValue value-string
  return		empty-string

XPath Function: lx:getTableKeyValue

  Return the value-string associated with theTable and theKey.

  arg1	required	xs:string	theTable name-string
  arg2	required	xs:string	theKey name-string

XPath Function: lx:getTableRowCount

  Return the number of key-value pairs in the virtual table.

  arg1	required	xs:string	theTable name-string
  return		xs:nonNegativeInteger	number of key-value pairs

XPath Function: lx:getTableKeyRow

  Return the value-string from theTable associated with theRow Number.
  A return value of zero indicates the Key value has not been assigned.

  arg1	required	xs:string	theTable name-string
  arg2  required	xs:string	theKey name-string
  return		xs:nonNegativeInteger	theRow number

XPath Function: lx:getTableRowKey

  Return theKey name-string associated with theTable name and theRow number.

  arg1  required	xs:string	theTable name-string
  arg2  required	xs:nonNegativeInteger	theRowNumber
  return		xs:string	theKey name-string

XPath Function: lx:getTableRowValue

  Return the value-string associated with theTable name and theRow number.

  arg1  required	xs:string	theTable name-string
  arg2	required	xs:nonNegativeInteger  theRowNumber
  return		xs:string	theValue string

XPath Function: lx:sortTableKeys

  Sort the specified virtual table by theKey name-string.
  The table is sorted in ascending order.

  arg1	required	xs:string	theTable name-string
  return		empty-string

XPath Function: lx:sortTableValues

  Sort the specified virtual table by the-value strings.
  The table is sorted in descending order.

  arg1	required	xs:string	theTable name-string
  return		empty-string

XPath Function: lx:invertTable

  The table rows are swapped.  A table that has ascending order becomes
  renumbered to descending order.

  arg1  required	xs:string	theTable name-string
  return		empty-string

XPath Function: lx:dropTableName

  Remove the virtual table from the virtual database.

  arg1  required	xs:string	theTable name-string
  return		empty-string

XPath Function: lx:dropTableKey

  Remove the specified key-name identified row from theTable.

  arg1	required	xs:string	theTable name-string
  arg2	required	xs:string	theKey name-string
  return		empty-string

XPath Function: lx:dropTableRow

  Remove the specified row from theTable identified by theRow number.

  arg1	required	xs:string	theTable name-string
  arg2	required	xs:nonNegativeInteger	theRow number
  return		empty-string

XPath Function: lx:getTableAsNodeset

  Copy the table to a temporary XML file and insert it into the stylesheet
  as a nodeset.  XALAN stylesheets allow multiple XML documents to be
  merged and processed in a single XSLT stylesheet.  The getTableAsNodeset
  converts a virtual key-value table to a nodeset.

  arg1	required	xs:string	theTable name-string
  return		xs:nodeset	XPath nodeset of TheTable

  Note:  The scope of a nodeset returned by an XPath function is the
  element in which the XPath function is referenced.  The nodeset is created
  within the scope of processing the stylesheet element and is destroyed 
  when the stylesheet element is ended.

  The temporary XML file created by the lx:getTableAsNodeset is validated
  according to Namespace 'http://www.oregon.gov/OSP/CJIS/xml/keytable'.
  File: keytable.dtd is the authoritative DOCTYPE definition for validation.
  File: keytable.xsd is the authoritative Schema definition for validation.

---------------------------------------------------------------------------------

EXAMPLE OF TEMPORARY FILE FOR VIRTUAL TABLE.XML
---------------------------------------------------------------------------------
  <?xml version='1.0' encoding='UTF-8'?>
  <ktb:table xmlns:ktb='http://www.oregon.gov/OSP/CJIS/xml/keytable'
       table='tablename' file='full-path-to-temporary.xml'>
  <ktb:row table='tablename' row='rownbr' key='key-string' value='value-string'/>
  <!-- ktb:row is repeated for each row in the virtual table -->
  </ktb:table>
---------------------------------------------------------------------------------

EXAMPLE OF A KEYTABLE FILE THAT CAN BE VALIDATED WITH DTD AND SCHEMA RULES
---------------------------------------------------------------------------------
<?xml version='1.0' encoding='UTF-8'?>
<!DOCTYPE ktb:table SYSTEM 'keytable.dtd'
[ <!ENTITY % ns 'ktb' ]>
<ktb:table xmlns:ktb='http"//www.oregon.gov/OSP/CJIS/xml/keytable'
    xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'
    xsi:schemaLocation='http://www.oregon.gov/OSP/CJIS/xml/keytable keytable.xsd'
    table='tablename' file='the-xml-file-uri'>
<ktb:row table='tablename' row='1' key='keyId1' value='value-string for keyId1'/>
<ktb:row table='tablename' row='2' key='keyId2' value='value-string for keyId2'/>
<ktb:row table='tablename' row='3' key='keyId3' value='value-string for keyId3'/>
<ktb:row table='tablename' row='4' key='keyId4' value='value-string for keyId4'/>
</ktb:table>
---------------------------------------------------------------------------------


  



