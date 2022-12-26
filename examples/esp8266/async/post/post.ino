#include "recipes/WiFi.h"
#include "Fetch.h"

#define SSID YourWiFiSSID
#define PASSPHRASE YourWiFiPassphrase
// Fingerprint will expire and might need to be updated before running this sketch.
#define FINGERPRINT "5A B8 55 55 6B 64 F3 5A CA 71 21 9F 5A BC 16 22 1A B7 5A 1E"

// Fetch when used in async mode returns a client that listens for the response.
FetchClient client;

// Function that handles the response when it's received.
void handleResponse(Response response) {
    Serial.println("Response received:");
    // Printing response.
    Serial.println(response);
    // Printing response headers.
    Serial.printf("Content-Type Header: \"%s\"\n", response.headers["Content-Type"].c_str());
    Serial.printf("Connection Header: \"%s\"\n", response.headers["Connection"].c_str());
}

void setup() {
    Serial.begin(9600);
    connectWiFi(SSID, PASSPHRASE);

    RequestOptions options;
    options.method = "POST";
    options.headers["Content-Type"] = "application/json";
    options.headers["Authorization"] = "c051d62800089fe87e35b820c9e36354f24695513aa4a8b2e49e8cd7cca7b6a8";
    options.fingerprint = FINGERPRINT;
    
    Serial.println("Sending the request...");

    // Fetch sends the response, when received, to our handleResponse function, instead of blocking the rest of the code until the response is received.
    client = fetch("https://api.grandeur.tech/devices/get/?apiKey=grandeurlc4z67ve03ec0jfl01gk15xe", options, handleResponse);

    Serial.println("Request Sent!");
    Serial.println("Waiting for the response...");
}

void loop() {
    // This is crucial, it listens on the connection for the response, until it's received.
    client.loop();
}