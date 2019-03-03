build_target:integer

integer: integer.o
	gcc  $^ -o $@


%.o:%.c
	gcc -c -o $@ $<

clean:
	-rm -rf integer integer.o
