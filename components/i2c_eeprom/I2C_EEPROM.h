#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"

namespace esphome {
namespace i2c_eeprom {

class I2C_EEPROM : public Component, public i2c::I2CDevice {
public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::BUS; }

  bool isConnected();

  bool put(uint16_t memaddr, uint8_t value);
  bool get(uint16_t memaddr, uint8_t *value);
};

} // namespace i2c_eeprom
} // namespace esphome
