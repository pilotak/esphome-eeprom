# esphome-eeprom

[![build](https://github.com/pilotak/esphome-eeprom/actions/workflows/build.yml/badge.svg)](https://github.com/pilotak/esphome-eeprom/actions/workflows/build.yml)

Lets you write/read EEPROM in lambdas

```yaml
external_components:
  - source: github://pilotak/esphome-eeprom
    components: [i2c_eeprom]

i2c:
  scl: 16
  sda: 17
  id: i2c_1

i2c_eeprom:
  - id: eeprom_1
    size: 16KB
  - id: eeprom_2
    size: 32KB
    i2c_id: i2c_1
    address: 0x52

some_option:
  on_something:
    - lambda: |-
        uint8_t write_byte = 0b1;
        eeprom_1->put(0x0001, write_byte);

        uint8_t read_byte;
        eeprom_1->get(0x0001, &read_byte);
        ESP_LOGD("eeprom","Read: 0x%02X ", read_byte);
```

> Multiple devices can be used at the same time.

### Configuration

- **size** - _(required)_ Set the EEPPROM size: `1KB`, `2KB`, `4KB`, `8KB`, `16KB`, `32KB`, `64KB`, `128KB`, `256KB`, `512KB`
- **id** - _(required)_ Unique ID for use in lambdas
- **i2c_id** - _(optional)_ ID of the I2C bus
- **address** - _(optional)_(default 0x50) I2C address
- **on_setup** - _(optional)_ Automation to run after setup

Please see [example](./example.yaml) how to use it action.
