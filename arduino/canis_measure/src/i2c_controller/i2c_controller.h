/**
 * @file i2c_controller.h
 * @brief I2C controller
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#pragma once

#include <cstdint>
#include <vector>

namespace i2c_controller {
class I2cController {
 public:
  virtual ~I2cController() = default;
  virtual void Init() = 0;
  virtual void Write(const uint8_t address, std::vector<uint8_t> data) = 0;
  virtual std::vector<uint8_t> Read(const uint8_t address,
                                    const int_fast32_t size) = 0;
};

}  // namespace i2c_controller