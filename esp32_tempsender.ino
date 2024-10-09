#include <WiFi.h>
#include <HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin where the DS18B20 sensor is connected
const int oneWireBus = 4; // GPIO4 (Adjust according to your wiring)

// Replace with your Wi-Fi credentials
const char* ssid = "your_ssid";
const char* password = "your_pass";

// Replace with your server URL
const char* serverUrl = "http://your-website.org/submit";

// Initialize OneWire and DallasTemperature libraries
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("Connected to Wi-Fi");
  
  // Start the DS18B20 sensor
  sensors.begin();
}

void loop() {
  // Request temperature reading
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  // Print temperature to Serial (for debugging)
  Serial.print("Temperature: ");
  Serial.println(temperatureC);
  
  // Send temperature data to the server
  if (WiFi.status() == WL_CONNECTED) { // Check if we're connected to Wi-Fi
    HTTPClient http;

    // Start the HTTP POST request
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");
    
    // Prepare JSON payload
    String jsonPayload = "{\"temperature\": " + String(temperatureC, 2) + "}";
    Serial.println("Sending data: " + jsonPayload);

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully, response code: " + String(httpResponseCode));
    } else {
      Serial.println("Error sending data, response code: " + String(httpResponseCode));
    }

    // Close connection
    http.end();
  }

  // Wait for 5 minutes (300000 ms) before sending the next reading
  delay(300000);
}
