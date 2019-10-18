ThinkPad Compact Bluetooth Keyboard with TrackPoint
===================================================

``tp-compact-keyboard`` is a small utility
to control some features of the keyboard, most
importantly to enable FnLk. It works only with the Bluetooth keyboard,
whilst the USB keyboard accepts the same commands but not in the same way,
one has to tweak the hidraw ioctls which (AFAIK) can't be done in a Bash script.

#### Requirements

This was developed under a Debian unstable kernel, 3.12-1-amd64, and has been
reported to work on kernels as early as 3.10 (CentOS 7). However older kernels (3.6, for example)
don't send the report; I'm not currently sure why.

#### Using

To enable or disable FnLk on all connected ThinkPad Bluetooth
keyboards, run ``./tp-compact-keyboard --fn-lock-enable`` or
``./tp-compact-keyboard --fn-lock-disable``. The program has a few
other functions as well, but they're not very useful on their own.
Have a look at the source.

#### Other options

There are a few other options, however they are mostly useless without a custom kernel
module handling the keyboard:

    --sensitivity xx
        Set sensitivity of TrackPoint. xx is hex value 01--09 (although values
        up to FF work).
    --native-fn-enable
    	By default, F7/F9/F11 generate a string of keypresses that might be
	useful under Windows. This stops this, and instead only generates
    	custom events.
    --native-mouse-enable
        The middle button generates 2 events by default, but Linux only
        understands one of them. This disables the event that Linux does
        understand, leaving you with no middle button.
    --native-mouse-disable
        Restore middle button to a working state.

Pair/unpair isn't enough to reset the keyboard, you need to power down the
keyboard to get it back to it's original state.

Pairing the Keyboard
--------------------

The Bluetooth keyboard uses [BT 2.1 Simple Secure Pairing](https://en.wikipedia.org/wiki/Bluetooth#Pairing_mechanisms) (SSP),
which should be supported by modern Linux distributions. Several different utilities can be used to pair the keyboard.

If your distribution does not support SSP, or if you have problems pairing with SSP, you can try disabling it with ``hciconfig hci0 sspmode 0``. [See this note on a Logitech keyboard](https://wiki.archlinux.org/index.php/Bluetooth#Logitech_keyboard_does_not_pair).

### bluetoothctl

Pairing can be done with the ``pair`` command. You should be prompted for the PIN code to type
as part of the process.

See: https://wiki.archlinux.org/index.php/Bluetooth_keyboard#Pairing_process

### bluetooth-wizard

Select the device (keyboard) and click "Continue". Watch for the PIN code in
the hcidump window and enter that.

See: https://bugzilla.redhat.com/show_bug.cgi?id=1019287#c3

### bluez-test-*

Versions of Bluez that ship with these scripts don't support SSP and thus don't report
the PIN code the keyboard requires you to use.

To get around this, run the following in a separate console before you start
pairing:

    sudo hcidump -at | grep -i passkey -A1

During the pairing process, you are looking out for lines like:

    > HCI Event: User Passkey Notification (0x3b) plen 10
    bdaddr 90:7F:61:01:02:03 passkey 123456

In this case, ``123456`` is the PIN you need to enter.

Put the keyboard into discoverable mode by holding down the power button until
the light starts flashing. Then use ``hcitool scan`` to find out what the
address of your keyboard is. It should resemble ``90:7F:61:01:02:03``.

Firstly, create the device:

    bluez-test-device create 90:7F:61:01:02:03

Then start trying to pair:

    bluez-simple-agent hci0 90:7F:61:01:02:03

You should see the passkey appear in the hcidump window. Type that passkey at
the prompt (using a keyboard already connected to your computer) and press
enter. You should now be paired. Finally:

    bluez-test-device trusted 90:7F:61:01:02:03 1
    bluez-test-input connect 90:7F:61:01:02:03
