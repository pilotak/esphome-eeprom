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
  uint8_t data[1] = {0};
  i2c::ErrorCode err = this->read(data, 1);
  return (err == i2c::ERROR_OK);
}

bool I2C_EEPROM::put(uint16_t memaddr, const uint8_t *value, size_t size) {
  uint8_t *data = new uint8_t[size + 2]();
  i2c::ErrorCode err;

  if (data == nullptr) {
    ESP_LOGE(TAG, "Not enough RAM");
    return false;
  }

  if (this->isTwoByteAddress_) {
    data[0] = memaddr >> 8;
    data[1] = memaddr & 0xFF;
    memcpy(data + 2, value, size);

    err = this->write(data, size + 2);
  } else {
    data[0] = memaddr & 0xFF;
    memcpy(data + 1, value, size);

    err = this->write(data, size + 1);
  }

  delete[] data;

  delay(5); // EEPROM takes 5ms to write

  if (err != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Write failed!");
    return false;
  }

  return true;
}

bool I2C_EEPROM::get(uint16_t memaddr, uint8_t *value, size_t size) {
  uint8_t data[2];
  i2c::ErrorCode err;

  if (this->isTwoByteAddress_) {
    data[0] = memaddr >> 8;
    data[1] = memaddr & 0xFF;

    err = this->write(data, 2);
  } else {
    data[0] = memaddr & 0xFF;
    err = this->write(data, 1);
  }

  if (err != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Requesting data failed!");
    return false;
  }

  err = this->read(value, size);

  if (err != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Reading failed!");
    return false;
  }

  return true;
}

} // namespace i2c_eeprom
} // namespace esphome
