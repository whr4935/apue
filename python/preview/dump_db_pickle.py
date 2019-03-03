#!/usr/bin/env python
# coding=utf-8

import pickle
dbfile = open('people-pickle', 'rb')
db = pickle.load(dbfile)
for key in db:
    print (key, '==>\n    ', db[key])

age = 'age'
l = [(db[rec][age] **2 if db[rec][age] >= 45 else db[rec][age]) for rec in db]
print (l)

dbfile.close()
