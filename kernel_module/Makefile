obj-m += kmain.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

redo:
	sudo rmmod kmain
	make
	sudo insmod kmain.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
