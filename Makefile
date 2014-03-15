obj-m:=hello.o
kernel_path=/lib/modules/$(shell uname -r)/build
pwd:=$(shell pwd)

all:
	make -C $(kernel_path) M=$(pwd) modules
clean:
	rm *.o *.ko *.mod.c Module.symvers modules.order -f
