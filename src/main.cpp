#include "ESP32TempLogger.h"
#include "firebase_handler.h"

ESP32TempLogger logger;
FirebaseHandler fb_handler;

void setup() {
    fb_handler.begin();
    logger.setup(); // Call the setup method of the logger
}

void loop() {
    logger.loop(); // Call the loop method of the logger
}