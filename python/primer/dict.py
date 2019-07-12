#!/usr/bin/env python
# coding=utf-8

bob1 = dict(name='bob', job='dev', age=40)
bob2 = dict(zip(['name', 'job', 'age'], ['bob', 'dev', 40]))

print(bob1)
print(bob2)
