// File: ESP32TempLogger.h
#ifndef ESP32_TEMP_LOGGER_H
#define ESP32_TEMP_LOGGER_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <SdFat.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

class ESP32TempLogger {
public:
    ESP32TempLogger();
    void setup();
    void loop();

private:
    void connectToWiFi();
    void checkWiFiConnection();
    bool initializeSDCard();
    bool ensureSDMounted();
    String getFormattedDateTime();

    // Wi-Fi Credentials
    const char* ssid = "ALHN-FAF3";
    const char* password = "yAXBzA52vq";

    // Pin configuration
    static const int ONE_WIRE_BUS = 0;
    static const int SD_CS_PIN = 10;
    static const int SPI_MOSI_PIN = 6;
    static const int SPI_MISO_PIN = 5;
    static const int SPI_SCK_PIN = 4;

    // Components
    OneWire oneWire;
    DallasTemperature sensors;
    SdFat sd;
    File32 dataFile;

    // NTP Client setup
    WiFiUDP ntpUDP;
    NTPClient timeClient;
};

#endif // ESP32_TEMP_LOGGER_H