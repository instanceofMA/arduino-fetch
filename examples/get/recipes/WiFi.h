#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "blink.h"

// Makes the device connect to a WiFi router or hotspot.
void connectWiFi(const char* ssid, const char* passphrase) {
    // Disconnecting WiFi if it"s already connected.
    WiFi.disconnect();
    // Setting it to Station mode which basically scans for nearby WiFi routers or hotspots.
    WiFi.mode(WIFI_STA);
    // Begin connecting to WiFi using the provided ssid and passphrase.
    WiFi.begin(ssid, passphrase);
    // Print a debug log to Serial.
    Serial.printf("\nDevice is connecting to WiFi using SSID %s and Passphrase %s.\n", ssid, passphrase);
    // Keep looping until the WiFi is not connected.
    while (WiFi.status() != WL_CONNECTED) {
        // Print dots in a horizontal line to the Serial, showing the WiFi is trying to connect.
        Serial.print(".");
        // Blink LED very fast, showing the WiFi is trying to connect.
        blinkN(10);
    }
    // Stop the LED blinking, showing the WiFi is successfully connected.
    blinkStop();
    // Print debug logs to Serial.
    Serial.println("WiFi connected");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP()); // Local IP Address of the ESP8266 device.
}

#endif /* WIFI_H */