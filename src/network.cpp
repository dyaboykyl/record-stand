
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
void onWifiEvent(WiFiEvent_t event) {
  Serial.println("[WiFi-event] event: " + event);
  switch (event) {
    case SYSTEM_EVENT_WIFI_READY:
      Serial.println("WiFi interface ready");
      break;
    case SYSTEM_EVENT_SCAN_DONE:
      Serial.println("Completed scan for access points");
      break;
    case SYSTEM_EVENT_STA_START:
      Serial.println("WiFi client started");
      break;
    case SYSTEM_EVENT_STA_STOP:
      Serial.println("WiFi clients stopped");
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
      Serial.println("Connected to access point");
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      Serial.println("Disconnected from WiFi access point");
      break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
      Serial.println("Authentication mode of access point has changed");
      break;
    case SYSTEM_EVENT_STA_GOT_IP:
      Serial.println("Obtained IP address: " + WiFi.localIP());
      break;
    case SYSTEM_EVENT_STA_LOST_IP:
      Serial.println("Lost IP address and IP address is reset to 0");
      //      vTaskDelay( 5000 );
      //      ESP.restart();
      break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
      Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
      Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
      //      ESP.restart();
      break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
      Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
      break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
      Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
      break;
    case SYSTEM_EVENT_AP_START:
      Serial.println("WiFi access point started");
      break;
    case SYSTEM_EVENT_AP_STOP:
      Serial.println("WiFi access point  stopped");
      //      WiFi.mode( WIFI_OFF);
      //      esp_sleep_enable_timer_wakeup( 1000000 * 2 ); // 1 second times how many seconds
      //      wanted esp_deep_sleep_start();
      break;
    case SYSTEM_EVENT_AP_STACONNECTED:
      Serial.println("Client connected");
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      Serial.println("WiFi client disconnected");
      break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
      Serial.println("Assigned IP address to client");
      break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
      Serial.println("Received probe request");
      break;
    case SYSTEM_EVENT_GOT_IP6:
      Serial.println("IPv6 is preferred");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.println("Obtained IP address");
      break;
    default:
      break;
  }
}

void initWifi() {
  WiFi.onEvent(onWifiEvent);
  // WiFi.disconnect(true);
  // delay(100);
}

bool isWifiConnected() { return WiFi.isConnected(); }

int getWifiStatus() { return WiFi.status(); }

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

void connectToWifi(const char* ssid, const char* password, bool connect) {
  // if (!WiFi.mode(WIFI_STA)) {
  //   LOG_ERROR(TAG, "Failed to set wifi mode");
  //   return;
  // }

  if (WiFi.begin(ssid, password) == WL_CONNECT_FAILED) {
    LOG_ERROR(TAG, "Failed to connect to wifi");
    return;
  }

  LOG_INFO(TAG, (String) "Connecting to " + ssid);
  if (connect) {
    auto last = millis();
    while (!isWifiConnected()) {
      delay(5);
      if (millis() - last > 500) {
        LOG_DEBUG(TAG, (String) "WIFI status: " + WiFi.status());
        last = millis();
      }
      // Serial.print(".");
    }
    LOG_INFO(TAG, "Connected");
  }
}
