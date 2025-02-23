#define ARDUINO_STL

#include <memory>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "src/air_pressure/air_pressure.h"
#include "src/co2_sensor/co2_sensor.h"
#include "src/display/face.h"
#include "src/i2c_controller/i2c_controller.h"
#include "src/impl/i2c_controller_impl.h"
#include "src/impl/serial_controller_impl.h"
#include "src/temperature_humidity/temperature_humidity.h"

constexpr byte kSclPin = 7;
constexpr byte kSdaPin = 6;
constexpr byte kRxPin = 4;
constexpr byte kTxPin = 5;
constexpr byte kDisplaySclPin = 19;
constexpr byte kDisplaySdaPin = 18;

constexpr auto kScreenWidth = 128;
constexpr auto kScreenHeight = 64;
constexpr auto kOledReset =
    -1;  // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 ssd1306(kScreenWidth, kScreenHeight, &Wire, kOledReset);

std::shared_ptr<i2c_controller::I2cController> i2c_controller_i;
std::unique_ptr<temperature_humidity::TemperatureHumidity> temp_humid;
std::unique_ptr<air_pressure::AirPressure> air_pressure_i;
std::unique_ptr<serial_controller::SerialController> serial_controller_i;
std::unique_ptr<co2::Co2Sensor> co2_sensor_i;

void BlinkEye() {
  ssd1306.clearDisplay();
  ssd1306.drawBitmap(0, 0, display::kFaceClose.data(), kScreenWidth,
                     kScreenHeight, 1);
  ssd1306.display();
  delay(100);
  ssd1306.clearDisplay();
  ssd1306.drawBitmap(0, 0, display::kFace.data(), kScreenWidth, kScreenHeight,
                     1);
  ssd1306.display();
}

void RetrieveAndDisplayTemperature() {
  auto [temperature, humidity] = temp_humid->GetTemperatureHumidity();

  ssd1306.clearDisplay();
  ssd1306.setCursor(0, 0);

  ssd1306.setTextSize(3);
  ssd1306.print(temperature, 1);
  ssd1306.setTextSize(2);
  ssd1306.print(" C");

  ssd1306.println();  // Margin
  ssd1306.setTextSize(3);
  ssd1306.println();

  ssd1306.print(humidity, 1);
  ssd1306.setTextSize(2);
  ssd1306.print(" %");

  ssd1306.display();
}

void RetrieveAndDisplayPressureAndCO2() {
  float pressure = air_pressure_i->GetAirPressure();
  int32_t co2_value = co2_sensor_i->GetCo2();

  ssd1306.clearDisplay();
  ssd1306.setCursor(0, 0);

  ssd1306.setTextSize(3);
  ssd1306.print(pressure, 0);
  ssd1306.setTextSize(2);
  ssd1306.print(" hPa");

  ssd1306.println();  // Margin
  ssd1306.setTextSize(3);
  ssd1306.println();

  ssd1306.print(co2_value);
  ssd1306.setTextSize(2);
  ssd1306.print(" ppm");

  ssd1306.display();
}

constexpr std::array<void (*)(), 2> kDisplayFunctions = {
    RetrieveAndDisplayTemperature, RetrieveAndDisplayPressureAndCO2};

void setup() {
  Wire.begin(kDisplaySdaPin, kDisplaySclPin);

  i2c_controller_i =
      std::make_shared<i2c_controller::I2cControllerImpl>(kSclPin, kSdaPin);
  i2c_controller_i->Init();

  temp_humid = std::make_unique<temperature_humidity::TemperatureHumidity>(
      i2c_controller_i);
  air_pressure_i =
      std::make_unique<air_pressure::AirPressure>(i2c_controller_i);
  serial_controller_i =
      std::make_unique<serial_controller::SerialControllerImpl>(kRxPin, kTxPin);
  serial_controller_i->Init();
  co2_sensor_i = std::make_unique<co2::Co2Sensor>(serial_controller_i.get());

  air_pressure_i->Init();
  co2_sensor_i->Init();

  if (!ssd1306.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (1);
  }

  ssd1306.setRotation(2);
  ssd1306.clearDisplay();
  ssd1306.setTextSize(2);
  ssd1306.setTextColor(WHITE);

  ssd1306.drawBitmap(0, 0, display::kFace.data(), kScreenWidth, kScreenHeight,
                     1);

  ssd1306.display();

  for (int i = 0; i < 10; i++) {
    if (random(10) > 4) {
      BlinkEye();
    }
    device_sleep::DeviceSleep::For(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::seconds(1)));
  }
}

void loop() {
  for (auto display_function : kDisplayFunctions) {
    display_function();
    device_sleep::DeviceSleep::For(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::seconds(5)));
  }
}
