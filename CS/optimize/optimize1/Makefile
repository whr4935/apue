
build_target: optimize1


optimize1:a.o
	gcc -pg -o $@ $^

%.o:%.c
	gcc -pg -c $< -o $@

clean:
	-rm -rf optimize1 a.o *.out

