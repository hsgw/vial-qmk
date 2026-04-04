// Copyright 2026 Takuya Urakawa @hsgw (5z6p.ccom)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "knot_c.h"

// Define layers
enum layers { _BASE, _SCROLL = 4 };

// Define keycodes
enum custom_keycodes { TOUCH_L = SAFE_RANGE, TOUCH_R };

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        TOUCH_L, TOUCH_R, MS_BTN1, MS_BTN3, MS_BTN2, MS_BTN4 // 2 Touch, 4 Physical
    ),
    [_SCROLL] = LAYOUT(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
    )
};
// clang-format on

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    return knot_c_handle_scroll(mouse_report, IS_LAYER_ON(_SCROLL));
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case TOUCH_L:
            if (record->event.pressed) {
                layer_on(_SCROLL);
                register_code(KC_F15);
            } else {
                unregister_code(KC_F15);
                layer_off(_SCROLL);
            }
            return false;

        case TOUCH_R:
            if (record->event.pressed) {
                layer_on(_SCROLL);
                register_code(KC_F16);
            } else {
                unregister_code(KC_F16);
                layer_off(_SCROLL);
            }
            return false;
    }
    return true;
}
