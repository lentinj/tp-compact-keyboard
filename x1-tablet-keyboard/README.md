# X1 Tablet Thin Keyboard (gen1)

A USB keyboard with a connector built-in to the hinge.

![keyboard](x1-tablet-keyboard/keyboard.jpg)

## Adaptor for use as a normal USB keyboard

The PCB can attach over the tablet's mounting posts, connecting with the pogo pins, breaking these out ready for a USB cable to be attached.

The gerbers can be found in [x1-tablet-keyboard/Gerber_tp-x1t-gen1-adapter.zip], which can be uploaded to JLPCB to manufacture more.

Then, solder a USB cable to the pins. The colour scheme *should* (but may not) be:

* GND: Black
* DN: Green
* DP: White
* 5v: Red

This is my first attempt at designing a PCB, and it probably shows. The holes are also slightly too tight in the boards I got back, and need filing for a proper fit.
It could also be extened by including a USB socket on the PCB.

## Other adaptors

This isn't the first attempt at attaching a regular USB cable to a X1 tablet, however 

* https://www.reddit.com/r/thinkpad/comments/10klmxv/pinout_of_thinkpad_x1_tablet_gen1_keyboard/?rdt=51762
* https://www.reddit.com/r/thinkpad/comments/10gneh1/diy_a_usb_thinkpad_keyboard_with_backlit_and/
* https://spawn.link/posts/2019-02-18_-_lenovo_x1_tablet_keyboard/
