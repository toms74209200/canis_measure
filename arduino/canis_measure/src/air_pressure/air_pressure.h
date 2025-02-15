/**
 * @file air_pressure.h
 * @brief BMP280 air pressure sensor controller
 */

#pragma once

#include <cstdint>
#include <memory>
#include "../i2c_controller/i2c_controller.h"

namespace air_pressure {

namespace constants {
constexpr uint8_t kDeviceAddress = 0x76;
constexpr uint8_t kTemperatureAddress = 0xFA;
constexpr uint8_t kPressureAddress = 0xF7;
}  // namespace constants

class AirPressure {
 public:
  explicit AirPressure(
      std::shared_ptr<i2c_controller::I2cController> i2c_controller);
  void Init();
  float GetTemperature();
  float GetAirPressure();

 private:
  std::shared_ptr<i2c_controller::I2cController> i2c_controller_;
};

}  // namespace air_pressure