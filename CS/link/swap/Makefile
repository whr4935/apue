build_target:ab

ab: a.o b.o
	ld -melf_i386 $^ -e main -o $@


%.o:%.c
	gcc -m32 -fno-stack-protector -c -o $@ $<

clean:
	-rm -rf *.o ab
