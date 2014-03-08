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

#include <linux/kernel.h> /*TODO: For debugging */
#include <linux/device.h>
#include <linux/hid.h>
#include <linux/module.h>

#define USB_VENDOR_ID_LENOVO	0x17ef
#define USB_DEVICE_ID_CBTKBD	0x6048

/*
 * Keyboard sends non-standard reports for most "hotkey" Fn functions.
 * Map these back to regular keys.
 *
 * Esc:	KEY_FN_ESC		FnLock [TODO: Not mapped]
 * (F1--F3 are regular keys)
 * F4:	KEY_MICMUTE		Mic Mute
 * F5:	KEY_BRIGHTNESSDOWN	Brightness down
 * F6:	KEY_BRIGHTNESSUP	Brightness up
 * F7:	KEY_SWITCHVIDEOMODE	External display (projector) [TODO: Win-P, SwMode]
 * F8:	KEY_FN_F8		Wireless
 * F9:	KEY_CONFIG		Control panel / settings [TODO: Win-F21, Config]
 * F10:	KEY_SEARCH		Search
 * F11:	KEY_FN_F11		View open applications (3 boxes) [TODO: Ctrl-Alt-Tab, F11]
 * F12:	KEY_FN_F12		Open My computer (6 boxes)
 */

#define tpckbd_map_key_clear(c)	hid_map_usage_clear(hi, usage, bit, max, \
					EV_KEY, (c))
static int tpcompactkbd_input_mapping(struct hid_device *hdev,
		struct hid_input *hi, struct hid_field *field,
		struct hid_usage *usage, unsigned long **bit, int *max)
{
	if ((usage->hid & HID_USAGE_PAGE) == HID_UP_CONSUMER) {
		set_bit(EV_REP, hi->input->evbit);
		switch (usage->hid & HID_USAGE) {
		case 0x0070:	tpckbd_map_key_clear(KEY_BRIGHTNESSDOWN);	return 1;
		case 0x006f:	tpckbd_map_key_clear(KEY_BRIGHTNESSUP);	return 1;
		case 0x03f1:	tpckbd_map_key_clear(KEY_FN_F8);	return 1;
		case 0x0221:	tpckbd_map_key_clear(KEY_SEARCH);	return 1;
		case 0x03f2:	tpckbd_map_key_clear(KEY_FN_F12);	return 1;
		}
	}

	if ((usage->hid & HID_USAGE_PAGE) == HID_UP_MSVENDOR) {
		set_bit(EV_REP, hi->input->evbit);
		switch (usage->hid & HID_USAGE) {
		case 0x00f1:	tpckbd_map_key_clear(KEY_MICMUTE);	return 1;
		case 0x00f4:	tpckbd_map_key_clear(KEY_SWITCHVIDEOMODE);	return 1; /* Down */
		case 0x00f5:	tpckbd_map_key_clear(KEY_SWITCHVIDEOMODE);	return 1; /* Up */
		case 0x00f6:	tpckbd_map_key_clear(KEY_CONFIG);	return 1; /* Down */
		case 0x00f7:	tpckbd_map_key_clear(KEY_CONFIG);	return 1; /* Up */
		case 0x00f8:	tpckbd_map_key_clear(KEY_FN_F11);	return 1; /* Down */
		case 0x00f9:	tpckbd_map_key_clear(KEY_FN_F11);	return 1; /* Up */
		}
	}

	return 0;
}

static const struct hid_device_id tpcompactkbd_devices[] = {
	{ HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_LENOVO, USB_DEVICE_ID_CBTKBD) },

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
