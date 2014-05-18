obj-m += hid-lenovo-tpcompactkbd.o
INTERFACE := bt
ifeq ($(INTERFACE),usb)
  HID_DEVICE := $(shell basename -a /sys/bus/hid/drivers/*/0003\:17EF\:6047* | tail -1)
else
  HID_DEVICE := $(shell basename /sys/bus/hid/drivers/*/0005\:17EF\:6048*)
endif

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load: hid-lenovo-tpcompactkbd.ko
	echo > /dev/kmsg
	date > /dev/kmsg
	rmmod hid-lenovo-tpcompactkbd || true
	insmod hid-lenovo-tpcompactkbd.ko
	for f in /sys/bus/hid/drivers/*; do [ -e "$$f/$(HID_DEVICE)" ] && echo -n "$(HID_DEVICE)" > $$f/unbind || true; done
	echo "Binding to $(HID_DEVICE)"
	echo -n "$(HID_DEVICE)" > /sys/bus/hid/drivers/lenovo_tpcompactkbd/bind
	dmesg | tail

install: hid-lenovo-tpcompactkbd.ko
	cp hid-lenovo-tpcompactkbd.ko /lib/modules/`uname -r`/
	depmod -a

.PHONY: load install
