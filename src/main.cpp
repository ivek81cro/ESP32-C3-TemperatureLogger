#include "ESP32TempLogger.h"

ESP32TempLogger logger; // Create an instance of the logger

void setup() {
    logger.setup(); // Call the setup method of the logger
}

void loop() {
    logger.loop(); // Call the loop method of the logger
    //sender.sendDataViaESPNOW("Hello from ESP32!"); // Send data via ESP-NOW
}