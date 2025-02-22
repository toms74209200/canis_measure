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
  // Configure sensor: normal mode, temperature and pressure oversampling x1
  std::vector<uint8_t> config{constants::kConfigAddress, 0x27};
  i2c_controller_->Write(constants::kDeviceAddress, config);
  ReadCalibrationData();
}

void air_pressure::AirPressure::ReadCalibrationData() {
  std::vector<uint8_t> addr{constants::kCalibrationAddress};
  i2c_controller_->Write(constants::kDeviceAddress, addr);
  auto data = i2c_controller_->Read(constants::kDeviceAddress, 24);

  calib_.dig_T1 = (data[1] << 8) | data[0];
  calib_.dig_T2 = (data[3] << 8) | data[2];
  calib_.dig_T3 = (data[5] << 8) | data[4];
  calib_.dig_P1 = (data[7] << 8) | data[6];
  calib_.dig_P2 = (data[9] << 8) | data[8];
  calib_.dig_P3 = (data[11] << 8) | data[10];
  calib_.dig_P4 = (data[13] << 8) | data[12];
  calib_.dig_P5 = (data[15] << 8) | data[14];
  calib_.dig_P6 = (data[17] << 8) | data[16];
  calib_.dig_P7 = (data[19] << 8) | data[18];
  calib_.dig_P8 = (data[21] << 8) | data[20];
  calib_.dig_P9 = (data[23] << 8) | data[22];
}

int32_t air_pressure::AirPressure::ReadRawTemperature() {
  std::vector<uint8_t> addr{constants::kTemperatureAddress};
  i2c_controller_->Write(constants::kDeviceAddress, addr);
  auto data = i2c_controller_->Read(constants::kDeviceAddress, 3);
  return (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
}

int32_t air_pressure::AirPressure::ReadRawPressure() {
  std::vector<uint8_t> addr{constants::kPressureAddress};
  i2c_controller_->Write(constants::kDeviceAddress, addr);
  auto data = i2c_controller_->Read(constants::kDeviceAddress, 3);
  return ((uint32_t)data[0] << 16 | (uint32_t)data[1] << 8 |
          (uint32_t)data[2]) >>
         4;
}

float air_pressure::AirPressure::GetTemperature() {
  int32_t raw_temp = ReadRawTemperature();
  int32_t t_fine = CalculateTFine(raw_temp, calib_);
  return CalculateTemperature(raw_temp, t_fine);
}

float air_pressure::AirPressure::GetAirPressure() {
  int32_t raw_temp = ReadRawTemperature();
  int32_t t_fine = CalculateTFine(raw_temp, calib_);
  int32_t raw_press = ReadRawPressure();
  return CalculatePressure(raw_press, t_fine, calib_);
}