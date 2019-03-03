#!/usr/bin/env python3
#coding=utf-8

from initdata import bob, sue
import shelve


db = shelve.open('people-shelve')
db['bob'] = bob
db['sue'] = sue
db.close()

