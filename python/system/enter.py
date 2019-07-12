#!/usr/bin/env python
# coding=utf-8


#  print('%c' % input())

import sys
import matplotlib.pyplot as plt

for module in sys.modules:
    print('(%s %s)' % (module, sys.modules[module]))
