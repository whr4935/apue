TARGET:=foo
OBJS:=foo.o bar.o
DEP:=$(OBJS:.o=.dep)

build_target:$(TARGET)

$(TARGET):$(OBJS)
	gcc $^ -o $@

-include $(DEP)

%.o:%.c
	gcc -g -c $< -o $@ -MMD -MF $(@:.o=.dep)

clean:
	-rm -rf $(TARGET) $(OBJS) $(DEP)
