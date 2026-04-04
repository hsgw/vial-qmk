#pragma once

#define I2C_DRIVER I2CD1
#define I2C1_SCL_PIN GP27
#define I2C1_SDA_PIN GP26
#define I2C1_SCL_SPEED 100000

/* Pointing Device */
#define POINTING_DEVICE_ROTATION_90
#define POINTING_DEVICE_INVERT_X

/* PMW3610 */
#define PMW3610_CS_PIN GP13
#define PMW3610_SCLK_PIN GP12
#define PMW3610_SDIO_PIN GP11

/* Default Configuration */
#ifndef DEFAULT_CPI
#    define DEFAULT_CPI 3 // CPI = 200 * (N+1) = 800
#endif
#ifndef DEFAULT_SCROLL_DIVISOR
#    define DEFAULT_SCROLL_DIVISOR 16
#endif
#ifndef DEFAULT_DISABLE_H_SCROLL
#    define DEFAULT_DISABLE_H_SCROLL 0
#endif

// high resolution scroll settings
#ifndef POINTING_DEVICE_HIRES_SCROLL_MULTIPLIER
#    define POINTING_DEVICE_HIRES_SCROLL_MULTIPLIER 12
#endif

// Enable this to use debug features (delayed init, console logs).
// Requires CONSOLE_ENABLE = yes in rules.mk
// #define KNOT_C_DEBUG
