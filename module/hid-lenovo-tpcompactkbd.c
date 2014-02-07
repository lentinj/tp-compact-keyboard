/*
 *  ThinkPad Compact (Bluetooth|USB) Keyboard with TrackPoint
 *
 *  Copyright (c) 2014 Jamie Lentin <jm@lentin.co.uk>
 *
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */

#include <linux/device.h>
#include <linux/hid.h>
#include <linux/module.h>

#define BT_VENDOR_ID_LENOVO	0
#define BT_DEVICE_ID_CBTKBD	0

/*
 * Keyboard sends non-standard reports for most "hotkey" Fn functions.
 * Map these back to regular keys.
 *
 * Esc:	KEY_FN_ESC		FnLock
 * (F1--F3 are regular keys)
 * F4:	KEY_MICMUTE		Mic Mute
 * F5:	KEY_BRIGHTNESSDOWN	Brightness down
 * F6:	KEY_BRIGHTNESSUP	Brightness up
 * F7:	KEY_SWITCHVIDEOMODE	External display (projector)
 * F8:	KEY_WWAN		Wireless
 * F9:	KEY_CONFIG		Control panel / settings
 * F10:	KEY_SEARCH		Search
 * F11:	KEY_FN_F11		View open applications (3 boxes)
 * F12:	KEY_FN_F12		Open My computer (6 boxes)
 */

#define tpckbd_map_key_clear(c)	hid_map_usage_clear(hi, usage, bit, max, \
					EV_KEY, (c))
static int tpcompactkbd_input_mapping(struct hid_device *hdev,
		struct hid_input *hi, struct hid_field *field,
		struct hid_usage *usage, unsigned long **bit, int *max)
{
	return 0;
}

static const struct hid_device_id tpcompactkbd_devices[] = {
	{ HID_BLUETOOTH_DEVICE(BT_VENDOR_ID_LENOVO, BT_DEVICE_ID_CBTKBD) },

	{ }
};
MODULE_DEVICE_TABLE(hid, tpcompactkbd_devices);

static struct hid_driver tpcompactkbd_driver = {
	.name = "lenovo_tpcompactkbd",
	.id_table = tpcompactkbd_devices,
	.input_mapping = tpcompactkbd_input_mapping,
};
module_hid_driver(tpcompactkbd_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jamie Lentin <jm@lentin.co.uk>");
MODULE_DESCRIPTION("ThinkPad Compact Keyboard with TrackPoint input driver");
