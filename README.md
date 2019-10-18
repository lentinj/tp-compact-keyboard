FnLk switcher for ThinkPad Compact Keyboards with TrackPoint
============================================================

The Lenovo Thinkpad Compact Keyboard with TrackPoint is a repackaging
of a Thinkpad laptop keyboard into a stand-alone case with a Bluetooth
wireless or cabled USB connection to the computer. Following the
current fashion, Lenovo has made the top row of keys serve two
purposes, using them for both the traditional Fn keys and for
"hotkeys" intended to control various functions such as volume and
screen brightness.

Pressing FnLk (Fn+Esc) switches these keys between generating Fn
keypresses and hotkey keypresses, but unlike laptop keyboards this mode change
is handled not by the keyboard itself but by Lenovo's keyboard driver for
Windows.

This repository contains some utilities that can configure the keyboards,
although largely of historical interest now support is baked into Linux.

Toggling FnLk
-------------

### Linux

Linux kernels 3.17 and above have support for toggling Fn-Lock in the
[hid-lenovo](https://elixir.bootlin.com/linux/latest/source/drivers/hid/hid-lenovo.c)
module. This means all keys should work out the box, and you can control
whether fn_lock is enabled by:

    echo 0 > /sys/bus/hid/devices/*17EF\:604*/fn_lock 

The kernel can't currently switch fn_lock automatically for you, however you
can use [esekeyd](https://sites.google.com/site/blabdupp/esekeyd) to map
KEY_FN_ESC to a script such as:

    { grep -q 1 /sys/bus/hid/devices/*17EF\:604*/fn_lock && echo 0 || echo 1; } \
        > /sys/bus/hid/devices/*17EF\:604*/fn_lock

...to toggle it.

### Apple OS X (MacOS)

[tpkb](https://github.com/unknownzerx/tpkb) is a Mac userland tool
similar to ``tp-compact-keyboard``. It uses the cross-platform hidapi
library.

### Linux Pre-3.17

Use either the [tp-compact-bt-keyboard](tp-compact-bt-keyboard/) or the
[tp-compact-usb-keyboard](tp-compact-usb-keyboard/) utilities in this
repository, or apply the [kernel-patches](kernel-patch/) to your kernel.

Disable FnLk by default
-----------------------

Linux assumes you want Fn keys to act as Fn keys and Enables Fn-Lock, however
uyou may want the opposite. If so create a udev rule with the following command:

    cat <<'EOF' > /etc/udev/rules.d/99-thinkpad-compact-keyboard.rules
    SUBSYSTEM=="hid", ATTRS{idVendor}=="17ef", ATTRS{idProduct}=="604*", \
        TEST == "/sys/$devpath/fn_lock", \
        RUN += "/bin/sh -c 'echo 0 > \"/sys/$devpath/fn_lock\"'"
    EOF

How I Did It
------------

I worked out the custom commands by sniffing what the Windows driver did.
First I set up a Windows XP virtual machine in KVM/QEMU and installed the drivers. Then I
disconnected the keyboard from my computer and ensured that the user running
QEMU could write to the ``/dev/bus/usb`` node associated with my Bluetooth
dongle.

Wireshark was readied for capture:

    # modprobe usbmon
    # wireshark

Then, I let Windows use the Bluetooth dongle:

    (qemu) usb_add host:0a5c:217f

If you capture the whole association process then you should be able to view
HID packets to/from the keyboard (filter for ``bthid``). The particuarly
interesting ones here are the ones going to the keyboard.

Disassembling the Keyboard
--------------------------

The upper part of the keyboard case is just clipped on; run something
around the seam on the underside of the keyboard to release it. The
keyboard itself is fastened to the lower part of the case with
double-sided tape. Very gentle persuasion (probably some heat too)
removes it.

The keyboard itself looks like it comes out of a Thinkpad, with metal plates stuck
to it to add heft. The keyboard connects to a controller board in the top right.

[Images of USB keyboard internals](tp-compact-usb-keyboard/)
[Images of BT keyboard internals](http://bdm.cc/2016/06/lenovo-bluetooth-keyboard-repairs/)

Repairing the Trackpoint
------------------------

Some users of the Bluetooth keyboard have found that the TrackPoint stops
working whilst the rest of the keyboard is fine. This isn't a software issue;
apparently the chip that controls the TrackPoint isn't correctly soldered down.

See: http://bdm.cc/2016/06/lenovo-bluetooth-keyboard-repairs/

Replacement trackpoint caps
---------------------------

The keyboards use the "ThinkPad Low Profile TrackPoint Caps" (FRU 0A33908 for a 10-pack) by default. The original-size will fit also, but stick out more. This is fine for the cat-tongue, but with the others your finger can catch the edge.

Other keyboards
---------------

Some notes on other keyboards, not currently supported:

* [Thinkpad Multi-Connect Keyboard](tp-multi-connect/)
* [Thinkpad Tablet 2 Keyboard](tp-tablet2/)
