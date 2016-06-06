Fn-Lock switcher for ThinkPad Compact Keyboards with TrackPoint
===============================================================

The Lenovo Thinkpad compact keyboard is a repackaging of a Thinkpad
laptop keyboard into a bluetooth keyboard case. Unfortunately Lenovo had
already started their bizzare process of throwing away keys at this point, and
the Fn-keys have been replaced with some random functions Lenovo thought more
useful. The Fn-Lock switching is handled by the windows drivers, and
unfortunately is off by default on the bluetooth keyboard.

There is also a physically-identical USB version of this keyboard. Fortunately
this leaves Fn-Lock on by default, so isn't so annoying under Linux.

Paring bluetooth keyboard
-------------------------

Connecting to the keyboard is somewhat quirky. You can connect without pairing,
and this generally works, e.g. by using ``bluez-test-input connect``. However
if you do this then the keyboard will not attempt to automatically when you
next use it. For this to happen, you need to exchange a PIN.

If the host supports it then the keyboard will send a PIN code to the host for
you to enter, rather than you entering a PIN code on both. Some versions of
bluez tell the keyboard it supports this approach, however the userland
doesn't. This means that you won't be told what PIN code to enter.

To get around this, run the following in a separate console before you start
pairing:

    sudo hcidump -at | grep -i passkey -A1

During the pairing process, you are looking out for lines like:

    > HCI Event: User Passkey Notification (0x3b) plen 10
    bdaddr 90:7F:61:01:02:03 passkey 123456

In this case, ``123456`` is the PIN you need to enter.

Your next step depends on what userland tools you use.

### bluez-test-*

Put the keyboard into discoverable mode by holding down the power button until
the light starts flashing. Then use ``hcitool scan`` to find out what the
address of your keyboard is. It should resemble ``90:7F:61:01:02:03``.

Firstly, create the device:

    bluez-test-device create 90:7F:61:01:02:03

Then start trying to pair:

    bluez-simple-agent hci0 90:7F:61:01:02:03

You should see the passkey to type in appear in the hcidump window, type it at
the prompt (using a keyboard already connected to your computer) and press
enter. You should have paired. Finally:

    bluez-test-device trusted 90:7F:61:01:02:03 1
    bluez-test-input connect 90:7F:61:01:02:03

### bluetooth-wizard

Select the device (keyboard) and click "Continue", watch for the PIN code in
the hcidump window, and enter that.

See: https://bugzilla.redhat.com/show_bug.cgi?id=1019287#c3

### bluetoothctl

See: https://wiki.archlinux.org/index.php/Bluetooth_keyboard#Pairing_process

Using keyboards, Linux 3.17+
----------------------------

Linux kernels 3.17 and above (or any kernel with the [kernel-patch](https://github.com/lentinj/tp-compact-keyboard/tree/master/kernel-patch)
applied) have built in support for the keyboards. This means all keys should
work out the box, and you can control whether fn_lock is enabled by:

    echo 0 > /sys/bus/hid/devices/*17EF\:604*/fn_lock 

The kernel can't currently switch fn_lock automatically for you, however you
can use [esekeyd](https://sites.google.com/site/blabdupp/esekeyd) to map
KEY_FN_ESC to a script such as:

    { grep -q 1 /sys/bus/hid/devices/*17EF\:604*/fn_lock && echo 0 || echo 1; } \
        > /sys/bus/hid/devices/*17EF\:604*/fn_lock

...to toggle it.

Using keyboards, Linux pre 3.17
-------------------------------

To use the keyboard in an older kernel, you have one of serveral options:-

1. Apply the patches in [kernel-patch](https://github.com/lentinj/tp-compact-keyboard/tree/master/kernel-patch) to your distribution's kernel
2. Use an external module, e.g. https://github.com/mithro/tp-compact-keyboard-backport
3. Use the userspace tool, ``tp-compact-keyboard``

The latter is presented here. ``tp-compact-keyboard`` is a small utility
to control some features of the keyboard, most
importantly to enable fn-lock. It only works with the bluetooth keyboard,
whilst the USB keyboard accepts the same commands but not in the same way,
one has to tweak the hidraw ioctls which (AFAIK) can't be done in a Bash script.

### Requirements

This was developed under a Debian unstable kernel, 3.12-1-amd64, and has been
reported to work for at least 3.11. However older kernels (3.6, for example)
don't send the report. Not sure why currently.

### Using

To enable/disable Fn-Lock, do "./tp-compact-keyboard --fn-lock-disable" or
"./tp-compact-keyboard --fn-lock-enable". This will find any bluetooth
keyboards connected and enable/disable. It has a few other functions too, but
they're not very useful on their own. Have a look at the source.

I'm assuming what you really want to do is force Fn-Lock on and forget about it
though. To do this, do the following as root:

    mkdir /etc/udev/scripts
    cp tp-compact-keyboard /etc/udev/scripts/tp-compact-keyboard
    cp tp-compact-keyboard.rules /etc/udev/rules.d/tp-compact-keyboard.rules

Then udev will enable Fn-lock every time the keyboard is connected.

### Other options

There are a few other options, however they are mostly useless without a custom kernel
module handling the keyboard:

    --sensitivity xx
        Set sensitivity of trackpoint. xx is hex value 01--09 (although values
        up to FF work).
    --native-fn-enable
    	By default, F7/F9/F11 generate a string of keypresses that might be
    	useful under windows. This stops this, and instead only generates
    	custom events.
    --native-mouse-enable
        The middle button generates 2 events by default, but Linux only
        understands one of them. This disables the event that Linux does
        understand, leaving you with no middle button.
    --native-mouse-disable
        Restore middle button to a working state.

Pair/unpair isn't enough to reset the keyboard, you need to power down the
keyboard to get it back to it's original state.

How I did it
------------

I worked out the custom commands by sniffing what the windows driver did.
Firstly I set up an XP vm in KVM/QEMU and installed the drivers. Then I
disconnected the keyboard from my computer, and ensured that my user running
QEMU could write to the ``/dev/bus/usb`` node associated with my bluetooth
dongle.

Wireshark was readied for capture::

    # modprobe usbmon
    # wireshark

Then, I let windows use the bluetooth dongle::

    (qemu) usb_add host:0a5c:217f

If you capture the whole association process then you should be able to view
HID packets to/from the keyboard (filter for ``bthid``). The particuarly
interesting ones here are the ones going to the keyboard.

Disassembling the keyboard
--------------------------

The topmost part of the keyboard is just clipped on, run something around the
seam on the underside of the keyboard to release it. The laptop keyboard part
is stuck to the underside with double-sided tape, fierce enough that keyboard
removal will probably bend it irreprably.

Repair of trackpoint
--------------------

Some users of the bluetooth keyboard have found that the trackpoint stops
working, whist the rest of the keyboard is fine. This isn't a software issue,
apparently the chip that controls the trackpoint isn't correctly soldered down.

See: http://bdm.cc/2016/06/lenovo-bluetooth-keyboard-repairs/
