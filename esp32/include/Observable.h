#pragma once

#include <Arduino.h>
#include <esp_log.h>

#include <functional>
#include <vector>

#include "logging.h"

using namespace std;

template <typename T>
class Observable {
 public:
  using Callback = std::function<void(const T&)>;

  Observable() = default;
  explicit Observable(const T& value) : value(value) {}

  void set(const T& newValue) {
    if (value != newValue) {
      value = newValue;
      notify();
    }
  }

  const T& get() const { return value; }

  void subscribe(const Callback& callback, bool notifyImmediately = false) {
    callbacks.push_back(callback);
    if (notifyImmediately) {
      callback(value);
    }
  }

 private:
  void notify() {
    for (const auto& callback : callbacks) {
      callback(value);
    }
  }

  T value{};
  std::vector<Callback> callbacks;
};
