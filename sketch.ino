#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>

// WiFi Credentials
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""

// Fake Edge API URL
#define ENDPOINT_URL "https://iot-edge-sample-wv71.free.beeceptor.com/api/v1/data-records"

// HTTP Client
HTTPClient httpClient;

// HTTP Header Parameters
#define CONTENT_TYPE_HEADER "Content-Type"
#define APPLICATION_JSON "application/json"

#define DEVICE_ID "HC001"

void setup() {
  // Serial Output initialization
  Serial.begin(115200);

  // WiFi Setup
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  // Sample Data Record Build
  JsonDocument dataRecord;
  dataRecord["deviceId"] = DEVICE_ID;
  dataRecord["distance"] = 250;

  String dataRecordResource;

  serializeJson(dataRecord, dataRecordResource);

  httpClient.begin(ENDPOINT_URL);

  httpClient.addHeader(CONTENT_TYPE_HEADER, APPLICATION_JSON);

  // Make HTTP POST Request
  httpClient.POST(dataRecordResource);

  // Check Response
  JsonDocument response;
  String responseResource;
  responseResource = httpClient.getString();
  deserializeJson(response, responseResource);
  serializeJsonPretty(response, Serial);
  // Manipulate Response
  Serial.println();
  Serial.println("Distance: ");
  Serial.println(response["distance"].as<long>());
  // Close Request session
  httpClient.end();

  // Get All
  httpClient.begin(ENDPOINT_URL);
  int httpResponseCode = httpClient.GET();
  Serial.print("Response code: ");
  Serial.println(httpResponseCode);

  responseResource = httpClient.getString();
  deserializeJson(response, responseResource);
  serializeJsonPretty(response, Serial);
  Serial.println();

  // Manipulate each element
  Serial.println("About to show ids");
  JsonArray dataRecords = response.as<JsonArray>();
  if(dataRecords != NULL) {
    for(JsonVariant dataRecord : dataRecords) {
      Serial.print("id: ");
      Serial.println(dataRecord["id"].as<String>());
    }
  }
  // End Request
  httpClient.end();

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
}
