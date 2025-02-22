/**
 * @file co2_sensor.h
 * @brief MH-Z19B CO2 sensor controller
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#pragma once

#include <array>
#include <vector>
#include "../serial_controller/serial_controller.h"

namespace co2 {

namespace constants {
constexpr uint8_t kStartByte = 0xFF;
constexpr uint8_t kRequestSensor = 0x01;
constexpr uint8_t kReadCo2 = 0x86;
constexpr uint8_t kCheckSum = 0x79;
constexpr int8_t kTemperatureOffset = 40;

constexpr std::array<uint8_t, 9> kReadCommand = {
    kStartByte, kRequestSensor, kReadCo2, 0x00,     0x00,
    0x00,       0x00,           0x00,     kCheckSum};
}  // namespace constants

class Co2Sensor {
 public:
  explicit Co2Sensor(serial_controller::SerialController* serial_controller);
  /**
   * @brief Initialize the sensor
   * Read the sensor data to initialize the sensor
   */
  void Init();
  /**
   * @brief Retrieve the CO2 value from the sensor
   * @return CO2 value in ppm
   */
  uint16_t GetCo2();
  /**
   * @brief Retrieve the temperature value from the sensor
   * @return Temperature in degrees Celsius
   */
  int16_t GetTemperature();

 private:
  std::vector<uint8_t> ReadMhz19b();
  serial_controller::SerialController* serial_controller_;
};

}  // namespace co2