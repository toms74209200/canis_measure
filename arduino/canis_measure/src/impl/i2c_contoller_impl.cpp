/**
 * @file i2c_controller_impl.cpp
 * @brief I2C controller implementation
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include "i2c_controller_impl.h"

i2c_controller::I2cControllerImpl::I2cControllerImpl(const byte scl,
                                                     const byte sda) {
  I2cControllerImpl::scl = scl;
  I2cControllerImpl::sda = sda;
}

void i2c_controller::I2cControllerImpl::Init() {
  Wire.begin(sda, scl);
}

void i2c_controller::I2cControllerImpl::Write(const uint8_t address,
                                              std::vector<uint8_t> data) {
  Wire.beginTransmission(address);
  for (const auto& d : data) {
    Wire.write(d);
  }
  Wire.endTransmission();
}

std::vector<uint8_t> i2c_controller::I2cControllerImpl::Read(
    const uint8_t address, const int_fast32_t size) {
  Wire.requestFrom(address, size);
  std::vector<uint8_t> data;
  while (Wire.available()) {
    data.push_back(Wire.read());
  }
  return data;
}