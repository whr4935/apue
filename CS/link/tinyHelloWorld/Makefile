build_target:hello

.PHONY:hello
hello:
	gcc -m32 -fno-asynchronous-unwind-tables -c -fno-builtin $@.c 
	#ld -m elf_i386 -static -e nomain -o $@ $@.o
	ld -m elf_i386 -T hello.lds -o $@ $@.o

clean:
	-rm -rf hello hello.o
