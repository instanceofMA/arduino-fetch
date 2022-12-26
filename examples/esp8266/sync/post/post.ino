#include "recipes/WiFi.h"
#include "Fetch.h"

#define SSID YourWiFiSSID
#define PASSPHRASE YourWiFiPassphrase
// Fingerprint will expire and might need to be updated before running this sketch.
#define FINGERPRINT "5A B8 55 55 6B 64 F3 5A CA 71 21 9F 5A BC 16 22 1A B7 5A 1E"

void setup() {
    Serial.begin(9600);
    connectWiFi(SSID, PASSPHRASE);

    RequestOptions options;
    options.method = "POST";
    options.headers["Content-Type"] = "application/json";
    options.body = "{\"email\": \"test@test.com\", \"password\": \"test:80\"}";
    options.fingerprint = FINGERPRINT;
    
    Response response = fetch("https://api.grandeur.tech/auth/login/?apiKey=grandeurkywxmoy914080rxf9dh05n7e", options);
    
    // Printing response.
    Serial.println(response);
    // Printing respons headers.
    if(response.ok) {
        Serial.printf("Connection Header: \"%s\"\n", response.headers["Content-Type"].c_str());
        Serial.printf("Connection Header: \"%s\"\n", response.headers["Connection"].c_str());
    }
}

void loop() {

}