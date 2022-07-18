#include "recipes/WiFi.h"
#include "Fetch.h"
 
#define SSID YourWiFiSSID
#define PASSPHRASE YourWiFiPassphrase
// Fingerprint will expire and might need to be updated before running this sketch.
#define FINGERPRINT "DC 78 3C 09 3A 78 E3 A0 BA A9 C5 4F 7A A0 87 6F 89 01 71 4C"

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
    Serial.printf("Connection Header: \"%s\"\n", response.headers["Content-Type"].c_str());
    Serial.printf("Connection Header: \"%s\"\n", response.headers["Connection"].c_str());
}

void loop() {

}