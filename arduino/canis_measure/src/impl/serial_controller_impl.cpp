/**
 * @file serial_controller_impl.cpp
 * @brief Serial controller implementation
 *
 * Copyright (c) 2025 toms74209200
 * This software is released under the MIT License.
 * http://opensource.org/licenses/mit-license.php
 */

#include "serial_controller_impl.h"

serial_controller::SerialControllerImpl::SerialControllerImpl(const uint8_t rx,
                                                              const uint8_t tx)
    : rx_(rx), tx_(tx) {}

void serial_controller::SerialControllerImpl::Init() {
  Serial1.begin(9600, SERIAL_8N1, rx_, tx_);
}

void serial_controller::SerialControllerImpl::Write(std::vector<uint8_t> data) {
  Serial1.write(data.data(), data.size());
}

std::vector<uint8_t> serial_controller::SerialControllerImpl::Read(
    const int_fast32_t size) {
  std::vector<uint8_t> buffer(size);
  byte buf[size];
  for (int i = 0; i < size; i++) {
    buf[i] = 0;
  }
  Serial1.readBytes(buf, sizeof(buf));
  std::memcpy(buffer.data(), buf, size);
  return buffer;
}