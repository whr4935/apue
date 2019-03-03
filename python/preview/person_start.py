#!/usr/bin/env python
# coding=utf-8

class Person:
    def __init__(self, name, age, pay = 0, job = None):
        self.name = name
        self.age = age
        self.pay = pay
        self.job = job

    def lastName(self):
        return self.name.split()[-1]

    def giveRaise(self, percent):
        self.pay *= (1.0+percent)
    
    def myprint(self):
        pass
        #  print 'name = %s' % self.name
        #  print 'age = %d' % self.age
        #  print 'pay = %d' % self.pay
        #  print 'job = %s' % self.job

    def __str__(self):
        return "<%s ==> %s>" %(self.__class__.__name__, self.name)


if __name__  == '__main__':
    bob = Person('Bob Smith', 42, 30000, 'software')
    sue = Person('sue Jones', 45, 40000, 'hardware')
    print('%s, %s' % bob.name, sue.pay)

    #  print bob.name.split()[-1]
    #  sue.pay *= 1.10
    #  print sue.pay

    #  print bob.lastName()
    sue.giveRaise(.10)
    print(sue.pay)

    sue.myprint()
