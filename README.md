Fn-Lock switcher for ThinkPad Compact Bluetooth Keyboard with TrackPoint
========================================================================

The Lenovo Thinkpad compact bluetooth keyboard is a repackaging of a Thinkpad
laptop keyboard into a bluetooth keyboard case. Unfortunately Lenovo had
already started their bizzare process of throwing away keys at this point, and
the Fn-keys have been replaced with some random functions Lenovo thought more
useful. The Fn-Lock switching is handled by the windows drivers, and
unfortunately is off by default.

There is also a physically-identical USB version of this keyboard. I'm guessing
that a lot of this would apply here too, but I do not have one to test.

Paring
------

Connecting to the keyboard is somewhat quirky, because it's bluez. Firstly you
can connect without pairing:

    bluez-test-device create 90:7F:61:01:02:03
    bluez-test-input connect 90:7F:61:01:02:03

However then the keyboard will not reconnect when it wakes up. To do this, you
need to pair properly. Naively pairing results in "hci0: Cancel Pair Device
(0x001a) failed: Invalid Parameters (0x0d)". What's going in is the keyboard is
the one that is specifying the PIN, however bluez doesn't tell you that.

In one window, do:

    hcidump | grep 'User Passkey Notification' -A1

Then in another, do:

    bluez-simple-agent hci0 90:7F:61:01:02:03

You should see the passkey to type in appear in the hcidump window, type it
into the keyboard and press enter. You should have paired. Finally:

    bluez-test-device trusted 90:7F:61:01:02:03 1
    bluez-test-input connect 90:7F:61:01:02:03

Kernel module
-------------

I have written a HID driver to cope with the keyboard's quirks. It enables
fn-lock by default and allows you to toggle via. sysfs.

For more information, see the module directory.

tp-compact-keyboard
-------------------

If you don't want to install the kernel module, you can use the
tp-compact-keyboard script to at least enable fn-lock.

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
