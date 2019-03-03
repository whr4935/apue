#!/usr/bin/env python
# coding=utf-8

from person_start import Person

class Manager(Person):
    def __init__(self, name, age, pay):
        Person.__init__(self, name, age, pay, "manager")

    def giveRaise(self, percent, bonus = 0.1):
        self.pay *= (1.0+percent + bonus)


if __name__ == '__main__':
    tom = Manager('Tom Doe', 50, 50000)
    #  print tom.lastName()
    tom.giveRaise(.20)
    #  print tom.pay


    tom.myprint()

    print( tom)
