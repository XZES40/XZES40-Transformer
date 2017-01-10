# Document

Document is a class with three main components and methods that operate on those components.

Documents are stored and retrieved from the Cache and operated on by the Transformer.
While we could us the Xerces and Xalan object natively, this wrapper allows for a seperation of concerns.
The Cache can worry about storing an arbitrary `class`, the Transformer worries about transforming the `Document.contents`, and the each component can be developed (mostly) seperate.

## Members

Here is the rough outline of the variables in a Document:

- `uri`: The file path or remote address that the contents of a Document are stored.
- `id`: Is a **unique** ID, probably created by hashing some safe metadata about the source file.
- `contents`: Are the **parsed** XML or XSLT Document.

The raw contents of a file are not stored, only the parsed version.
This may change going forward.

## Methods

The Document class has a few methods:

- `Document()` is the initializer.
- `set/get_uri()` set and get the URI respectively.
- `set/get_id()` set and get the ID respectively.
- `set/get_contents()` set and get the contents respectively.
- `to_str()` processes the `contents` member and outputs and XML/XSLT document (useful for `output` documents).
- `to_file()` processes the `contents` member and outputs the XML/XSLT document to a file (useful for `output` documents).

## Workflow

The Document class is actually the part of the project that interfaces with the Cache the most.
Here is why:

1. For a Document to be created, it must be passed a URI (file path or HTTP address).
2. The URI is parsed and basd on that an ID is created by either hashing the file contents or hashing metadata about the remote URI.
3. The construcor then checks to see if the Document ID is a key in the Cache.
    - If the Document is *not* in the Cache it is parsed and stored in the Cache.
    - If the Document *is* in the Cache the value of `contents` is filled with the Cached value.
4. Once the Document is initialized it is returned (probably to the Transformer).

The Document component was broken out into it's own file as it pertained to both Caching and Transformation and seemed complicated enough not to need it's own source file.
