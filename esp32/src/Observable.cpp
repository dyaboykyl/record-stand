
// #include "Observable.h"

// #include <esp_log.h>

// #include <functional>
// #include <vector>

// #include "logging.h"

// using namespace std;

// static auto logger = Logger("Observable");
// template <typename T>
// class Observable {
//   void set(const T& newValue) {
//     if (value != newValue) {
//       value = newValue;
//       notify();
//     }
//   }

//   const T& get() { return value; }
//   void subscribe(const Callback& callback) {
//     // logger.info("[subscribe] %d 34", 23);
//     callbacks.push_back(callback);
//   }

//   private:
//   void notify() {
//     for (const auto& callback : callbacks) {
//       callback(value);
//     }
//   }
// };

// // template <typename T>
// // void Observable<T>::set(const T& newValue) {
// //   if (value != newValue) {
// //     value = newValue;
// //     notify();
// //   }
// // }

// // template <typename T>
// // const T& Observable<T>::get() {
// //   return value;
// // }

// // template <typename T>
// // void Observable<T>::subscribe(const Callback& callback) {
// //   // logger.info("[subscribe] %d 34", 23);
// //   callbacks.push_back(callback);
// // }

// // template <typename T>
// // void Observable<T>::notify() {
// //   for (const auto& callback : callbacks) {
// //     callback(value);
// //   }
// // }
