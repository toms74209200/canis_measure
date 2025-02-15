/**
 * @file air_pressure.cpp
 * @brief BMP280 air pressure sensor controller
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include "air_pressure.h"

air_pressure::AirPressure::AirPressure(
    std::shared_ptr<i2c_controller::I2cController> i2c_controller)
    : i2c_controller_(i2c_controller) {}

void air_pressure::AirPressure::Init() {
  std::vector<uint8_t> config{0xF4, 0x27};
  i2c_controller_->Write(constants::kDeviceAddress, config);
}

float air_pressure::AirPressure::GetTemperature() {
  std::vector<uint8_t> addr{constants::kTemperatureAddress};
  i2c_controller_->Write(constants::kDeviceAddress, addr);

  auto data = i2c_controller_->Read(constants::kDeviceAddress, 3);

  int_fast32_t temperature = (data[0] << 12) + (data[1] << 4) + (data[2] >> 4);
  float temperature_data = 0.0025 * static_cast<float>(temperature);

  return temperature_data;
}

float air_pressure::AirPressure::GetAirPressure() {
  std::vector<uint8_t> addr{constants::kPressureAddress};
  i2c_controller_->Write(constants::kDeviceAddress, addr);

  auto data = i2c_controller_->Read(constants::kDeviceAddress, 3);

  int_fast32_t pressure = (data[0] << 12) + (data[1] << 4) + (data[2] >> 4);
  float pressure_data = 0.0025 * static_cast<float>(pressure);

  return pressure_data;
}