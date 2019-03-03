#!/usr/bin/env python
# coding=utf-8

import shelve
db = shelve.open('people-shelve')
for key in db:
    print (key , '\n    ', db[key])

bob = db['bob']
bob['job'] = 'software'
db['bob'] = bob

print (db['bob'])

db.close()


db = shelve.open('people-shelve')
print (db['bob'])
db.close()
