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

#define USB_VENDOR_ID_LENOVO	0x17ef
#define USB_DEVICE_ID_LENOVO_CUSBKBD	0x6047
#define USB_DEVICE_ID_LENOVO_CBTKBD		0x6048
#devine HID_UP_LNVENDOR	0xffa00000

static unsigned int fnmode;
module_param(fnmode, uint, 0644);
MODULE_PARM_DESC(fnmode, "Fn lock mode ([0] = normal (Fn Lock toggles), 1 = Permanently on, 2 = Permanently off)");

struct tpcompactkbd_sc {
	unsigned int fn_lock;
};

/* Send a config command to the keyboard */
static int tpcompactkbd_send_cmd(struct hid_device *hdev,
			unsigned char byte2, unsigned char byte3)
{
	int ret;
	unsigned char buf[] = {0x18, byte2, byte3};
	unsigned char report_type = HID_OUTPUT_REPORT;

	/* The USB keyboard accepts commands via SET_FEATURE */
	if (hdev->product == USB_DEVICE_ID_LENOVO_CUSBKBD) {
		buf[0] = 0x13;
		report_type = HID_FEATURE_REPORT;
	}

	ret = hdev->hid_output_raw_report(hdev, buf, sizeof(buf), report_type);
	return ret < 0 ? ret : 0; /* BT returns 0, USB returns sizeof(buf) */
}

/* Toggle fnlock on or off, if fnmode allows */
static void tpcompactkbd_toggle_fnlock(struct hid_device *hdev)
{
	struct tpcompactkbd_sc *tpcsc = hid_get_drvdata(hdev);

	tpcsc->fn_lock = fnmode == 2 ? 0 : fnmode == 1 ? 1 : !tpcsc->fn_lock;
	if (tpcompactkbd_send_cmd(hdev, 0x05, tpcsc->fn_lock ? 0x01 : 0x00))
		hid_err(hdev, "Fn-lock toggle failed\n");
}

#define tpckbd_map_key_clear(c)	hid_map_usage_clear(hi, usage, bit, max, \
					EV_KEY, (c))
static int tpcompactkbd_usb_input_mapping(struct hid_device *hdev,
		struct hid_input *hi, struct hid_field *field,
		struct hid_usage *usage, unsigned long **bit, int *max)
{
	if ((usage->hid & HID_USAGE_PAGE) == HID_UP_LNVENDOR) {
		set_bit(EV_REP, hi->input->evbit);
		switch (usage->hid & HID_USAGE) {
		case 0xfa: /* Fn-Esc: Fn-lock toggle */
			tpckbd_map_key_clear(KEY_FN_ESC);
			return 1;
		case 0xf1: /* Fn-F4: Mic mute */
			tpckbd_map_key_clear(KEY_MICMUTE);
			return 1;
		case 0xf2: /* Fn-F5: Brightness down */
			tpckbd_map_key_clear(KEY_BRIGHTNESSDOWN);
			return 1;
		case 0xf3: /* Fn-F6: Brightness up */
			tpckbd_map_key_clear(KEY_BRIGHTNESSUP);
			return 1;
		case 0xf4: /* Fn-F7: External display (projector) */
			tpckbd_map_key_clear(KEY_SWITCHVIDEOMODE);
			return 1;
		case 0xf5: /* Fn-F8: Wireless */
			tpckbd_map_key_clear(KEY_WLAN);
			return 1;
		case 0xf6: /* Fn-F9: Control panel */
			tpckbd_map_key_clear(KEY_CONFIG);
			return 1;
		case 0xf8: /* Fn-F11: View open applications (3 boxes)*/
			tpckbd_map_key_clear(KEY_FN_F11);
			return 1;
		case 0xf9: /* Fn-F12: Open My computer (6 boxes)*/
			tpckbd_map_key_clear(KEY_FN_F12);
			return 1;
		}
	}

	return 0;
}
static int tpcompactkbd_bt_input_mapping(struct hid_device *hdev,
		struct hid_input *hi, struct hid_field *field,
		struct hid_usage *usage, unsigned long **bit, int *max)
{
	if ((usage->hid & HID_USAGE_PAGE) == HID_UP_CONSUMER) {
		set_bit(EV_REP, hi->input->evbit);
		switch (usage->hid & HID_USAGE) {
		case 0x03f1: /* Fn-F8: Wireless */
			tpckbd_map_key_clear(KEY_WLAN);
			return 1;
		case 0x0221: /* Fn-F10: Search */
			tpckbd_map_key_clear(KEY_SEARCH);
			return 1;
		case 0x03f2: /* Fn-F12: Open My computer (6 boxes) */
			tpckbd_map_key_clear(KEY_FN_F12);
			return 1;
		}
	}

