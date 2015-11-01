/*
 * Utility to play with Thinkpad compact USB keyboard
 *
 * Based on Hidraw Userspace Example
 *
 * Copyright (c) 2010 Alan Ott <alan@signal11.us>
 * Copyright (c) 2010 Signal 11 Software
 *
 * The code may be used by anyone for any purpose,
 * and can serve as a starting point for developing
 * applications using hidraw.
 */

/* Linux */
#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>

/*
 * Ugly hack to work around failing compilation on systems that don't
 * yet populate new version of hidraw.h to userspace.
 */
#ifndef HIDIOCSFEATURE
#warning Please have your distro update the userspace kernel headers
#define HIDIOCSFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x06, len)
#define HIDIOCGFEATURE(len)    _IOC(_IOC_WRITE|_IOC_READ, 'H', 0x07, len)
#endif

/* Unix */
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* C */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void send_cmd(int fd, int arg1, int arg2)
{
	int res;
	char buf[3];

	memset(buf, 0x0, sizeof(buf));
	buf[0] = 0x13;
	buf[1] = arg1;
	buf[2] = arg2;
	res = ioctl(fd, HIDIOCSFEATURE(3), buf);
	if (res < 0)
		perror("HIDIOCSFEATURE");
	else if (res == 3)
		printf("Sent command %02x %02x\n", arg1, arg2);
	else
		printf("ioctl HIDIOCSFEATURE returned: %d\n", res);
}

int main(int argc, char **argv)
{
	int fd;

	if (argc < 2) {
		printf("Usage: tp-compact-usb-keyboard (hidraw device, e.g. /dev/hidraw1)\n");
		return 1;
	}

	/* Open the Device with non-blocking reads. In real life,
	   don't use a hard coded path; use libudev instead. */
	fd = open(argv[1], O_RDWR|O_NONBLOCK);

	if (fd < 0) {
		perror("Unable to open device");
		return 1;
	}

	/* Same commands as bluetooth, except they start with 13 
	 *
	 * 13 01 03 - Make F7/F9/F11 return custom single events, as opposed to strings of keys
	 * 13 02 05 - Sensitivity 01--09
	 * 13 05 01 - Enable fn-lock
	 * 13 05 00 - Disable fn-lock
	 * 13 09 01 - Disable middle mouse button's standard response (leave custom one working)
	 * 13 09 00 - Enable middle mouse button's standard response (leave custom one working)
	 * 13 02 01 -- 09 - Sensitivity control
	 */

	/* Enable Fn keys */
	send_cmd(fd, 0x01, 0x03);

	/* Enable native middle mouse mode */
	send_cmd(fd, 0x09, 0x01);

	/* Fn-lock enable */
	send_cmd(fd, 0x05, 0x01);
}
