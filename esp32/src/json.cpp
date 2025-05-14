// #include <iostream>
// #include <map>
// #include <sstream>
// #include <string>

// using namespace std;

// map<string, string> parseJson(const string& json) {
//   map<string, string> result;
//   string key, value;
//   bool inKey = false, inValue = false;
//   char quoteChar = '"';

//   for (size_t i = 0; i < json.size(); ++i) {
//     char c = json[i];
//     if (c == '{' || c == '}' || c == ' ' || c == ',') {
//       continue;
//     }
//     if (c == quoteChar) {
//       if (!inKey && !inValue) {
//         inKey = true;
//         key.clear();
//       } else if (inKey) {
//         inKey = false;
//       } else if (inValue) {
//         inValue = false;
//         result[key] = value;
//       }
//     } else if (c == ':' && !inValue) {
//       inValue = true;
//       value.clear();
//     } else {
//       if (inKey) {
//         key += c;
//       } else if (inValue) {
//         value += c;
//       }
//     }
//   }

//   return result;
// }

// // Example usage
// int main() {
//   string json = R"({"key1":"value1","key2":"value2","key3":"value3"})";
//   map<string, string> parsed = parseJson(json);

//   for (const auto& pair : parsed) {
//     cout << pair.first << ": " << pair.second << endl;
//   }

//   return 0;
// }