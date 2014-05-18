hid-lenovo-tpcompactkbd kernel module
=====================================

A module to handle the quirks of both the USB and the bluetooth keyboards
transparently. This will:-

* Map all special functions to something useful
* Enable Fn-Lock, and allow Fn-Esc to toggle

Ideally at some point this will be included with the Linux kernel, but until it
is you'll have to build it yourself.

Ensure kernel headers are installed:

    # apt-get install linux-headers-$(uname -r)

Then build:

    $ make

To test the driver at this point, connect the keyboard and then rebind it to
the driver, depending on which keyboard you have:

    # make load INTERFACE=bt
    # make load INTERFACE=usb

You can install with:

    # make install

To bind automatically, it needs to be listed in hid_have_special_driver,
but that's not going to work so well with an out-of-tree driver. You can
probably bodge it with a udev rule or something.

Fn-lock mode
------------

There is one kernel parameter, fnmode, that controls the Fn-Lock behaviour.

    0: Default (on, Fn-Esc toggles)
    1: Permanently on
    2: Permanently off

This can be toggled at run-time with /sys/module/hid_lenovo_tpcompactkbd/parameters/fnmode
but you may have to press Fn-Esc for this to take effect.

Also, if you map Fn-Esc to something other than KEY_FN_ESC, toggling will not
work.

Notes on creating
-----------------

Since this isn't a USB device, we can't as easily see the Vendor ID / Product
ID, which we need to find our keyboard. To do this, use the ``hid-example``
tool included with the linux kernel.

Also, you can't use lsusb to print out a nicely formatted version of the HID
report discriptors. However you can get it in binary form via:

    cat /sys/class/hidraw/hidraw2/device/report_descriptor | hexdump -v -e '/1 "%02X "'

Then [this request parser](http://www.frank-zhao.com/usb_desc_req_parser), will
turn it into something useful. At least after a bit of obvious hacking.
