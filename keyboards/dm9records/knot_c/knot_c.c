// Copyright 2026 Takuya Urakawa @hsgw (5z6p.ccom)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "knot_c.h"
#include "i2c_master.h"
#include "cy8cmbr3108.h"
#include "pmw3610.h"
#include "raw_hid.h"

#ifdef KNOT_C_DEBUG
#    include "print.h"
#endif

knot_c_config_t knot_c_config;

void eeconfig_init_kb(void) {
    knot_c_config_reset();
    eeconfig_init_user();
}

void knot_c_config_load(void) {
    if (!eeconfig_is_enabled()) {
        eeconfig_init();
    }
    knot_c_config.raw = eeconfig_read_user();

    // Validate or reset if needed (0xFF means uninitialized in this bitfield)
    if (knot_c_config.cpi_coeff == 0xFF) {
        knot_c_config_reset();
    }
}

void knot_c_config_save(void) {
    eeconfig_update_user(knot_c_config.raw);
}

void knot_c_config_reset(void) {
    knot_c_config.cpi_coeff        = DEFAULT_CPI;
    knot_c_config.scroll_divisor   = DEFAULT_SCROLL_DIVISOR;
    knot_c_config.disable_h_scroll = DEFAULT_DISABLE_H_SCROLL;
}

void keyboard_pre_init_kb(void) {
    i2c_init();

#ifndef KNOT_C_DEBUG
    // Normal boot initialization
    wait_ms(15);
    cy8cmbr3108_init();
#endif

    keyboard_pre_init_user();
}

void keyboard_post_init_kb(void) {
    // Load configuration from EEPROM
    knot_c_config_load();

    // Apply CPI to sensor ((cpi_coeff + 1) * 200)
    pmw3610_set_cpi_wrapper((knot_c_config.cpi_coeff + 1) * 200);

#ifdef KNOT_C_DEBUG
    // Debug initialization with delay for USB console
    debug_enable = true;
    wait_ms(5000);
    uprintf("KNOT-C: Debug mode enabled.\n");
    cy8cmbr3108_init();
#endif

    keyboard_post_init_user();
}

extern matrix_row_t matrix[MATRIX_ROWS];

void matrix_scan_kb(void) {
    if (cy8cmbr3108_is_ready()) {
        uint16_t     btn_stat = cy8cmbr3108_read();
        matrix_row_t row      = matrix[0];

        // Map bits to matrix row (0x04 -> Bit 0, 0x20 -> Bit 1)
        if (btn_stat & 0x04)
            row |= (1 << 0);
        else
            row &= ~(1 << 0);

        if (btn_stat & 0x20)
            row |= (1 << 1);
        else
            row &= ~(1 << 1);

        matrix[0] = row;
    }

    matrix_scan_user();
}

// Accumulators for scrolling (avoid losing slow movements)
static int32_t scroll_x_remainder = 0;
static int32_t scroll_y_remainder = 0;

report_mouse_t knot_c_handle_scroll(report_mouse_t mouse_report, bool is_scroll_mode) {
    if (is_scroll_mode) {
        int16_t delta_x = mouse_report.x;
        int16_t delta_y = -mouse_report.y; // Invert Y for natural scrolling

        // Prevent mouse cursor from moving
        mouse_report.x = 0;
        mouse_report.y = 0;

        const int32_t divisor = (int32_t)knot_c_config.scroll_divisor;

        // Handle Horizontal Scroll
        if (knot_c_config.disable_h_scroll) {
            scroll_x_remainder = 0;
            mouse_report.h     = 0;
        } else {
            scroll_x_remainder += (int32_t)delta_x;
            mouse_report.h     = (int8_t)(scroll_x_remainder / divisor);
            scroll_x_remainder %= divisor;
        }

        // Handle Vertical Scroll
        scroll_y_remainder += (int32_t)delta_y;
        mouse_report.v     = (int8_t)(scroll_y_remainder / divisor);
        scroll_y_remainder %= divisor;
    } else {
        // Reset remainders when not in scroll mode to avoid "ghost" scrolling
        scroll_x_remainder = 0;
        scroll_y_remainder = 0;
    }

    return mouse_report;
}

/**
 * @brief Handle custom HID commands for configuration
 * 
 * Protocol Definition (Custom ID: 0xFC)
 * 
 * Command 0x01: Get Settings / Identify
 *   Request:  [0xFC, 0x01]
 *   Response: [0xFC, 0xFD, 'K', 'N', 'O', 'T', Version(0x01), HiresEnabled(0/1), CPI, Divisor, DisableHScroll]
 * 
 * Command 0x02: Set Settings (RAM only)
 *   Request:  [0xFC, 0x02, CPI, Divisor, DisableHScroll]
 *   Response: [0xFC, 0xFD]
 * 
 * Command 0x03: Save Settings (EEPROM)
 *   Request:  [0xFC, 0x03]
 *   Response: [0xFC, 0xFD]
 */
void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    if (data[0] == 0xFC) {
        switch (data[1]) {
            case 0x01: // Get current settings and identify
                // Unique ID: 'KNOT'
                data[2] = 'K';
                data[3] = 'N';
                data[4] = 'O';
                data[5] = 'T';
                // Protocol Version
                data[6] = 0x01;
                // Current Settings
#ifdef POINTING_DEVICE_HIRES_SCROLL_ENABLE
                data[7] = 1; // High Resolution Scroll is enabled in firmware
#else
                data[7] = 0; // Regular Scroll is enabled in firmware
#endif
                data[8]  = knot_c_config.cpi_coeff;
                data[9]  = knot_c_config.scroll_divisor;
                data[10] = knot_c_config.disable_h_scroll;
                length   = 11;
                // Success response
                data[1] = 0xFD;
                break;
            case 0x02: // Set settings (RAM only)
                knot_c_config.cpi_coeff        = data[2];
                knot_c_config.scroll_divisor   = data[3];
                knot_c_config.disable_h_scroll = data[4];
                // Re-apply CPI to sensor immediately
                pmw3610_set_cpi_wrapper((knot_c_config.cpi_coeff + 1) * 200);
                // Success response
                data[1] = 0xFD;
                break;
            case 0x03: // Save settings to EEPROM
                knot_c_config_save();
                // Success response
                data[1] = 0xFD;
                break;
        }
        // Send back response
        raw_hid_send(data, length);
    }
}
