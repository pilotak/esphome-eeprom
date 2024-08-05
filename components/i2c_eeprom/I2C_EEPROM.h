#pragma once
#include "esphome/components/i2c/i2c.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"

namespace esphome {
namespace i2c_eeprom {

class I2C_EEPROM : public Component, public i2c::I2CDevice {
public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::BUS; }

  bool isConnected();

  bool put(uint16_t memaddr, const uint8_t *value, size_t size);

  bool put(uint16_t memaddr, uint8_t value) { return put(memaddr, &value, 1); }

  bool get(uint16_t memaddr, uint8_t *value, size_t size = 1);

  // set variable from config
  void set_size(uint16_t size) {
    _isTwoByteAddress = size > 2048; // 16kb
  }

  Trigger<> *get_on_setup_trigger() {
    // Lazy create
    if (!this->on_setup_trigger_)
      this->on_setup_trigger_ = make_unique<Trigger<>>();
    return this->on_setup_trigger_.get();
  }

protected:
  bool _isTwoByteAddress = false;
  std::unique_ptr<Trigger<>> on_setup_trigger_{nullptr};
};

} // namespace i2c_eeprom
} // namespace esphome
