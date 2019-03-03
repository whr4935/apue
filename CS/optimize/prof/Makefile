
build_target:

prof:a.o
	gcc -pg -ftest-coverage -fprofile-arcs -o $@ $^ -lm

a.o:a.c
	gcc -c -pg -ftest-coverage -fprofile-arcs -g -o $@ $<

clean:
	-rm -rf prof a.o
