<?xml version="1.0"?> 
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:param name="param1" select="'no passing, default value'"/>
  <xsl:param name="param2" select="'no passing, default value'"/>
  <xsl:param name="param3" select="'no passing, default value'"/>
  <xsl:param name="param4" select="'no passing, default value'"/>
  <xsl:param name="param5" select="'no passing, default value'"/>
  <xsl:template match="doc">
    <out><xsl:value-of select="$param1"/></out>
    <out><xsl:value-of select="$param2"/></out>
    <out><xsl:value-of select="$param3"/></out>
    <out><xsl:value-of select="$param4"/></out>
    <out><xsl:value-of select="$param5"/></out>
  </xsl:template>
</xsl:stylesheet>
