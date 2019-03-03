#!/usr/bin/env python
# coding=utf-8


#  print('%c' % input())

import sys

for module in sys.modules:
    print('(%s %s)' % (module, sys.modules[module]))
