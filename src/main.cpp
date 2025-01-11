// File: ESP32TempLogger.cpp
#include "ESP32TempLogger.h"

ESP32TempLogger logger;

void setup() {
    logger.setup(); // Call the setup method of the logger
}

void loop() {
    logger.loop(); // Call the loop method of the logger
}

ESP32TempLogger::ESP32TempLogger()
    : oneWire(ONE_WIRE_BUS), sensors(&oneWire), timeClient(ntpUDP, "pool.ntp.org", 0, 60000) {}

void ESP32TempLogger::setup() {
    Serial.begin(115200);
    delay(2000);  // Allow Serial to stabilize
    Serial.println("Starting ESP32-C3...");

    // Connect to Wi-Fi
    connectToWiFi();

    // Initialize NTP client
    timeClient.begin();

    // Initialize DS18B20 sensor
    sensors.begin();
    Serial.println("DS18B20 Initialized.");

    // Initialize SD card with retries
    if (!initializeSDCard()) {
        Serial.println("SD Card initialization failed after retries.");
        while (1);
    }
}

void ESP32TempLogger::loop() {
    // Check Wi-Fi connection
    checkWiFiConnection();

    // Request temperature
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);

    // Format temperature reading with full timestamp
    String dataString = getFormattedDateTime() + String(", Temperature: ") + temperatureC + " °C\n";
    Serial.print("Logging: ");
    Serial.print(dataString);

    // Open file and append data
    dataFile = sd.open("temp_log.txt", O_RDWR | O_CREAT | O_AT_END);
    if (dataFile) {
        dataFile.print(dataString);
        dataFile.close();
        Serial.println("Data written to SD card.");
    } else {
        Serial.println("Error opening temp_log.txt");
    }

    // Enable deep sleep for 30 seconds
    Serial.println("Entering deep sleep for 30 seconds...");
    esp_sleep_enable_timer_wakeup(30 * 1000000ULL);
    esp_deep_sleep_start();
}

void ESP32TempLogger::connectToWiFi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Connected.");
}

void ESP32TempLogger::checkWiFiConnection() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi disconnected. Reconnecting...");
        connectToWiFi();
    }
}

bool ESP32TempLogger::initializeSDCard() {
    SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SD_CS_PIN);
    for (int i = 0; i < 5; i++) {
        if (sd.begin(SD_CS_PIN, SD_SCK_MHZ(8))) {  // 8 MHz for stability
            Serial.println("SD Card Initialized.");
            return true;
        }
        Serial.println("Retrying SD card initialization...");
        delay(1000);
    }
    return false;
}

bool ESP32TempLogger::ensureSDMounted() {
    if (!sd.card()->isBusy()) {
        Serial.println("SD Card not mounted. Attempting to remount...");
        return initializeSDCard();
    }
    return true;
}

String ESP32TempLogger::getFormattedDateTime() {
    timeClient.update();
    time_t rawTime = timeClient.getEpochTime();
    struct tm* timeInfo = localtime(&rawTime);
    char buffer[25];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    return String(buffer);
}
