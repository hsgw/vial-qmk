// Copyright 2026 Takuya Urakawa @hsgw (5z6p.ccom)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

/**
 * @brief Configuration structure for knot_c settings stored in EEPROM
 * Uses a 32-bit raw value compatible with eeconfig_read_user/eeconfig_update_user
 */
typedef union {
    uint32_t raw;
    struct {
        uint8_t cpi_coeff;         // CPI / 200 - 1 (0 = 200, 1 = 400, ...)
        uint8_t scroll_divisor;    // Divisor to scale scroll movement
        uint8_t disable_h_scroll;  // 1 to disable horizontal scroll in scroll mode
    } __attribute__((packed));
} knot_c_config_t;

extern knot_c_config_t knot_c_config;

// Forward declarations
void knot_c_config_reset(void);
void knot_c_config_save(void);
void knot_c_config_load(void);

// Helper for PMW3610 CPI (can be shared across keymaps)
void pmw3610_set_cpi_wrapper(uint16_t cpi);

/**
 * @brief Process mouse report for scrolling.
 * Should be called from pointing_device_task_user.
 *
 * @param mouse_report Current mouse report.
 * @param is_scroll_mode True if scroll mode is active.
 * @return report_mouse_t Modified mouse report.
 */
report_mouse_t knot_c_handle_scroll(report_mouse_t mouse_report, bool is_scroll_mode);
