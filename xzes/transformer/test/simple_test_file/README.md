# How to use this test?

## In the command-line

To auto-run the tests run the following commandin this directory:

```
./simple_test.sh
```

This can be run in the virtual machine, or on your local system.
It depends on the application being avaliable on `192.168.33.22:8000`.

## In the browser

Once the application is up and running using the Vagrant setup script, go to the address `192.168.33.22:8000`.
For each pair of files in this directory (`sample1.xml`+`sample1.xsl`, etc), upload the pair, click 'Transform Files' and when the button becomes available click 'View Raw XML'.
Fore each sample you can also add parameters.
The accepted parameter keys are `param1`, `param2`, etc. up to `param5`.
The key must be one of those but the value may be anything you want.

`sample1` does not use parameters, but all of them can accept parameters.

Click the 'View Raw XML' button to refresh the document preview.

Click 'Download Output' to download a copy of the transformed file.

## How do I know it works?

In the CLI you should see 200 OK response output form `curl`, and transformed XML documents.

For the web-browser testing you should see the output of the transformation in the preview.

If you upload a *non* XML/Style Sheet you should see an error message.
