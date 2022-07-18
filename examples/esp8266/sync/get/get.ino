#include "recipes/WiFi.h"
#include "Fetch.h"
 
#define SSID YourWiFiSSID
#define PASSPHRASE YourWiFiPassphrase
// Fingerprint will expire and might need to be updated before running this sketch.
#define FINGERPRINT "29 70 30 74 CA 3C 48 F5 4A 79 C6 2D 11 57 A2 41 2A 2D 7D 5C"


void setup() {
    Serial.begin(9600);
    connectWiFi(SSID, PASSPHRASE);

    RequestOptions options;
    options.method = "GET";
    options.fingerprint = FINGERPRINT;
    
    Response response = fetch("https://api.github.com/", options);

    // Printing response body as plain text.
    Serial.println();
    Serial.println(response.text());
}

void loop() {

}