// Copyright 2026 Takuya Urakawa @hsgw (5z6p.ccom)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define VIAL_KEYBOARD_UID \
    { 0x21, 0x9B, 0xED, 0xEB, 0x72, 0x9F, 0xC0, 0x49 }

#define VIAL_UNLOCK_COMBO_ROWS \
    { 0, 0 }
#define VIAL_UNLOCK_COMBO_COLS \
    { 2, 5 }

#define DYNAMIC_KEYMAP_LAYER_COUNT 6

// CPI = 200 * (N+1)
#define DEFAULT_CPI 3
#define DEFAULT_SCROLL_DIVISOR 16
#define DEFAULT_DISABLE_H_SCROLL 0

// high resolution scroll settings
#define POINTING_DEVICE_HIRES_SCROLL_MULTIPLIER 12
// #define POINTING_DEVICE_HIRES_SCROLL_EXPONENT 1
// #define POINTING_DEVICE_HIRES_SCROLL_ENABLE 1
