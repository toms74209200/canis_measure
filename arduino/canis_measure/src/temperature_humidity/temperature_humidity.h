/**
 * @file temperature_humidity.h
 * @brief SHT30 temperature and humidity sensor controller
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#pragma once

#include <cstdint>
#include <memory>
#include <utility>
#include "../i2c_controller/i2c_controller.h"
#include "../sleep/sleep.h"

namespace temperature_humidity {

namespace constants {
constexpr uint8_t kDeviceAddress = 0x44;
}  // namespace constants

class TemperatureHumidity {
 public:
  explicit TemperatureHumidity(
      std::shared_ptr<i2c_controller::I2cController> i2c_controller,
      std::shared_ptr<device_sleep::DeviceSleep> sleep);
  std::pair<float, float> GetTemperatureHumidity();

 private:
  std::shared_ptr<i2c_controller::I2cController> i2c_controller_;
  std::shared_ptr<device_sleep::DeviceSleep> sleep_;
};

}  // namespace temperature_humidity