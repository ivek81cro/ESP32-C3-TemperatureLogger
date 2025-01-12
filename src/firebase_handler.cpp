#include "firebase_handler.h"
#include "WiFiCredentials.h"
#include <Arduino.h>

// Constructor: Initializes Firebase configuration
FirebaseHandler::FirebaseHandler() {
    config.database_url = FIREBASE_HOST;                 // Set Firebase database URL
    config.signer.tokens.legacy_token = FIREBASE_AUTH;   // Set authentication token
}

// Begin Firebase connection
void FirebaseHandler::begin() {
    Firebase.begin(&config, &auth);         // Initialize Firebase
    Firebase.reconnectWiFi(true);           // Enable automatic Wi-Fi reconnection
    Serial.println("Firebase initialized.");
}

// Upload temperature logs to Firebase
bool FirebaseHandler::uploadTemperatureLogs(SdFat& sd, const char* filePath) {
    // Ensure the file exists on the SD card
    if (!sd.exists(filePath)) {
        Serial.println("Temperature log file does not exist.");
        return false;
    }

    // Open the file for reading
    File32 file = sd.open(filePath, O_RDONLY);
    if (!file) {
        Serial.println("Failed to open temperature log file for reading.");
        return false;
    }

    // Read the file content into a String
    String dataPayload = "";
    while (file.available()) {
        dataPayload += (char)file.read();
    }
    file.close();

    // Upload the data to Firebase
    if (Firebase.RTDB.setString(&fbdo, "/temperature_logs", dataPayload)) {
        Serial.println("Temperature logs uploaded to Firebase successfully.");
        sd.remove(filePath);  // Remove the file after successful upload
        Serial.println("Log file deleted from SD card.");
        return true;
    } else {
        Serial.print("Failed to upload logs to Firebase: ");
        Serial.println(fbdo.errorReason());
        return false;
    }
}