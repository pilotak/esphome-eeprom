# esphome-eeprom

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
  - id: eeprom_2
    i2c_id: i2c_1
    address: 0x52

some_option:
  on_something:
    - lambda: |-
        uint8_t write_byte = 0b1;
        eeprom_1->put(0x0001, write_byte);

        uint8_t read_byte;
        eeprom_1->read(0x0001, &read_byte);
        ESP_LOGD("eeprom","Read: 0x%02X ", read_byte);
```
