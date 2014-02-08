Kernel module to enable extra hotkeys
=====================================

Ensure kernel headers are installed:

# apt-get install kernel-headers-$(uname -r)

Then build:

$ make


Notes on creating
-----------------

Since this isn't a USB device, we can't as easily see the Vendor ID / Product
ID, which we need to find our keyboard. To do this, use the ``hid-example``
tool included with the linux kernel.
