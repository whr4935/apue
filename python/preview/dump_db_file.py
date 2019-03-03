#!/usr/bin/env python
# coding=utf-8


if __name__ == '__main__':
    from make_db_file import loadDbase
    db =loadDbase()
    for key in db:
        print(key, '=>\n  ', db[key])

