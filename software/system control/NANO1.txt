#include <Wire.h>
#include <DHT.h>

#define DHTPIN 2 
#define DHTTYPE DHT22 
#define MOISTUREPIN A0 

DHT dht(DHTPIN, DHTTYPE); 
int moistureValue; 

void setup() {
  Wire.begin(8); 
  dht.begin(); 
  pinMode(MOISTUREPIN, INPUT); 
}

void loop() {
  float temperature = dht.readTemperature(); 
  float humidity = dht.readHumidity(); 
  moistureValue = analogRead(MOISTUREPIN);

  Wire.beginTransmission(8); 
  Wire.write((byte*)&temperature, sizeof(temperature)); 
  Wire.write((byte*)&humidity, sizeof(humidity)); 
  Wire.write((byte*)&moistureValue, sizeof(moistureValue)); 
  Wire.endTransmission();

  delay(5000); 
}