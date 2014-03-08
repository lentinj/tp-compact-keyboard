Kernel module to enable extra hotkeys
=====================================

Ensure kernel headers are installed:

# apt-get install kernel-headers-$(uname -r)

Then build:

$ make

For debugging:

# insmod hid-lenovo-tpcompactkbd.ko
(connect the keyboard, should see a connecting device like 0005:17EF:6048.004B)
# echo -n "0005:17EF:6048.004B" > /sys/bus/hid/drivers/hid-generic/unbind
# echo -n "0005:17EF:6048.004B" > /sys/bus/hid/drivers/lenovo_tpcompactkbd/bind


Notes on creating
-----------------

Since this isn't a USB device, we can't as easily see the Vendor ID / Product
ID, which we need to find our keyboard. To do this, use the ``hid-example``
tool included with the linux kernel.



http://unix.stackexchange.com/questions/12005/how-to-use-linux-kernel-driver-bind-unbind-interface-for-usb-hid-devices
