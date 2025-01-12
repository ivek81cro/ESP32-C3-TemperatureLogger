#include "ESP32TempLogger.h"

ESP32TempLogger logger;

void setup() {
    logger.setup(); // Call the setup method of the logger
}

void loop() {
    logger.loop(); // Call the loop method of the logger
}