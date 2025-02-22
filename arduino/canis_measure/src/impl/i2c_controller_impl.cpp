/**
 * @file i2c_controller_impl.cpp
 * @brief I2C controller implementation
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include "i2c_controller_impl.h"

i2c_controller::I2cControllerImpl::I2cControllerImpl(const uint8_t scl,
                                                     const uint8_t sda) {
  I2cControllerImpl::scl = scl;
  I2cControllerImpl::sda = sda;
}

void i2c_controller::I2cControllerImpl::Init() {
  digitalWrite(scl, HIGH);
  digitalWrite(sda, HIGH);
  pinMode(scl, OUTPUT);
  pinMode(sda, INPUT);
}

void i2c_controller::I2cControllerImpl::Start() {
  pinMode(sda, INPUT);
  pinMode(scl, OUTPUT);
  digitalWrite(scl, HIGH);
  digitalWrite(sda, HIGH);
  device_sleep::DeviceSleep::For(std::chrono::microseconds(kSclPeriodUs));

  digitalWrite(sda, LOW);
  pinMode(sda, OUTPUT);
  device_sleep::DeviceSleep::For(std::chrono::microseconds(kSclPeriodUs));

  digitalWrite(scl, LOW);
  device_sleep::DeviceSleep::For(std::chrono::microseconds(kSclPeriodUs));
}

void i2c_controller::I2cControllerImpl::Stop() {
  digitalWrite(sda, LOW);
  pinMode(sda, OUTPUT);
  device_sleep::DeviceSleep::For(std::chrono::microseconds(kSclPeriodUs));

  digitalWrite(scl, HIGH);
  device_sleep::DeviceSleep::For(std::chrono::microseconds(kSclPeriodUs));

  digitalWrite(sda, HIGH);
  device_sleep::DeviceSleep::For(std::chrono::microseconds(kSclPeriodUs));
}

bool i2c_controller::I2cControllerImpl::WriteByte(uint8_t data) {
  digitalWrite(sda, HIGH);
  pinMode(sda, OUTPUT);

  for (int i = 0; i < 8; i++) {
    digitalWrite(sda, (data & 0x80) >> 7);
    data <<= 1;
    device_sleep::DeviceSleep::For(std::chrono::microseconds(kClockPeriodUs));

    digitalWrite(scl, HIGH);
    device_sleep::DeviceSleep::For(std::chrono::microseconds(kClockPeriodUs));

    digitalWrite(scl, LOW);
    device_sleep::DeviceSleep::For(std::chrono::microseconds(kClockPeriodUs));
  }

  pinMode(sda, INPUT);
  digitalWrite(sda, HIGH);
  device_sleep::DeviceSleep::For(std::chrono::microseconds(kClockPeriodUs));

  digitalWrite(scl, HIGH);
  device_sleep::DeviceSleep::For(std::chrono::microseconds(kClockPeriodUs));
  bool ack = !digitalRead(sda);

  digitalWrite(scl, LOW);
  return ack;
}

uint8_t i2c_controller::I2cControllerImpl::ReadByte(bool sendAck) {
  uint8_t data = 0;

  pinMode(sda, INPUT);
  digitalWrite(sda, HIGH);

  for (int i = 0; i < 8; i++) {
    device_sleep::DeviceSleep::For(std::chrono::microseconds(kClockPeriodUs));

    digitalWrite(scl, HIGH);
    device_sleep::DeviceSleep::For(std::chrono::microseconds(kClockPeriodUs));
    data = (data << 1) | digitalRead(sda);

    digitalWrite(scl, LOW);
    device_sleep::DeviceSleep::For(std::chrono::microseconds(kClockPeriodUs));
  }

  digitalWrite(sda, !sendAck);
  pinMode(sda, OUTPUT);
  device_sleep::DeviceSleep::For(std::chrono::microseconds(kClockPeriodUs));

  digitalWrite(scl, HIGH);
  device_sleep::DeviceSleep::For(std::chrono::microseconds(kClockPeriodUs));

  digitalWrite(scl, LOW);
  pinMode(sda, INPUT);
  digitalWrite(sda, HIGH);

  return data;
}

void i2c_controller::I2cControllerImpl::Write(const uint8_t address,
                                              std::vector<uint8_t> data) {
  Start();
  WriteByte(address << 1);

  for (const auto& d : data) {
    WriteByte(d);
  }

  Stop();
}

std::vector<uint8_t> i2c_controller::I2cControllerImpl::Read(
    const uint8_t address, const int_fast32_t size) {
  std::vector<uint8_t> data;

  Start();
  WriteByte((address << 1) | 0x01);

  for (int i = 0; i < size; i++) {
    data.push_back(ReadByte(i < size - 1));
  }

  Stop();
  return data;
}