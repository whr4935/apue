#!/usr/bin/env python
# coding=utf-8

def foo(familyname, givename, lastname):
    print('my familyname is ' +str(familyname))
    print('my givename is ' + str(givename))
    print('my lastname is ' + str(lastname))



if __name__ == '__main__':
    username = ('wang', 'hao')
    #  foo(* username)

    username2 = {'givename':'hao', 'familyname':'wang', 'lastname':'lee'}
    foo(**username2)
