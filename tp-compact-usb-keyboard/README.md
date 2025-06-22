# ThinkPad Compact USB Keyboard with TrackPoint

[Images of USB keyboard internals](tp-compact-usb-keyboard/)

* **HID Name:** Lenovo ThinkPad Compact Keyboard with TrackPoint
* **Model:** KU-1255
* **Fru P/N:** 03X8746
* **Vendor ID:** 0x17ef
* **Product ID:** 0x6047

The Lenovo Thinkpad Compact Keyboard with TrackPoint is a repackaging of a
Thinkpad laptop keyboard into a stand-alone case with a cabled USB connection
to the computer. Following the current fashion, Lenovo has made the top row of
keys serve two purposes, using them for both the traditional Fn keys and for
"hotkeys" intended to control various functions such as volume and screen
brightness.

Pressing FnLk (Fn+Esc) switches these keys between generating Fn
keypresses and hotkey keypresses, but unlike laptop keyboards this mode change
is handled not by the keyboard itself but by Lenovo's keyboard driver for
Windows.

## Toggling FnLk

Same as [the bluetooth keyboard](../tp-compact-bt-keyboard/#toggling-fnlk)

## Replacement trackpoint caps

Same as [the bluetooth keyboard](../tp-compact-bt-keyboard/#replacement-trackpoint-caps)

## Disassembling the Keyboard

Same as [the bluetooth keyboard](../tp-compact-bt-keyboard/#disassembling-the-keyboard)

## Repairing the Trackpoint

Same as [the bluetooth keyboard](../tp-compact-bt-keyboard/#repairing-the-trackpoint)

## Old userland utility

The linux kernel can now control this directly, the below is of historical interest only.

The C utility in this directory demonstrates the commands understood by the
keyboard. It supports:

* [Thinkpad Compact USB keyboard with Trackpoint](https://www.google.com/search?tbm=isch&q=KU-1255) (KU-1255)
* [Lenovo Low Profile USB Keyboard](https://www.google.com/search?tbm=isch&q=LXH-JME2209U) (LXH-JME2209U)

Anything this tool can do is already handled by linux now, but the could be
used to add support to another operating system, for example.

To build, run ``make``.

### udev rule

To run ``tp-compact-usb-keyboard`` whenever a keyboard is inserted, you can use
udev rule similar to the following:

```
SUBSYSTEM=="hidraw", ATTRS{idVendor}=="17ef", ATTRS{idProduct}=="6018", RUN+="/path/to/tp-compact-usb-keyboard %E{DEVNAME}"
```
