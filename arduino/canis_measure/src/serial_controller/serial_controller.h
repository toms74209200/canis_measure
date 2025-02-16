/**
 * @file serial_controller.h
 * @brief Serial controller
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#pragma once

#include <cstdint>
#include <vector>

namespace serial_controller {
class SerialController {
 public:
  virtual ~SerialController() = default;
  virtual void Init() = 0;
  virtual void Write(std::vector<uint8_t> data) = 0;
  virtual std::vector<uint8_t> Read(const int_fast32_t size) = 0;
};

}  // namespace serial_controller