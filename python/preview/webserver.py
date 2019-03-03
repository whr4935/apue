#!/usr/bin/env python
# coding=utf-8

import os, sys
from http.server import HTTPServer, CGIHTTPRequestHandler

webdir = '.'
port = 8000

os.chdir(webdir)
srvraddr = ("", port)
srvrobj = HTTPServer(srvraddr, CGIHTTPRequestHandler)
srvrobj.serve_forever()
