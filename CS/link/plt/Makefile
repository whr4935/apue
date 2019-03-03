build_target:plt

plt: main.o libaddvec.so
	gcc -o $@ $< -L. -laddvec -Wl,-rpath=.

%.o:%.c
	gcc -g -c -o $@ $<

libaddvec.so:addvec.o
	gcc -fPIC -shared -o $@ $^

clean:
	-rm -rf *.o plt libaddvec.so
