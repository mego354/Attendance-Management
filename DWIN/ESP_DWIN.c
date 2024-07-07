#include "dwin_lib.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "WIFI ssid"; // e.g. HUAWEI nova 9
const char* password = "WIFI password"; // e.g. 10001000

bool fill = false;
// Function to make HTTP request and return payload
String fetchPayload(const String& url) {
  HTTPClient http;
  http.begin(url);
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    return http.getString();
  } else {
    return ""; // Return an empty string if there's an error
  }
  http.end();
}

// Function to process the payload
void processPayload(const String& payload) {
  // Parse JSON
  DynamicJsonDocument doc(1024); // Adjust size according to JSON response size
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    error_case("error", 0);
  } 
  else { 
    // Extract data
    int casee = doc["case"];
    if (casee == 3)
    {
    const char* message = doc["message"];
    error_case(message, 0);
    }
    else {
    int pic_id = doc["pic_id"];
    const char* department_name = doc["department_name"];
    const char* college_name = doc["college_name"];
    const char* doctor_name = doc["doctor_name"];
    const char* subject_name = doc["subject_name"];
    const char* time = doc["time"];
    const char* students = doc["students"];
    conferm_case(department_name, college_name, subject_name, time, students, pic_id);
    }
  }
}

void setup() {
  Serial.begin(9600);
  delay(1000);
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  String url = "https://megahd.pythonanywhere.com/room/A118";

  // Fetch payload
  String payload = fetchPayload(url);
  if (!payload.isEmpty()) {
    // Process the payload
    processPayload(payload);
  }
  else{
    delay(5000);
  }

}

