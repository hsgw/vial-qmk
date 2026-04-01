# knot-c vial keymap

## Use high resolution scroll

Define `POINTING_DEVICE_HIRES_SCROLL_ENABLE`.

```bash
qmk compile -kb dm9records/knot_c -km vial -e 'CDEFS += -DPOINTING_DEVICE_HIRES_SCROLL_ENABLE'
```
