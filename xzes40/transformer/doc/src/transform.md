# Transform

The `transform_documents` method wraps the use of the Transformer class.

## Transformer class

The Transformer's job is to perform the actual application's purpose: transforming an XML document with an XSLT document and writing that to disk or printing it to the screen.

### Members

The three main members of the Transformer are:

- `Document xml`
- `Document xsl`
- `Document out`

These are created with the Document class initializer outlined in the Document ... err... doc.

### Methods

When the Documents are created they contain three components themselves `id`, `uri`, and `contents`.
The `contents` is the part we care about.

Using the Xerces and Xalan libraries we perform a DOM transformation with `xml.contents` as the XML component of the transformation, `xsl.contents` as the XSLT component and `out.contents` as the output.

### Caching output documents

Once the `xml` and `xsl` documents are initialized the cache gets requested for a document with the id `xml.contents + xsl.contents`.
This is a unique identifier at which all output documents are stored.
If the output document already exists then we bypass the entire transformation process and return this cached output document.

Once saved the output document is saved to the cache by it's own `id` **and** by the `id` `xml.contents + xsl.contents`.

Only output documents have a double-length id so there should be no collisions.
