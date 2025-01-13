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
#include <esp_now.h>

// Structure to send data
typedef struct espNow_message {
    char dateAndTime[25];
    float temperature;    
} espNow_message;

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
    bool readDataFromSD(String &data);
    void prepareESPNOW();
    void ESP32TempLogger::logDataToSD(const String &dataString);
    void sendDataViaESPNOW(const char* date, float temperature);
    static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    static void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);

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
    
    esp_now_peer_info_t peerInfo;

};

#endif // ESP32_TEMP_LOGGER_H
