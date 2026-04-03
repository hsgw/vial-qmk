# knot-c vial keymap

## Use high resolution scroll

Define `POINTING_DEVICE_HIRES_SCROLL_ENABLE`.

```bash
qmk compile -kb dm9records/knot_c -km vial -e 'CDEFS += -DPOINTING_DEVICE_HIRES_SCROLL_ENABLE'
```

## Custom Settings

This keymap includes custom settings that can be configured via Raw HID or by modifying `config.h`.

### Trackball Settings

- **CPI (Counts Per Inch)**:
  - Controls the sensitivity of the PMW3610 sensor.
  - Formula: `CPI = (cpi_coeff + 1) * 200`.
  - Default: `cpi_coeff = 3` (800 CPI).
- **Scroll Sensitivity (Divisor)**:
  - Only applicable when high-resolution scrolling is **disabled**.
  - Trackball movement is divided by this value to determine scroll wheel steps.
  - Default: 16. Higher values result in slower, more precise scrolling.
- **Horizontal Scroll**:
  - Can be disabled to prevent accidental horizontal scrolling while in Scroll mode.
  - Default: Enabled.

### Scroll Mode

The **Scroll Mode** is activated by switching to layer 4 (`_SCROLL`). In this mode:
- Trackball vertical movement maps to the vertical scroll wheel (inverted for natural scrolling).
- Trackball horizontal movement maps to the horizontal scroll wheel (unless disabled).
- The mouse cursor remains stationary.

### Raw HID Configuration Protocol

Custom configuration tools can use the following Raw HID protocol (Command ID `0xFC`):

- **Get/Identify** (`0xFC 0x01`): Returns device identification ('KNOT'), protocol version, and current RAM settings.
- **Set Settings** (`0xFC 0x02 [cpi_coeff] [divisor_high] [divisor_low] [disable_h_scroll]`): Updates settings in RAM immediately.
- **Save Settings** (`0xFC 0x03`): Persists current RAM settings to EEPROM.
