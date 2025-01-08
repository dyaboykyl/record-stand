

#include <ArduinoJSON.h>
#include <EasyLogger.h>
#include <HttpClient.h>
#include <WiFiClientSecure.h>
#include <acr.h>

#define TAG "ACR"
#define ACR_HOST "identify-eu-west-1.acrcloud.com"
#define ACR_ENDPOINT "/v1/identify"

#define LOCAL_HOST "192.168.1.129:3000"
#define UPLOAD_WAV_ENDPOINT "/identify"

String ACR_URL = (String) "http://" + ACR_HOST + ACR_ENDPOINT;
String LOCAL_URL = (String) "http://" + LOCAL_HOST + UPLOAD_WAV_ENDPOINT;

static uint8_t *body = NULL;

WiFiClient wifi;
HTTPClient client;

String formDataItem(String name, String value) {
  return (String) "--boundary\r\nContent-Disposition: form-data; name=\"" + name + "\"\r\n\r\n" +
         value + "\r\n";
}

SongInfo identifySongV2(uint8_t *wavData, int size, bool local) {
  String bodyPart1 = formDataItem("access_key", "") + formDataItem("data_type", "audio") +
                     formDataItem("signature_version", "1") +
                     formDataItem("signature", "Ff2r6KobaCflFiQjFkG+B6utuQw=") +
                     formDataItem("timestamp", "0") + formDataItem("sample_bytes", (String)size) +
                     "--boundary\r\nContent-Disposition: form-data; name=\"sample\"; "
                     "filename=\"sample.wav\"\r\nContent-Type: application/octet-stream\r\n\r\n";
  String bodyPart2 = "\r\n--boundary--\r\n";

  int bodySize = bodyPart1.length() + size + bodyPart2.length();
  String url = local ? LOCAL_URL : ACR_URL;

  LOG_INFO(TAG, (String) "Calling " + url);
  if (!client.begin(url)) {
    LOG_ERROR(TAG, (String) "Unable to start reques ");
    return {};
  }

  client.addHeader("Content-Type", "multipart/form-data;boundary=\"boundary\"");
  if (body == NULL) {
    LOG_DEBUG(TAG, "Allocating body");
    body = (uint8_t *)ps_malloc(bodySize);
    LOG_DEBUG(TAG, (String) "Body allocated. Free PSRAM:" + ESP.getFreePsram());
  }

  LOG_DEBUG(TAG, "Copying part 1");
  memcpy(body, bodyPart1.c_str(), bodyPart1.length());
  LOG_DEBUG(TAG, "Copying wav data");
  memcpy(body + bodyPart1.length(), wavData, size);
  LOG_DEBUG(TAG, "Copying part 2");
  memcpy(body + bodyPart1.length() + size, bodyPart2.c_str(), bodyPart2.length());

  int result = client.POST(body, bodySize);
  LOG_INFO(TAG, (String) "Result: " + result);
  if (result != HTTP_CODE_OK) {
    LOG_ERROR(TAG, "Did not receive success");
    return {};
  }

  String payload = client.getString();
  LOG_INFO(TAG, (String) "Response: " + payload);
  JsonDocument doc;
  deserializeJson(doc, payload);
  JsonDocument musicData = doc["metadata"]["music"][0];
  return {
      .artist = String((const char *)musicData["artists"][0]["name"]),
      .song = String((const char *)musicData["title"]),
      .album = String((const char *)musicData["album"]),
  };
}