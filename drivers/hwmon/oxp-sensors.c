// SPDX-License-Identifier: GPL-2.0+
/*
 * Platform driver for OXP Handhelds that expose fan reading and control
 * via hwmon sysfs.
 *
 * All boards have the same DMI strings and they are told appart by the
 * boot cpu vendor (Intel/AMD). Currently only AMD boards are supported
 * but the code is made to be simple to add other handheld boards in the
 * future.
 * Fan control is provided via pwm interface in the range [0-255]. AMD
 * boards use [0-100] as range in the EC, the written value is scaled to
 * accommodate for that.
 *
 * Copyright (C) 2022 Joaquín I. Aramendía <samsagax@gmail.com>
 */

#include <linux/acpi.h>
#include <linux/dev_printk.h>
#include <linux/dmi.h>
#include <linux/hwmon.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/processor.h>

/* Handle ACPI lock mechanism */
static u32 oxp_mutex;

#define ACPI_LOCK_DELAY_MS	500

static bool lock_global_acpi_lock(void)
{
	return ACPI_SUCCESS(acpi_acquire_global_lock(ACPI_LOCK_DELAY_MS, &oxp_mutex));
}

static bool unlock_global_acpi_lock(void)
{
	return ACPI_SUCCESS(acpi_release_global_lock(oxp_mutex));
}

#define OXP_SENSOR_FAN_REG		0x76 /* Fan reading is 2 registers long */
#define OXP_SENSOR_PWM_ENABLE_REG	0x4A /* PWM enable is 1 register long */
#define OXP_SENSOR_PWM_REG		0x4B /* PWM reading is 1 register long */

static const struct dmi_system_id dmi_table[] = {
	{
		.matches = {
			DMI_MATCH(DMI_BOARD_VENDOR, "ONE-NETBOOK"),
			DMI_EXACT_MATCH(DMI_BOARD_NAME, "ONE XPLAYER"),
		},
	},
	{},
};

/* Helper functions to handle EC read/write */
static int read_from_ec(u8 reg, int size, long *val)
{
	int i;
	int ret;
	u8 buffer;

	if (!lock_global_acpi_lock())
		return -EBUSY;

	*val = 0;
	for (i = 0; i < size; i++) {
		ret = ec_read(reg + i, &buffer);
		if (ret)
			return ret;
		*val <<= i * 8;
		*val += buffer;
	}

	if (!unlock_global_acpi_lock())
		return -EBUSY;

	return 0;
}

static int write_to_ec(const struct device *dev, u8 reg, u8 value)
{
	int ret;

	if (!lock_global_acpi_lock())
		return -EBUSY;

	ret = ec_write(reg, value);

	if (!unlock_global_acpi_lock())
		return -EBUSY;

	return ret;
}

static int oxp_pwm_enable(const struct device *dev)
{
	return write_to_ec(dev, OXP_SENSOR_PWM_ENABLE_REG, 0x01);
}

static int oxp_pwm_disable(const struct device *dev)
{
	return write_to_ec(dev, OXP_SENSOR_PWM_ENABLE_REG, 0x00);
}

/* Callbacks for hwmon interface */
static umode_t oxp_ec_hwmon_is_visible(const void *drvdata,
				       enum hwmon_sensor_types type, u32 attr, int channel)
{
	switch (type) {
	case hwmon_fan:
		return 0444;
	case hwmon_pwm:
		return 0644;
	default:
		return 0;
	}
}

static int oxp_platform_read(struct device *dev, enum hwmon_sensor_types type,
			     u32 attr, int channel, long *val)
{
	int ret;

	switch (type) {
	case hwmon_fan:
		switch (attr) {
		case hwmon_fan_input:
			return read_from_ec(OXP_SENSOR_FAN_REG, 2, val);
		default:
			break;
		}
		break;
	case hwmon_pwm:
		switch (attr) {
		case hwmon_pwm_input:
			ret = read_from_ec(OXP_SENSOR_PWM_REG, 2, val);
			if (ret)
				return ret;
			*val = (*val * 255) / 100;
			return 0;
		case hwmon_pwm_enable:
			return read_from_ec(OXP_SENSOR_PWM_ENABLE_REG, 1, val);
		default:
			break;
		}
		break;
	default:
		break;
	}
	return -EOPNOTSUPP;
}

