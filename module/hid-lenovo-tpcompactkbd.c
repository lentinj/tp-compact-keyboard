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
#include <linux/sysfs.h>

#define USB_VENDOR_ID_LENOVO	0x17ef
#define USB_DEVICE_ID_CBTKBD	0x6048

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
 * F8:	KEY_FN_F8		Wireless
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
	if ((usage->hid & HID_USAGE_PAGE) == HID_UP_CONSUMER) {
		set_bit(EV_REP, hi->input->evbit);
		switch (usage->hid & HID_USAGE) {
		case 0x0070:
			/* Used before 18 01 03 is sent to keyboard */
			tpckbd_map_key_clear(KEY_BRIGHTNESSDOWN);
			return 1;
		case 0x006f:
			/* Used before 18 01 03 is sent to keyboard */
			tpckbd_map_key_clear(KEY_BRIGHTNESSUP);
			return 1;
		case 0x03f1:
			tpckbd_map_key_clear(KEY_FN_F8);
			return 1;
		case 0x0221:
			tpckbd_map_key_clear(KEY_SEARCH);
			return 1;
		case 0x03f2:
			tpckbd_map_key_clear(KEY_FN_F12);
			return 1;
		}
	}

	if ((usage->hid & HID_USAGE_PAGE) == HID_UP_MSVENDOR) {
		set_bit(EV_REP, hi->input->evbit);
		switch (usage->hid & HID_USAGE) {
		case 0x00f0:
			tpckbd_map_key_clear(KEY_FN_ESC);
			return 1;
		case 0x00f1:
			tpckbd_map_key_clear(KEY_MICMUTE);
			return 1;
		case 0x00f2:
			tpckbd_map_key_clear(KEY_BRIGHTNESSDOWN);
			return 1;
		case 0x00f3:
			tpckbd_map_key_clear(KEY_BRIGHTNESSUP);
			return 1;
		case 0x00f4:
			tpckbd_map_key_clear(KEY_SWITCHVIDEOMODE);
			return 1;
		case 0x00f5:
			tpckbd_map_key_clear(KEY_FN_F8);
			return 1;
		case 0x00f6:
			tpckbd_map_key_clear(KEY_CONFIG);
			return 1;
		case 0x00f8:
			tpckbd_map_key_clear(KEY_FN_F11);
			return 1;
		case 0x00fa:
			tpckbd_map_key_clear(KEY_FN_ESC);
			return 1;
		}
	}

	return 0;
}

/* Send a config command to the keyboard */
static int tpcompactkbd_send_cmd(struct hid_device *hdev,
			__u8 byte2, __u8 byte3)
{
	unsigned char buf[] = {0x18, byte2, byte3};

	return hdev->hid_output_raw_report(hdev, buf, sizeof(buf),
						HID_OUTPUT_REPORT);
}

/* Set fnlock on or off */
static int tpcompactkbd_set_fnlock(struct hid_device *hdev,
			__u8 fn_lock_on)
{
	return tpcompactkbd_send_cmd(hdev, 0x05, fn_lock_on ? 0x01 : 0x00);
}

/* Control Fn lock via. sysfs node */
static ssize_t fn_lock_store(struct device *dev,
		struct device_attribute *attr,
		const char *buf,
		size_t count)
{
	struct hid_device *hdev = container_of(dev, struct hid_device, dev);
	int value;

	if (kstrtoint(buf, 10, &value))
		return -EINVAL;

	if (tpcompactkbd_set_fnlock(hdev, value == 1))
		return -EINVAL;

	return count;
}
static struct device_attribute dev_attr_fn_lock = __ATTR_WO(fn_lock);

static struct attribute *attributes_pointer[] = {
	&dev_attr_fn_lock.attr,
	NULL
};

static const struct attribute_group tpcompactkbd_attr_group_pointer = {
	.attrs = attributes_pointer,
};

static int tpcompactkbd_probe(struct hid_device *hdev,
			const struct hid_device_id *id)
{
	int ret;

	ret = hid_parse(hdev);
	if (ret) {
		hid_err(hdev, "hid_parse failed\n");
		goto err;
	}

	ret = hid_hw_start(hdev, HID_CONNECT_DEFAULT);
	if (ret) {
		hid_err(hdev, "hid_hw_start failed\n");
		goto err;
	}

	/*
	 * Tell the keyboard a driver understands it, and turn F7, F9, F11 into
	 * regular keys
	 */
	ret = tpcompactkbd_send_cmd(hdev, 0x01, 0x03);
	if (ret)
		hid_warn(hdev, "Failed to switch F7/9/11 into regular keys\n");

	/* Init fnlock node, start with Fn lock on */
	if (sysfs_create_group(&hdev->dev.kobj,
				&tpcompactkbd_attr_group_pointer)) {
		hid_warn(hdev, "Could not create sysfs group\n");
	}
	tpcompactkbd_set_fnlock(hdev, 1);

	return 0;

err:
	return ret;
}

static void tpcompactkbd_remove(struct hid_device *hdev)
{
	sysfs_remove_group(&hdev->dev.kobj,
			&tpcompactkbd_attr_group_pointer);

	hid_hw_stop(hdev);
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
	.probe = tpcompactkbd_probe,
	.remove = tpcompactkbd_remove,
};
module_hid_driver(tpcompactkbd_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jamie Lentin <jm@lentin.co.uk>");
MODULE_DESCRIPTION("ThinkPad Compact Keyboard with TrackPoint input driver");
