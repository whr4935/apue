import threading, time

print threading.Lock()
cond = threading.Condition()
g_data = 5

def run_thread(n):
    global cond
    global g_data

    print 'begin run_thread'
    time.sleep(1)

    print 'set global data to 10'
    cond.acquire()
    g_data = 10
    cond.notify()
    cond.release()
    print 'thread exit'



t1 = threading.Thread(target=run_thread, args=(5,))
t1.start()

while True:
    cond.acquire()
    print 'test g_data'
    if g_data == 5:
        print 'wait...'
        cond.wait()
    else:
        break
    cond.release()
print "g_data == %d" % g_data


t1.join()
print 'main thread exit'
