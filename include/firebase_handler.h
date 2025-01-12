#ifndef FIREBASE_HANDLER_H
#define FIREBASE_HANDLER_H

#include <Firebase_ESP_Client.h>
#include <SdFat.h>

class FirebaseHandler {
private:
    FirebaseData fbdo;       // Firebase data object
    FirebaseAuth auth;       // Firebase authentication
    FirebaseConfig config;   // Firebase configuration

public:
    FirebaseHandler();       // Constructor
    void begin();            // Initialize Firebase connection
    bool uploadTemperatureLogs(SdFat& sd, const char* filePath);  // Upload logs to Firebase
};

#endif // FIREBASE_HANDLER_H
