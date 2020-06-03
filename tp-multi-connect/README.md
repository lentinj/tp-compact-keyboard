Thinkpad Multi-Connect Keyboard
===============================

There is also another physically-identical keyboard, the Thinkpad
*Multi-Connect* Keyboard, which allows you to toggle between 3 paired devices.


* **HID Name:** ThinkPad Multi Connect Bluetooth Keyboard with Trackpoint
* **Model:** KT-1525
* **P/N:** 4X30K12182
* **Fru P/N:** 03X7111
* **Vendor ID:** 0x17ef
* **Product ID:** 0x6083
* **MFG:** CEM 2019-04-02

It appears to be China-only, and only available through sites like AliExpress.
There are no drivers for any OS (iOS/Android or Windows), just a single-sheet
manual is available here:
http://webdoc.lenovo.com.cn/lenovowsi/new_cskb/att/147372/%E5%BF%AB%E9%80%9F%E4%BD%BF%E7%94%A8%E6%8C%87%E5%8D%97.pdf

Unlike the Compact keyboards, the keyboard does not have (known) built-in
Fn-Lock functions, Fn, Fn-F1 and F1 are all separate scancodes, so they can be
remapped using a ``hwdb`` database.

Keyboard-supported key combinations:

* **Fn+Up/Down:** Trackpoint sensitivity
* **Fn+Delete:** Change paired device type (Windows/Android/iOS). Changes behaviour of some keys, e.g. Super.

There is no known way of performing Fn-Lock (even under windows), or changing middle-button behaviour. See: https://github.com/lentinj/tp-compact-keyboard/issues/48
