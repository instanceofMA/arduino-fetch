#include "recipes/WiFi.h"
#include "Fetch.h"

#define SSID YourWiFiSSID
#define PASSPHRASE YourWiFiPassphrase
// Fingerprint will expire and might need to be updated before running this sketch.
#define FINGERPRINT "FE 48 BD 27 57 4A 79 B5 12 07 31 97 05 25 DC 4A A0 AF 32 CA"

// Fetch when used in async mode returns a client that listens for the response.
FetchClient client;

// Function that handles the response when it's received.
void handleResponse(Response response) {
    Serial.println("Response received:");
    // Printing response.
    Serial.println(response);
    // Printing respons headers.
    Serial.printf("Connection Header: \"%s\"\n", response.headers["Content-Type"].c_str());
    Serial.printf("Connection Header: \"%s\"\n", response.headers["Connection"].c_str());
}

void setup() {
    Serial.begin(9600);
    connectWiFi(SSID, PASSPHRASE);

    RequestOptions options;
    options.method = "POST";
    options.headers["Content-Type"] = "application/json";
    options.body = "{\"email\": \"test@test.com\", \"password\": \"test:80\"}";
    options.fingerprint = FINGERPRINT;
    
    Serial.println("Sending the request...");

    // Fetch sends the response, when received, to our handleResponse function, instead of blocking the rest of the code until the response is received.
    client = fetch("https://api.grandeur.tech/auth/login/?apiKey=grandeurkywxmoy914080rxf9dh05n7e", options, handleResponse);

    Serial.println("Request Sent!");
    Serial.println("Waiting for the response...");
}

void loop() {
    // This is crucial, it listens on the connection for the response, until it's received.
    client.loop();
}