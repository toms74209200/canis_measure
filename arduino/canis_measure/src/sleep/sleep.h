/**
 * @file sleep.h
 * @brief Sleep interface
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#pragma once

#include <chrono>
#include <cstdint>

#ifdef ARDUINO
#  include <Arduino.h>
#endif

namespace device_sleep {

class DeviceSleep {
 public:
  static void For(const std::chrono::milliseconds duration) noexcept {
#ifdef ARDUINO
    delay(static_cast<uint32_t>(duration.count()));
#endif
  };

  static void For(const std::chrono::microseconds duration) noexcept {
#ifdef ARDUINO
    noInterrupts();
    delayMicroseconds(static_cast<uint32_t>(duration.count()));
    interrupts();
#endif
  };
};

}  // namespace device_sleep