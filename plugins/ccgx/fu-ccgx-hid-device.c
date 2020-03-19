/*
 * Copyright (C) 2020 Cypress Semiconductor Corporation.
 * Copyright (C) 2020 Richard Hughes <richard@hughsie.com>
 *
 * SPDX-License-Identifier: LGPL-2.1+
 */

#include "config.h"

#include "fu-ccgx-hid-device.h"

struct _FuCcgxHidDevice
{
	FuHidDevice		 parent_instance;
};

G_DEFINE_TYPE (FuCcgxHidDevice, fu_ccgx_hid_device, FU_TYPE_HID_DEVICE)

#define FU_CCGX_HID_DEVICE_TIMEOUT	5000 /* ms */

static gboolean
fu_ccgx_hid_device_detach (FuDevice *device, GError **error)
{
	guint8 buf[5] = {0xEE, 0xBC, 0xA6, 0xB9, 0xA8};

	fu_device_set_status (device, FWUPD_STATUS_DEVICE_RESTART);
	if (!fu_hid_device_set_report (FU_HID_DEVICE (device), buf[0],
				       buf, sizeof(buf),
				       FU_CCGX_HID_DEVICE_TIMEOUT,
				       FU_HID_DEVICE_FLAG_NONE,
				       error)) {
		g_prefix_error (error, "mfg mode error: ");
		return FALSE;
	}
	fu_device_add_flag (device, FWUPD_DEVICE_FLAG_WAIT_FOR_REPLUG);
	return TRUE;
}

static void
fu_ccgx_hid_device_init (FuCcgxHidDevice *self)
{
	fu_device_set_protocol (FU_DEVICE (self), "com.cypress.ccgx");
	fu_device_add_flag (FU_DEVICE (self), FWUPD_DEVICE_FLAG_REQUIRE_AC);
	fu_device_add_flag (FU_DEVICE (self), FWUPD_DEVICE_FLAG_WILL_DISAPPEAR);
	fu_hid_device_set_interface (FU_HID_DEVICE (self), 0x01);
}

static void
fu_ccgx_hid_device_class_init (FuCcgxHidDeviceClass *klass)
{
	FuDeviceClass *klass_device = FU_DEVICE_CLASS (klass);
	klass_device->detach = fu_ccgx_hid_device_detach;
}
