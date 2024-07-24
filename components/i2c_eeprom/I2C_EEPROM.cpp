#include "I2C_EEPROM.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

namespace esphome {
namespace i2c_eeprom {

static const char *const TAG = "eeprom";

void I2C_EEPROM::setup() {
  ESP_LOGCONFIG(TAG, "Setting up EEPROM...");

  if (!this->isConnected()) {
    ESP_LOGE(TAG, "Device on address 0x%x not found!", this->address_);
    this->mark_failed();
    return;
  }
}

void I2C_EEPROM::dump_config() {
  ESP_LOGCONFIG(TAG, "Setting up EEPROM...");
  LOG_I2C_DEVICE(this);

  if (!this->isConnected()) {
    ESP_LOGE(TAG, "  Device not found!");
  }
}

bool I2C_EEPROM::isConnected() {
  i2c::ErrorCode err = this->bus_->write(this->address_, nullptr, 0, true);
  return (err == i2c::ERROR_OK);
}

bool I2C_EEPROM::put(uint16_t memaddr, uint8_t value) {
  uint8_t data[3];
  data[0] = memaddr >> 8;
  data[1] = memaddr >> 0;
  data[2] = value;

  i2c::ErrorCode err = this->write(data, 3);
  delay(5); // EEPROM takes 5ms to write
  return (err == i2c::ERROR_OK);
}

bool I2C_EEPROM::get(uint16_t memaddr, uint8_t *value) {
  uint8_t data[2];
  data[0] = memaddr >> 8;
  data[1] = memaddr >> 0;
  i2c::ErrorCode err = this->write(data, 2);

  if (err != i2c::ERROR_OK) {
    return false;
  }

  err = this->read(value, 1);
  return (err == i2c::ERROR_OK);
}

} // namespace i2c_eeprom
} // namespace esphome
