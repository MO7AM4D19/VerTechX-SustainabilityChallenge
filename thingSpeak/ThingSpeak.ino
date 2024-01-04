#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

#define DHTPIN 3         // DHT11 data pin
#define DHTTYPE DHT11    // DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE);

#define SOIL_MOISTURE_PIN A0  // Soil Moisture sensor analog pin
#define PUMP_PIN 5            // Pin connected to the pump
#define PIEZO_PIN 6           // Pin connected to the piezoelectric element

const char *ssid = "Salma";
const char *password = "10101990";
const char *thingSpeakAPIKey = "JYTW4G1XI6BMFBOI";

WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(PIEZO_PIN, OUTPUT);

  ThingSpeak.begin(client);
  connectToWiFi();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);

  // Send data to ThingSpeak
  sendToThingSpeak(humidity, temperature, soilMoisture);

  // Control the pump based on soil moisture
  if (soilMoisture < 400) {  // Adjust this threshold based on soybean moisture requirements
    digitalWrite(PUMP_PIN, HIGH);  // Turn on the pump
  } else {
    digitalWrite(PUMP_PIN, LOW);   // Turn off the pump
  }

  // Control the piezoelectric element based on humidity
  if (humidity > 60) {  // Adjust this threshold based on soybean humidity requirements
    tone(PIEZO_PIN, 1000, 1000);  // Play a tone on the piezoelectric element for 1 second
  }

  delay(60000);  // Delay for 1 minute before the next iteration
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void sendToThingSpeak(float humidity, float temperature, int soilMoisture) {
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, soilMoisture);

  int status = ThingSpeak.writeFields(1, thingSpeakAPIKey);

  if (status == 200) {
    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Failed to connect to ThingSpeak");
  }
}