TARGET:=test_hook

build_target:$(TARGET)

test_hook:test_hook.o libhook.so
	gcc -o $@ $< libhook.so  -Wl,-rpath="."

libhook.so:hook_getpid.o
	gcc -o $@ -shared $^

%.o:%.c
	gcc -o $@ -fPIC -c $<

clean:
	-rm -rf test_hook libhook.so *.o
