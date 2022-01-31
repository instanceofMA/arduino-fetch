#include "recipes/WiFi.h"
#include "Fetch.h"
 
#define SSID "MA"
#define PASSPHRASE "nopassword"
#define FINGERPRINT "96 84 07 DF 0B 1C F6 58 14 DF D7 33 35 57 51 9B 15 4D 8C E7"


void setup() {
    Serial.begin(9600);
    connectWiFi(SSID, PASSPHRASE);

    RequestOptions options;
    options.method = "GET";
    options.fingerprint = FINGERPRINT;
    
    Response response = fetch("https://api.github.com/", options);

    Serial.println();
    Serial.println(response.text());
}

void loop() {

}