# xzes frontend

This is the frontend web interface for xzes40.
It is served via an Apache httpd server.
The Apache conf can be found in this component's parent directory.

## Dependencies

The frontend depends on [FileSaver.js][filesaver.js](git commit 4db4a78) and [Blob.js][blob.js](git commit 079824b) , both licensed under the MIT license, for downloading transformed files.

To upgrade these dependencies:

1. Visit their GitHub pages
2. download `FileSaver.js` and `Blob.js` respectively.
3. replace the current copies of those files in this directory with the updated versions.

The frontend also depends on JQuery, which is resovled by a Content Delivery Network (CDN).
If you need to use a local copy of JQuery, please consult the JQuery documentation for this task.
It roughly looks like:

1. Downlaod JQuery 3 (latest, full, minified).
2. Place it in this directory.
3. Replace the JQuery `script` tag with a refernce to the downloaded copy.

[blob.js]: https://github.com/eligrey/Blob.js
[filesaver.js]: https://github.com/eligrey/FileSaver.js/
