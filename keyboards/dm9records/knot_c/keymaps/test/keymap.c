// Copyright 2026 Takuya Urakawa @hsgw (5z6p.ccom)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

// Adjustment factor for scroll sensitivity
// Increase this value to slow down the scroll speed.
#define SCROLL_DIVISOR 4

// Define layers
enum layers { _BASE, _SCROLL = 4 };

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_1, KC_2, KC_3, KC_4, KC_5, KC_6
    )
};
// clang-format on
