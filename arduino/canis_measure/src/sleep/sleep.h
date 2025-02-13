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

namespace device_sleep {

class DeviceSleep {
 public:
  virtual ~DeviceSleep() noexcept = default;
  virtual void For(const std::chrono::milliseconds duration) const noexcept = 0;
};

}  // namespace device_sleep