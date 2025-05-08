#pragma once

#include <Arduino.h>
#include <esp_log.h>

#include <functional>
#include <vector>

#include "logging.h"

const auto LABEL = "Observable";

using namespace std;

// const auto logger = Logger("Observable");

template <typename T>
class Observable {
 public:
  using Callback = std::function<void(const T&)>;

  Observable() = default;
  explicit Observable(const T& value) : value(value) {}

  void set(const T& newValue) {
    int addr = (int)&value;
    Serial.print(addr, HEX);
    Serial.println(" set");
    if (value != newValue) {
      value = newValue;
      notify();
    }
    addr = (int)&value;
    Serial.print(addr, HEX);
    Serial.println(" set done");
  }

  const T& get() const { return value; }

  void subscribe(const Callback& callback, bool notifyImmediately = false) {
    // logger.info("[subscribe] %d 34", 23);
    callbacks.push_back(callback);
    int addr = (int)&value;
    Serial.print(addr, HEX);
    Serial.print(" subscribe: ");
    Serial.println(callbacks.size());
    if (notifyImmediately) {
      callback(value);
    }
  }

 private:
  void notify() {
    int addr = (int)&value;
    Serial.print(addr, HEX);
    Serial.print(" notify: ");
    Serial.println(callbacks.size());
    for (const auto& callback : callbacks) {
      callback(value);
    }
  }

  T value{};
  std::vector<Callback> callbacks;
};

// template <typename T>
// class Observable {
//  public:
//   using Callback = std::function<void(const T&)>;

//   Observable() = default;
//   explicit Observable(const T& value) : value(value) {}

//   void set(const T& newValue);
//   const T& get();
//   void subscribe(const Callback& callback);

//  private:
//   T value{};
//   std::vector<Callback> callbacks;
//   void notify();
// };
