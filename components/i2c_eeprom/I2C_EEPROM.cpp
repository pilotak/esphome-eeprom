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

  ESP_LOGCONFIG(TAG, "OK");

  this->on_setup_trigger_->trigger();
}

void I2C_EEPROM::dump_config() {
  ESP_LOGCONFIG(TAG, "EEPROM:");
  LOG_I2C_DEVICE(this);
}

bool I2C_EEPROM::isConnected() {
  i2c::ErrorCode err = this->bus_->write(this->address_, nullptr, 0);
  return (err == i2c::ERROR_OK);
}

bool I2C_EEPROM::put(uint16_t memaddr, uint8_t value) {
  uint8_t data[3];
  i2c::ErrorCode err;

  if (_isTwoByteAddress) {
    data[0] = memaddr >> 8;
    data[1] = memaddr & 0xFF;
    data[2] = value;

    err = this->write(data, 3);
  } else {
    data[0] = memaddr & 0xFF;
    data[1] = value;
    err = this->write(data, 2);
  }

  delay(5); // EEPROM takes 5ms to write

  if (err != i2c::ERROR_OK) {
    this->mark_failed();
    ESP_LOGE(TAG, "Write failed!");
    return false;
  }

  return true;
}

bool I2C_EEPROM::get(uint16_t memaddr, uint8_t *value) {
  uint8_t data[2];
  i2c::ErrorCode err;

  if (_isTwoByteAddress) {
    data[0] = memaddr >> 8;
    data[1] = memaddr & 0xFF;

    err = this->write(data, 2);
  } else {
    data[0] = memaddr & 0xFF;
    err = this->write(data, 1);
  }

  err = this->read(value, 1);

  if (err != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Reading failed!");
    this->mark_failed();
    return false;
  }

  return true;
}

} // namespace i2c_eeprom
} // namespace esphome
