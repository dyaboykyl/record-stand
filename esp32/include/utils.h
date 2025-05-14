#pragma once

#include <lvgl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <map>
#include <string>
#include <vector>

using namespace std;

struct Pair {
  int x;
  int y;

  bool operator!=(const Pair &other) const { return x != other.x || y != other.y; }
};

inline Pair pairFromString(string str) {
  int x = 0, y = 0;
  if (!str.empty()) {
    auto pos = str.find(",");
    if (pos != string::npos) {
      x = stoi(str.substr(0, pos));
      y = stoi(str.substr(pos + 1));
    }
  }
  return {x, y};
};

inline string pairToString(const Pair &offset) {
  return to_string(offset.x) + "," + to_string(offset.y);
}

static lv_color_t BASE_ONE = lv_color_hex(0x815432);
static lv_color_t BASE_ONE_GRAD = lv_color_hex(0x562d0f);
static lv_color_t BASE_TWO = lv_color_hex(0xcea578);
static lv_color_t DISABLED_COLOR = lv_color_make(200, 200, 200);

inline void hide(lv_obj_t *obj) { lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN); }
inline void show(lv_obj_t *obj) { lv_obj_remove_flag(obj, LV_OBJ_FLAG_HIDDEN); }
inline void enable(lv_obj_t *obj) {
  lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_remove_flag(obj, LV_OBJ_FLAG_PRESS_LOCK);
}
inline void disable(lv_obj_t *obj) {
  lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_flag(obj, LV_OBJ_FLAG_PRESS_LOCK);
}

// Trims leading and trailing whitespace (space, tab, newline, carriage return)
// inline void trim(std::string &s) {
//   size_t start = s.find_first_not_of(" \t\n\r");
//   size_t end = s.find_last_not_of(" \t\n\r");
//   s = (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
// }

// // Removes surrounding quotes from a string if present
// inline void stripQuotes(std::string &s) {
//   if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
//     s = s.substr(1, s.size() - 2);
//   }
// }

// // Parses a flat JSON string with string values only into a std::map
// inline std::map<std::string, std::string> parseJson(const std::string &input) {
//   std::map<std::string, std::string> result;

//   size_t i = 0;
//   size_t len = input.length();

//   // Skip leading spaces and opening brace
//   while (i < len && (input[i] == ' ' || input[i] == '{' || input[i] == '\n' || input[i] == '\r'))
//     ++i;

//   while (i < len) {
//     // Parse key
//     while (i < len && input[i] != '"') ++i;
//     size_t key_start = ++i;
//     while (i < len && input[i] != '"') ++i;
//     std::string key = input.substr(key_start, i - key_start);
//     ++i;  // skip closing quote

//     // Skip to colon
//     while (i < len && input[i] != ':') ++i;
//     ++i;

//     // Skip to starting quote of value
//     while (i < len && input[i] != '"') ++i;
//     size_t val_start = ++i;
//     while (i < len && input[i] != '"') ++i;
//     std::string value = input.substr(val_start, i - val_start);
//     ++i;

//     result[key] = value;

//     // Skip to next key or end
//     while (i < len && input[i] != '"') {
//       if (input[i] == '}') break;
//       ++i;
//     }
//   }

//   return result;
// }

inline std::vector<std::string> split(const std::string &s, const std::string &delimiter) {
  std::vector<std::string> tokens;
  size_t pos = 0;
  std::string token;
  std::string temp = s;
  while ((pos = temp.find(delimiter)) != std::string::npos) {
    token = temp.substr(0, pos);
    tokens.push_back(token);
    temp.erase(0, pos + delimiter.length());
  }
  tokens.push_back(temp);

  return tokens;
}