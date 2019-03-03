#!/usr/bin/env python
# coding=utf-8

dbfilename = 'people-file'
ENDDB = 'enddb.'
ENDREC = 'endrec.'
RECSTP = '==>'

def storeDbase(db, dbfilename = dbfilename):
    "formatted dump of database to flat file"
    dbfile = open(dbfilename, 'w')
    for key in db:
        print(key, file=dbfile)
        for (name, value) in db[key].items():
            print(name + RECSTP + repr(value), file=dbfile)
        print(ENDREC, file=dbfile)
    print(ENDDB, file=dbfile)
    dbfile.close()

def loadDbase(dbfilename = dbfilename):
    "parse data to reconstruct database"
    dbfile = open(dbfilename)
    import sys
    stdin_bak = sys.stdin
    sys.stdin = dbfile
    db = {}
    key = input()
    while key != ENDDB:
        rec = {}
        field = input()
        while field != ENDREC:
            name, value = field.split(RECSTP)
            rec[name] = eval(value)
            field = input()
        db[key] = rec
        key = input()
    sys.stdin = stdin_bak
    return db


store = False

if __name__ == '__main__':
    from initdata import db
    if store:
        storeDbase(db)
        exit(0)
    else:
        db2 = loadDbase()
        for key in db2:
            print(key , '=>\n', db2[key])