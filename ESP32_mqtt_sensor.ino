#include <Adafruit_BME280.h>
#include <WiFi.h>
#include <MQTT.h>
#include "Secrets.h"

#define CONNECTION_TIMEOUT 60000
#define DATA_COLLECTION_INTERVAL 3 * 60 * 1000000
#define BATTERY_VOLTAGE_PIN A0

#define SEALEVELPRESSURE_HPA (1013.25)

WiFiClient wifiClient;
MQTTClient mqttClient;
Adafruit_BME280 bme;


void setup() {
  Serial.begin(9600);

  esp_sleep_enable_timer_wakeup(DATA_COLLECTION_INTERVAL);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  mqttClient.begin(MQTT_BROKER, MQTT_PORT, wifiClient);

  unsigned long connectionTimeout = millis() + CONNECTION_TIMEOUT;

  while (!mqttClient.connected()) {
    if (millis() > connectionTimeout) {
      Serial.println("Connection to MQTT broker failed. Going to deep sleep.");
      esp_deep_sleep_start();
    }
    Serial.println("Connecting to MQTT...");
    if (WiFi.status() == WL_CONNECTED) {
      mqttClient.connect(DEVICE_NAME, MQTT_USER, MQTT_PASS);
    }
    delay(1000);
  }

  if (bme.begin(0x76)) {
    Serial.println("BME280 sensor found.");
  } else {
    Serial.println("BME280 sensor not found. Going to deep sleep.");
    esp_deep_sleep_start();
  }


  Serial.println("Sending data to MQTT broker...");
  mqttClient.publish("home/" DEVICE_NAME "/temp", String(bme.readTemperature()));
  mqttClient.publish("home/" DEVICE_NAME "/pressure", String(bme.readPressure() / 100.0F));
  mqttClient.publish("home/" DEVICE_NAME "/altitude", String(bme.readAltitude(SEALEVELPRESSURE_HPA)));
  mqttClient.publish("home/" DEVICE_NAME "/humidity", String(bme.readHumidity()));
  mqttClient.publish("home/" DEVICE_NAME "/wifi_signal", String(WiFi.RSSI()));
  mqttClient.publish("home/" DEVICE_NAME "/battery", String(analogRead(BATTERY_VOLTAGE_PIN) * 3.3 / 4095));

  for (int i = 0; i < 10; i++) {
    mqttClient.loop();
    delay(100);
  }

  esp_deep_sleep_start();
}

void loop() {
    // This should never be reached
    esp_deep_sleep_start();
}
