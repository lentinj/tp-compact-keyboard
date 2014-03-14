obj-m += hid-lenovo-tpcompactkbd.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install:: hid-lenovo-tpcompactkbd.ko
	cp hid-lenovo-tpcompactkbd.ko /lib/modules/`uname -r`/
	depmod -a
