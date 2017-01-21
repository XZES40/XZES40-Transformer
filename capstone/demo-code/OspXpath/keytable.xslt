<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.00"
     xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
     xmlns:lx="http://www.oregon.gov/OSP/CJIS/xml/xpath"
     xmlns:TBL="http://www.oregon.gov/OSP/CJIS/xml/keytable">
 <xsl:import href="keytable.tmpl"/>
 <xsl:strip-space elements="*"/>
 <xsl:output method="xml" omit-xml-declaration="yes"/>
 <xsl:template match='/'>
   <xsl:call-template name="LoadTableABC"/>
   <xsl:call-template name="LoadColorTable"/>
   <xsl:variable name="abcRows" select="lx:getTableRowCount('TableABC')"/>
   <xsl:variable name="colorRows" select="lx:getTableRowCount('TableColor')"/>
<xsl:text>

THIS IS A TEST OF VIRTUAL TABLE TO XML NODESET CREATION

</xsl:text>
   <!-- The <copy-of> element can be used to serialize a nodeset to a file -->
   <xsl:copy-of select="lx:getTableToNodeset('TableColor')"/>
<xsl:text>

THIS IS A TEST OF STANDARD XPATH document() FUNCTION TO HANDLE A
TRANSLATION INVOLVING MORE THAN ONE DOCUMENT.  THE SECONDARY DOCUMENT
XPATH INFOSET IS FROM FILE: "keytable.xml".

</xsl:text>
   <xsl:for-each select="document('keytable.xml')/TBL:table">
      <xsl:for-each select="TBL:row">
<xsl:text>
</xsl:text>
         <xsl:value-of select="concat('TABLE=', @table)"/>
         <xsl:value-of select="concat(' ROW=', @row)"/>
         <xsl:value-of select="concat(' KEY=', @key)"/>
         <xsl:value-of select="concat(' VALUE=', @value)"/>
      </xsl:for-each>
   </xsl:for-each>
 </xsl:template>
</xsl:stylesheet>
