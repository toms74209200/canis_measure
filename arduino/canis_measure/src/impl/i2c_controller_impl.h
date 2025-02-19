/**
 * @file i2c_controller_impl.h
 * @brief I2C controller implementation
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */
#pragma once

#include <chrono>

#include <Arduino.h>

#include "../i2c_controller/i2c_controller.h"
#include "../sleep/sleep.h"

namespace i2c_controller {
constexpr int_fast32_t kSclFrequencyhz = 100'000;
constexpr int_fast32_t kSclPeriodUs =
    std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::seconds(1))
        .count() /
    kSclFrequencyhz;
constexpr int_fast32_t kClockPeriodUs = kSclPeriodUs / 2;

class I2cControllerImpl : public I2cController {
 public:
  I2cControllerImpl(const byte scl, const byte sda);
  void Init() override;
  void Write(const uint8_t address, std::vector<uint8_t> data) override;
  std::vector<uint8_t> Read(const uint8_t address,
                            const int_fast32_t size) override;

 private:
  void Start();
  void Stop();
  bool WriteByte(uint8_t data);
  uint8_t ReadByte(bool sendAck);
  byte scl;
  byte sda;
};
}  // namespace i2c_controller