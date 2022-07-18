#include "recipes/WiFi.h"
#include "Fetch.h"

#define SSID YourWiFiSSID
#define PASSPHRASE YourWiFiPassphrase
// Fingerprint will expire and might need to be updated before running this sketch.
#define FINGERPRINT "29 70 30 74 CA 3C 48 F5 4A 79 C6 2D 11 57 A2 41 2A 2D 7D 5C"

// Fetch when used in async mode returns a client that listens for the response.
FetchClient client;

// Function that handles the response when it's received.
void handleResponse(Response response) {
    Serial.println("Response received:");
    // Printing response body as plain text.
    Serial.println();
    Serial.println(response.text());
}

void setup() {
    Serial.begin(9600);
    connectWiFi(SSID, PASSPHRASE);

    RequestOptions options;
    options.method = "GET";
    options.fingerprint = FINGERPRINT;
    
    Serial.println("Sending the request...");

    // Fetch sends the response, when received, to our handleResponse function, instead of blocking the rest of the code until the response is received.
    client = fetch("https://api.github.com/", options, handleResponse);

    Serial.println("Request Sent!");
    Serial.println("Waiting for the response...");
}

void loop() {
    // This is crucial, it listens on the connection for the response, until it's received.
    client.loop();
}