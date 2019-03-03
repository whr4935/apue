build_target:float

float: float.o
	gcc  $^ -o $@


%.o:%.c
	gcc -c -o $@ $<

clean:
	-rm -rf float float.o
