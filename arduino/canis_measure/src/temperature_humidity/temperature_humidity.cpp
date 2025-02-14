/**
 * @file temperature_humidity.cpp
 * @brief SHT30 temperature and humidity sensor controller
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include "temperature_humidity.h"

temperature_humidity::TemperatureHumidity::TemperatureHumidity(
    std::shared_ptr<i2c_controller::I2cController> i2c_controller)
    : i2c_controller_(i2c_controller) {}

std::pair<float, float>
temperature_humidity::TemperatureHumidity::GetTemperatureHumidity() {
  i2c_controller_->Write(constants::kDeviceAddress,
                         std::vector<uint8_t>{0x2C, 0x06});
  device_sleep::DeviceSleep::For(std::chrono::milliseconds(500));

  auto data = i2c_controller_->Read(constants::kDeviceAddress, 6);

  int_fast32_t temperature = (data[0] << 8) + data[1];
  int_fast32_t humidity = (data[3] << 8) + data[4];

  float temperature_data =
      -45 + 175 * static_cast<float>(temperature) / (2 << 15);
  float humidity_data = 100 * static_cast<float>(humidity) / (2 << 15);

  return std::make_pair(temperature_data, humidity_data);
}