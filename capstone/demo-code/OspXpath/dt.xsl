<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.00"
 xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
 xmlns:lx="http://xml.osp.state.or.us/cjis/lems">
 <xsl:output method="text" omit-xml-declaration="yes"/>
 <xsl:template match="TESTXML">
  <xsl:for-each select="testdate">
    <xsl:text>testdate element = (</xsl:text>
    <xsl:value-of select="."/><xsl:text>)
Format MDY = </xsl:text>
    <xsl:value-of select="lx:cvdate(.,'MDY',10)"/><xsl:text>
Format MDX = </xsl:text>
    <xsl:value-of select="lx:cvdate(.,'MDX',10)"/><xsl:text>
Format FH8 = </xsl:text>
    <xsl:value-of select="lx:cvdate(.,'FH8',10)"/><xsl:text>
Format NH8 = </xsl:text>
    <xsl:value-of select="lx:cvdate(.,'NH8',10)"/><xsl:text>
Format ND6 = </xsl:text>
    <xsl:value-of select="lx:cvdate(.,'ND6',10)"/><xsl:text>
Format FD8 = </xsl:text>
    <xsl:value-of select="lx:cvdate(.,'FD8')"/><xsl:text>
Format FD8 -7yr =</xsl:text>
    <xsl:value-of select="lx:cvdate(.,'FD8',-7)"/><xsl:text>
Format Default = </xsl:text>
    <xsl:value-of select="lx:cvdate(.)"/><xsl:text>
</xsl:text>
  </xsl:for-each>
 </xsl:template>
 <xsl:template match="/">
  <xsl:text>DIAGNOSTIC TEST OF lx:cvdate() CONVERSIONS
</xsl:text>
  <xsl:apply-templates select="TESTXML"/>
<!--  <xsl:value-of select="lx:cvdate(TESTXML/testdate,'FD8',10)"/> -->
 </xsl:template>
</xsl:stylesheet>
