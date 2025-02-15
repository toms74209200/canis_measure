/**
 * @file i2c_controller_impl.h
 * @brief I2C controller implementation
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#pragma once

#include <Arduino.h>
#include <Wire.h>

#include "../i2c_controller/i2c_controller.h"

namespace i2c_controller {
class I2cControllerImpl : public I2cController {
 public:
  I2cControllerImpl(const byte scl, const byte sda);
  void Init() override;
  void Write(const uint8_t address, std::vector<uint8_t> data) override;
  std::vector<uint8_t> Read(const uint8_t address,
                            const int_fast32_t size) override;

 private:
  byte scl;
  byte sda;
};
}  // namespace i2c_controller