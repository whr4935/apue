import os

# print 'Process %s start...' % os.getpid()
# pid = os.fork()
# if pid == 0:
#     print 'I am child process %s and my parent is %s' % (os.getpid(), os.getppid())

from multiprocessing import Process
def run_proc(name):
    print "Run child process %s(%s)" % (name, os.getpid())

p = Process(target=run_proc, args=('test',))
print 'child process will start'
p.start()
p.join()
print 'child process end.'
