Fn-Lock switcher for ThinkPad Compact Bluetooth Keyboard with TrackPoint
========================================================================

The Lenovo Thinkpad compact bluetooth keyboard is a repackaging of a Thinkpad
laptop keyboard into a bluetooth keyboard case. Unfortunately Lenovo had
already started their bizzare process of throwing away keys at this point, and
the Fn-keys have been replaced with some random functions Lenovo thought more
useful. The Fn-Lock switching is handled by the windows drivers, and
unfortunately is off by default.

What this script does is enable/disable Fn-Lock for your keyboard, so you can
use the Fn-keys as Fn-keys.

Requirements
------------

This was developed under a Debian unstable kernel, 3.12-1-amd64, and has been
reported to work for at least 3.11. However older kernels (3.6, for example)
don't send the report. Not sure why currently.

Using
-----

To enable/disable Fn-Lock, do "./tp-compact-keyboard --fn-lock-disable" or
"./tp-compact-keyboard --fn-lock-enable". This will find any bluetooth
keyboards connected and enable/disable. 

I'm assuming what you really want to do is force Fn-Lock on and forget about it
though. To do this, do the following as root:

    mkdir /etc/udev/scripts
    cp tp-compact-keyboard /etc/udev/scripts/tp-compact-keyboard
    cp tp-compact-keyboard.rules /etc/udev/rules.d/tp-compact-keyboard.rules

Then udev will enable Fn-lock every time the keyboard is connected.

What this doesn't do
--------------------

There is also a physically-identical USB version of this keyboard. I do not
have one, nor know if it suffers the same problems.

You've probably noticed that most of the default Fn-key functions do nothing.
These are reported by the keyboard, but not as regular keys. One could write a
kernel driver to fix this (i.e. map them back to regular keys), but I haven't
yet.