static int oxp_platform_write(struct device *dev, enum hwmon_sensor_types type,
			      u32 attr, int channel, long val)
{
	switch (type) {
	case hwmon_pwm:
		switch (attr) {
		case hwmon_pwm_enable:
			if (val == 1)
				return oxp_pwm_enable(dev);
			else if (val == 0)
				return oxp_pwm_disable(dev);
			return -EINVAL;
		case hwmon_pwm_input:
			if (val < 0 || val > 255)
				return -EINVAL;
			val = (val * 100) / 255;
			return write_to_ec(dev, OXP_SENSOR_PWM_REG, val);
		default:
			break;
		}
		break;
	default:
		break;
	}
	return -EOPNOTSUPP;
}

/* Known sensors in the OXP EC controllers */
static const struct hwmon_channel_info *oxp_platform_sensors[] = {
	HWMON_CHANNEL_INFO(fan,
			   HWMON_F_INPUT),
	HWMON_CHANNEL_INFO(pwm,
			   HWMON_PWM_INPUT | HWMON_PWM_ENABLE),
	NULL,
};

static const struct hwmon_ops oxp_ec_hwmon_ops = {
	.is_visible = oxp_ec_hwmon_is_visible,
	.read = oxp_platform_read,
	.write = oxp_platform_write,
};

static const struct hwmon_chip_info oxp_ec_chip_info = {
	.ops = &oxp_ec_hwmon_ops,
	.info = oxp_platform_sensors,
};

/* Initialization logic */
static int oxp_platform_probe(struct platform_device *pdev)
{
	const struct dmi_system_id *dmi_entry;
	struct device *dev = &pdev->dev;
	struct device *hwdev;

	/*
	 * Have to check for AMD processor here because DMI strings are the
	 * same between Intel and AMD boards, the only way to tell them appart
	 * is the CPU.
	 * Intel boards seem to have different EC registers and values to
	 * read/write.
	 */
	dmi_entry = dmi_first_match(dmi_table);
	if (!dmi_entry || boot_cpu_data.x86_vendor != X86_VENDOR_AMD)
		return -ENODEV;

	hwdev = devm_hwmon_device_register_with_info(dev, "oxpec", NULL,
						     &oxp_ec_chip_info, NULL);

	return PTR_ERR_OR_ZERO(hwdev);
}

static struct platform_driver oxp_platform_driver = {
	.driver = {
		.name = "oxp-platform",
	},
	.probe = oxp_platform_probe,
};

static struct platform_device *oxp_platform_device;

static int __init oxp_platform_init(void)
{
	oxp_platform_device =
		platform_create_bundle(&oxp_platform_driver,
				       oxp_platform_probe, NULL, 0, NULL, 0);

	return PTR_ERR_OR_ZERO(oxp_platform_device);
}

static void __exit oxp_platform_exit(void)
{
	platform_device_unregister(oxp_platform_device);
	platform_driver_unregister(&oxp_platform_driver);
}

MODULE_DEVICE_TABLE(dmi, dmi_table);

/*
 * module_platform_driver() may be used here but somehow it breaks the module
 * either by preventing it from loading or not exposing hwmon attributes.
 * Either way I'm not smart enough to figure it out so I'll leave init/exit
 * macros for now.
 */
module_init(oxp_platform_init);
module_exit(oxp_platform_exit);

MODULE_AUTHOR("Joaquín Ignacio Aramendía <samsagax@gmail.com>");
MODULE_DESCRIPTION("Platform driver that handles EC sensors of OneXPlayer devices");
MODULE_LICENSE("GPL");
