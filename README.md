# ESP32 MQTT Environmental Sensor  

This project is an advanced implementation for an ESP32-based environmental sensor. It integrates multiple sensors to collect environmental data, publishes it to an MQTT broker, and enters deep sleep to conserve power.  

## Features  

- Reads data from multiple sensors:  
  - **BME280**: Temperature, pressure, altitude, and humidity.  
  - **ENS160**: Air quality metrics (TVOC and eCO2).  
  - **AHT20**: Additional temperature and humidity readings for precise environmental adjustments.  
- Measures Wi-Fi signal strength and battery voltage.  
- Publishes data to an MQTT broker.  
- Uses deep sleep mode to minimize power consumption.  
- Configurable via the Secrets.h file.  

## Hardware Requirements  

- **ESP32**  
- **Adafruit BME280 sensor** (connected via I2C, default address 0x76)  
- **Adafruit AHT20 sensor** (connected via I2C)  
- **ScioSense ENS160 air quality sensor** (connected via I2C, default address 0x53)  
- **Voltage divider circuit** for battery voltage measurement (optional)  

## Software Requirements  

- **Arduino IDE** with the following libraries:  
  - Adafruit BME280  
  - Adafruit AHTX0  
  - ScioSense ENS160  
  - WiFi  
  - MQTT  

## Configuration  

1. Create a Secrets.h file in the project folder with the following content:  

```cpp  
#define WIFI_SSID "your_wifi_ssid"  
#define WIFI_PASS "your_wifi_password"  
#define MQTT_BROKER "mqtt_broker_address"  
#define MQTT_PORT 1883  
#define MQTT_USER "mqtt_username"  
#define MQTT_PASS "mqtt_password"  
#define DEVICE_NAME "your_device_name"
```

2. Update CONNECTION_TIMEOUT and DATA_COLLECTION_INTERVAL in the code as needed:  
   - CONNECTION_TIMEOUT: Maximum time (in milliseconds) to establish a connection to the MQTT broker.  
   - DATA_COLLECTION_INTERVAL: Time (in microseconds) between data collections.  

3. Connect the hardware:  
   - Wire the BME280, AHT20, and ENS160 sensors to the ESP32 via I2C.  
   - Connect the battery voltage measurement pin to A0.  

## Usage  

1. Flash the code onto your ESP32 using the Arduino IDE.  
2. The ESP32 will:  
   - Connect to the specified Wi-Fi network.  
   - Connect to the MQTT broker.  
   - Collect data from the sensors.  
   - Publish data to the MQTT topics:  
     - home/<DEVICE_NAME>/temp: Temperature from BME280 in Celsius.  
     - home/<DEVICE_NAME>/pressure: Pressure in hPa.  
     - home/<DEVICE_NAME>/altitude: Approximate altitude in meters.  
     - home/<DEVICE_NAME>/humidity: Humidity percentage from BME280.  
     - home/<DEVICE_NAME>/wifi_signal: Wi-Fi RSSI (signal strength).  
     - home/<DEVICE_NAME>/battery: Battery voltage (in volts).  
     - home/<DEVICE_NAME>/tvoc: Total Volatile Organic Compounds (TVOC) from ENS160.  
     - home/<DEVICE_NAME>/eco2: Equivalent CO2 concentration from ENS160.  
   - Enter deep sleep until the next collection cycle.  

## Power Optimization  

The code leverages the ESP32's deep sleep mode to save energy:  
- The device wakes up after the interval specified in DATA_COLLECTION_INTERVAL.  
- After publishing data, it immediately re-enters deep sleep.  

## License  

This project is open-source and can be used or modified under the terms of the MIT License.  

Feel free to contribute or report issues!  
