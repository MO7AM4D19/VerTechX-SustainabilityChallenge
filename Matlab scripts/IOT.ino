#include <Cloudchip.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
 
 //not sure about this
 api ="5TQY3BMZO8MG023N"


SoftwareSerial espSerial(2, 3);  
DHT dht(7, DHT22);

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
const char* serverUrl = ""; 

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read data from other soil sensors
  float phValue = readPHSensor();  
  float moistureValue = readMoistureSensor();
  float waterValue = readWaterSensor();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor");
    return;
  }

  String data = "temperature=" + String(temperature) +
                "&humidity=" + String(humidity) +
                "&ph=" + String(phValue) +
                "&moisture=" + String(moistureValue) +
                "&water=" + String(waterValue);

  sendDataToServer(data);
  delay(60000); 
}

float readPHSensor() {
  int phSensorValue = analogRead(A2);
  float phValue = map(phSensorValue, 0, 1023, 0, 14); 
  return phValue;
}

float readMoistureSensor() {
  int moistureSensorValue = analogRead(A3);
  float moistureValue = map(moistureSensorValue, 0, 1023, 0, 100); 
  return moistureValue;
}

float readWaterSensor() {
  int waterSensorValue = analogRead(A4);
  float waterValue = map(waterSensorValue, 0, 1023, 0, 100);
  return waterValue;
}

void sendDataToServer(String data) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, serverUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpResponseCode = http.POST(data);

  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("HTTP Response Code: " + String(httpResponseCode));
    Serial.println(response);
  } else {
    Serial.println("Error on HTTP request");
  }

  http.end();
}