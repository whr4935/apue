TARGET = tinyHelloworld
OBJS := tinyHelloworld.o

CC=gcc
LD=ld.gold

$(TARGET):$(OBJS)
	$(LD) -static -T tinyHelloworld.lds -o $@ $^

%.o:%.c
	$(CC) -m32 -c -o $@ $< -MD -MF $(@:.o=.dep)

clean:
	-rm -rf $(TARGET) *.o *.dep
