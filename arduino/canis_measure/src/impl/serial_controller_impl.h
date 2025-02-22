/**
 * @file serial_controller_impl.h
 * @brief Serial controller implementation
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#pragma once

#include <cstring>

#include <Arduino.h>
#include "../serial_controller/serial_controller.h"

namespace serial_controller {
class SerialControllerImpl : public SerialController {
 public:
  SerialControllerImpl(const uint8_t rx, const uint8_t tx);
  void Init() override;
  void Write(std::vector<uint8_t> data) override;
  std::vector<uint8_t> Read(const int_fast32_t size) override;

 private:
  uint8_t rx_;
  uint8_t tx_;
};
}  // namespace serial_controller