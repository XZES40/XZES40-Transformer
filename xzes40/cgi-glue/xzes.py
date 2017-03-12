#!/usr/bin/python
# file: xzes.py
# destination: /usr/local/apache/fcgi-bin/xzes.py

import fcgi

def app(environ, start_response):
     start_response('200 OK', [('Content-Type', 'text/html')])
     return('''<html>
     <head>
          <title>Hello World!</title>
     </head>
     <body>
          <h1>Hello world!</h1>
     </body>
</html>''')

fcgi.WSGIServer(app, bindAddress = '/tmp/fcgi.sock').run()
