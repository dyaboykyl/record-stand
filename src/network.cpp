
#include <EasyLogger.h>
#include <WiFi.h>

#include <vector>

#define TAG "WIFI"

auto ssids = std::vector<String>();
void setSsids(int count) {
  ssids.clear();
  for (int i = 0; i < count; i++) {
    ssids.push_back(WiFi.SSID(i));
  }
}

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

bool isConnected() {
  return WiFi.isConnected();
  WiFi.status();
}

int getWifiStatus() {
  return WiFi.status();
}

bool checkWifiScan() {
  int result = WiFi.scanComplete();
  if (result == WIFI_SCAN_RUNNING) {
    return true;
  }

  if (result == WIFI_SCAN_FAILED || result < 0) {
    LOG_ERROR(TAG, (String) "Couldn't get networks: " + result);
    // WiFi.scanNetworks(true);
    // handle
    return false;
  }

  setSsids(result);

  return false;
}

void scanNetworks() {
  LOG_DEBUG(TAG, "Scanning");
  bool async = true;
  int result = WiFi.scanNetworks(async);
  if (result == WIFI_SCAN_FAILED) {
    LOG_ERROR(TAG, "Failed to scan");
    // handle
  }

  if (result >= 0) {
    LOG_INFO(TAG, "Scan already finished");
    setSsids(result);
  }
}

std::vector<String>* getNetworks() { return &ssids; }

void connectToWifi(const char* ssid, const char* password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Waiting for WiFi to connect...");
  // while (WiFi.status() != WL_CONNECTED) {
  //   Serial.println(WiFi.status());
  // }
  // Serial.println(" connected");
}
