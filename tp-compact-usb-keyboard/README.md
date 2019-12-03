ThinkPad Compact USB Keyboard with TrackPoint
=============================================

The C utility in this directory demonstrates the commands understood by the
keyboard. It supports:

* [Thinkpad Compact USB keyboard with Trackpoint](https://www.google.com/search?tbm=isch&q=KU-1255) (KU-1255)
* [Lenovo Low Profile USB Keyboard](https://www.google.com/search?tbm=isch&q=LXH-JME2209U) (LXH-JME2209U)

Anything this tool can do is already handled by linux now, but the could be
used to add support to another operating system, for example.

To build, run ``make``.

udev rule
---------

To run ``tp-compact-usb-keyboard`` whenever a keyboard is inserted, you can use
udev rule similar to the following:

```
SUBSYSTEM=="hidraw", ATTRS{idVendor}=="17ef", ATTRS{idProduct}=="6018", RUN+="/path/to/tp-compact-usb-keyboard %E{DEVNAME}"
```
