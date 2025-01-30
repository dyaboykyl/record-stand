

#include <ArduinoJSON.h>
#include <EasyLogger.h>
#include <HttpClient.h>
#include <WiFiClientSecure.h>
#include <acr.h>

#include "network.h"

using namespace std;

#define LABEL "ACR"
#define ACR_HOST "identify-eu-west-1.acrcloud.com"
#define ACR_ENDPOINT "/v1/identify"

#define LOCAL_HOST "192.168.1.129:3000"
#define UPLOAD_WAV_ENDPOINT "/identify"

auto ACR_URL = string("http://").append(ACR_HOST) + ACR_ENDPOINT;
auto LOCAL_URL = string("http://") + LOCAL_HOST + UPLOAD_WAV_ENDPOINT;

static uint8_t *body = NULL;

WiFiClient wifi;
HTTPClient client;

string formDataItem(string name, string value) {
  return "--boundary\r\nContent-Disposition: form-data; name=\"" + name + "\"\r\n\r\n" + value +
         "\r\n";
}

SongInfo identifySongV2(uint8_t *wavData, int size, bool local) {
  if (!isWifiConnected()) {
    LOG_ERROR(LABEL, "Cannot identify song. Not connected to wifi");
    return {};
  }

  string bodyPart1 = formDataItem("access_key", "") + formDataItem("data_type", "audio") +
                     formDataItem("signature_version", "1") +
                     formDataItem("signature", "Ff2r6KobaCflFiQjFkG+B6utuQw=") +
                     formDataItem("timestamp", "0") +
                     formDataItem("sample_bytes", to_string(size)) +
                     "--boundary\r\nContent-Disposition: form-data; name=\"sample\"; "
                     "filename=\"sample.wav\"\r\nContent-Type: application/octet-stream\r\n\r\n";
  string bodyPart2 = "\r\n--boundary--\r\n";

  int bodySize = bodyPart1.length() + size + bodyPart2.length();
  string url = local ? LOCAL_URL : ACR_URL;

  LOG_INFO(LABEL, "Calling " << url.c_str());
  if (!client.begin(String(url.c_str()))) {
    LOG_ERROR(LABEL, "Unable to start reques ");
    return {};
  }

  client.addHeader("Content-Type", "multipart/form-data;boundary=\"boundary\"");
  if (body == NULL) {
    LOG_DEBUG(LABEL, "Allocating body. Free PSRAM:" << ESP.getFreePsram());
    body = (uint8_t *)ps_malloc(bodySize);
    LOG_DEBUG(LABEL, "Body allocated. Free PSRAM:" << ESP.getFreePsram());
  }

  LOG_DEBUG(LABEL, "Copying part 1. size: " << bodyPart1.length());
  memcpy(body, bodyPart1.c_str(), bodyPart1.length());
  LOG_DEBUG(LABEL, "Copying wav data. size: " << size);
  memcpy(body + bodyPart1.length(), wavData, size);
  LOG_DEBUG(LABEL, "Copying part 2. Size: " << bodyPart2.length());
  memcpy(body + bodyPart1.length() + size, bodyPart2.c_str(), bodyPart2.length());

  int result = client.POST(body, bodySize);
  LOG_INFO(LABEL, "Result: " << result);
  if (result != HTTP_CODE_OK) {
    LOG_ERROR(LABEL, "Did not receive success");
    return {};
  }

  auto payload = client.getString();
  LOG_INFO(LABEL, "Response: " << payload);
  JsonDocument doc;
  deserializeJson(doc, payload);
  JsonDocument musicData = doc["metadata"]["music"][0];
  return {
      .artist = string((const char *)musicData["artists"][0]["name"]),
      .song = string((const char *)musicData["title"]),
      .album = string((const char *)musicData["album"]),
  };
}