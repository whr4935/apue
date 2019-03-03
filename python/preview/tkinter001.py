#!/usr/bin/env python
# coding=utf-8

from Tkinter import *
from tkMessageBox import showinfo

def reply():
    showinfo(title='popup', message='Buttong Pressed!')

window = Tk()
button = Button(window, text = 'press', command = reply)
button.pack()


window.mainloop()