	if ((usage->hid & HID_USAGE_PAGE) == HID_UP_MSVENDOR) {
		set_bit(EV_REP, hi->input->evbit);
		switch (usage->hid & HID_USAGE) {
		case 0x00f0: /* Fn-Esc: Fn-lock toggle */
			tpckbd_map_key_clear(KEY_FN_ESC);
			return 1;
		case 0x00f1: /* Fn-F4: Mic mute */
			tpckbd_map_key_clear(KEY_MICMUTE);
			return 1;
		case 0x00f2: /* Fn-F5: Brightness down */
			tpckbd_map_key_clear(KEY_BRIGHTNESSDOWN);
			return 1;
		case 0x00f3: /* Fn-F6: Brightness up */
			tpckbd_map_key_clear(KEY_BRIGHTNESSUP);
			return 1;
		case 0x00f4: /* Fn-F7: External display (projector) */
			tpckbd_map_key_clear(KEY_SWITCHVIDEOMODE);
			return 1;
		case 0x00f5: /* Fn-F8: Wireless */
			tpckbd_map_key_clear(KEY_WLAN);
			return 1;
		case 0x00f6: /* Fn-F9: Control panel */
			tpckbd_map_key_clear(KEY_CONFIG);
			return 1;
		case 0x00f8: /* Fn-F11: View open applications (3 boxes) */
			tpckbd_map_key_clear(KEY_FN_F11);
			return 1;
		case 0x00fa: /* Fn-Esc: Fn-lock toggle */
			tpckbd_map_key_clear(KEY_FN_ESC);
			return 1;
		}
	}

	return 0;
}
static int tpcompactkbd_input_mapping(struct hid_device *hdev,
		struct hid_input *hi, struct hid_field *field,
		struct hid_usage *usage, unsigned long **bit, int *max)
{
	if (hdev->product == USB_DEVICE_ID_LENOVO_CUSBKBD)
		return tpcompactkbd_usb_input_mapping(hdev, hi, field,
						usage, bit, max);
	return tpcompactkbd_bt_input_mapping(hdev, hi, field, usage, bit, max);
}

static int tpcompactkbd_event(struct hid_device *hdev, struct hid_field *field,
		struct hid_usage *usage, __s32 value)
{
	/* Switch fn-lock on fn-esc */
	if (unlikely(usage->code == KEY_FN_ESC && value))
		tpcompactkbd_toggle_fnlock(hdev);

	return 0;
}

static int tpcompactkbd_probe(struct hid_device *hdev,
			const struct hid_device_id *id)
{
	int ret;
	struct tpcompactkbd_sc *tpcsc;

	tpcsc = devm_kzalloc(&hdev->dev, sizeof(*tpcsc), GFP_KERNEL);
	if (tpcsc == NULL) {
		hid_err(hdev, "can't alloc keyboard descriptor\n");
		return -ENOMEM;
	}
	hid_set_drvdata(hdev, tpcsc);

	ret = hid_parse(hdev);
	if (ret) {
		hid_err(hdev, "hid_parse failed\n");
		return ret;
	}

	ret = hid_hw_start(hdev, HID_CONNECT_DEFAULT);
	if (ret) {
		hid_err(hdev, "hid_hw_start failed\n");
		return ret;
	}

	/*
	 * Tell the keyboard a driver understands it, and turn F7, F9, F11 into
	 * regular keys
	 */
	ret = tpcompactkbd_send_cmd(hdev, 0x01, 0x03);
	if (ret)
		hid_warn(hdev, "Failed to switch F7/9/11 into regular keys\n");

	/* Toggle once to init the state of fn-lock */
	tpcsc->fn_lock = 0;
	tpcompactkbd_toggle_fnlock(hdev);

	return 0;
}

static const struct hid_device_id tpcompactkbd_devices[] = {
	{HID_USB_DEVICE(USB_VENDOR_ID_LENOVO, USB_DEVICE_ID_LENOVO_CUSBKBD)},
	{HID_BLUETOOTH_DEVICE(USB_VENDOR_ID_LENOVO, USB_DEVICE_ID_LENOVO_CBTKBD)},

	{ }
};
MODULE_DEVICE_TABLE(hid, tpcompactkbd_devices);

static struct hid_driver tpcompactkbd_driver = {
	.name = "lenovo_tpcompactkbd",
	.id_table = tpcompactkbd_devices,
	.input_mapping = tpcompactkbd_input_mapping,
	.probe = tpcompactkbd_probe,
	.event = tpcompactkbd_event,
};
module_hid_driver(tpcompactkbd_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jamie Lentin <jm@lentin.co.uk>");
MODULE_DESCRIPTION("ThinkPad Compact Keyboard with TrackPoint input driver");
