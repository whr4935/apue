MY_TARGET = myAsmAdd
OBJS := asm.o main.o

CC=gcc -m32
AS=as --32
LD=ld -m elf_i386 --dynamic-linker /lib/ld-linux.so.2  -lc

build_target:$(MY_TARGET)

$(MY_TARGET):$(OBJS)
	$(LD) -o $@ $^ 

%.o:%.c
	$(CC) -m32 -c -o $@ $< -MD -MF $(@:.o=.dep)

%.o:%.s
	$(AS) -o $@ $<

clean:
	-rm -rf $(MY_TARGET) *.o *.dep
