
MODULE_NAME = scull

ifeq ($(KERNELRELEASE),)

include ../Make.param

else

obj-m := $(MODULE_NAME).o

$(MODULE_NAME)-objs := main.o pipe.o

#access.o pipe.o

endif

