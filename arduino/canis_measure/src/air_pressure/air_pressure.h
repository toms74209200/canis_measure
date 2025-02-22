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
constexpr uint8_t kCalibrationAddress = 0x88;
constexpr uint8_t kConfigAddress = 0xF4;
}  // namespace constants

struct CalibrationData {
  uint16_t dig_T1;
  int16_t dig_T2;
  int16_t dig_T3;
  uint16_t dig_P1;
  int16_t dig_P2;
  int16_t dig_P3;
  int16_t dig_P4;
  int16_t dig_P5;
  int16_t dig_P6;
  int16_t dig_P7;
  int16_t dig_P8;
  int16_t dig_P9;
};

class AirPressure {
 public:
  explicit AirPressure(
      std::shared_ptr<i2c_controller::I2cController> i2c_controller);
  /**
   * @brief Initialize the sensor
   * Set the sensor to normal mode with temperature and pressure oversampling
   * x1, and read calibration data from the sensor
   */
  void Init();
  /**
   * @brief Retrieve the temperature value from the sensor
   * @return Temperature in degrees Celsius
   */
  float GetTemperature();
  /**
   * @brief Retrieve the air pressure value from the sensor
   * @return Pressure in hPa
   */
  float GetAirPressure();

 private:
  std::shared_ptr<i2c_controller::I2cController> i2c_controller_;
  CalibrationData calib_;
  void ReadCalibrationData();
  int32_t ReadRawTemperature();
  int32_t ReadRawPressure();

  static int32_t CalculateTFine(int32_t raw_temp,
                                CalibrationData calib) noexcept {
    int32_t var1 = ((((raw_temp >> 3) - ((int32_t)calib.dig_T1 << 1)) *
                     ((int32_t)calib.dig_T2)) >>
                    11);

    int32_t var2 = (((((raw_temp >> 4) - ((int32_t)calib.dig_T1)) *
                      ((raw_temp >> 4) - ((int32_t)calib.dig_T1))) >>
                     12) *
                    ((int32_t)calib.dig_T3)) >>
                   14;

    return var1 + var2;
  }
  static float CalculateTemperature(int32_t raw_temp, int32_t t_fine) noexcept {
    float temp = (t_fine * 5 + 128) >> 8;
    return temp / 100.0f;
  }
  static float CalculatePressure(int32_t raw_press, int32_t t_fine,
                                 CalibrationData calib) noexcept {
    int64_t var1, var2, p;

    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)calib.dig_P6;
    var2 = var2 + ((var1 * (int64_t)calib.dig_P5) << 17);
    var2 = var2 + (((int64_t)calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)calib.dig_P3) >> 8) +
           ((var1 * (int64_t)calib.dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)calib.dig_P1) >> 33;

    if (var1 == 0) {
      return 0;
    }

    p = 1048576 - raw_press;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)calib.dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)calib.dig_P7) << 4);
    return (p / 256) / 100.0f;
  }
};

}  // namespace air_pressure