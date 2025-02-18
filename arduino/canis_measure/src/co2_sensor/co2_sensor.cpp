/**
 * @file co2_sensor.cpp
 * @brief MH-Z19B CO2 sensor controller
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include "co2_sensor.h"

co2::Co2Sensor::Co2Sensor(
    serial_controller::SerialController* serial_controller)
    : serial_controller_(serial_controller) {}

void co2::Co2Sensor::Init() {
  serial_controller_->Init();
  ReadMhz19b();
}

uint16_t co2::Co2Sensor::GetCo2() {
  auto response = ReadMhz19b();
  if (response.empty()) {
    return 0;
  }

  uint16_t co2_data = (response[0] << 8) + response[1];
  return co2_data;
}

int16_t co2::Co2Sensor::GetTemperature() {
  auto response = ReadMhz19b();
  if (response.empty()) {
    return 0;
  }

  int16_t temperature = response[2] - constants::kTemperatureOffset;
  return temperature;
}

std::vector<uint8_t> co2::Co2Sensor::ReadMhz19b() {
  std::vector<uint8_t> cmd(constants::kReadCommand.begin(),
                           constants::kReadCommand.end());
  serial_controller_->Write(cmd);

  const uint16_t retry_count = 10;
  for (uint16_t i = 1;; i++) {
    auto buf = serial_controller_->Read(2);
    if (buf[0] == constants::kStartByte && buf[1] == constants::kReadCo2) {
      break;
    }
    if (i > retry_count) {
      return std::vector<uint8_t>();
    }
  }

  return serial_controller_->Read(7);
}