#include "recipes/WiFi.h"
#include "ArduinoFetch.h"
 
#define SSID WIFISSID
#define PASSPHRASE WIFIPASSPHRASE

void setup() {
    Serial.begin(9200);
    connectWiFi(SSID, PASSPHRASE);

    RequestOptions options;
    options.method = "GET";
    
    Response response = fetch("https://api.grandeur.tech/", options);

    Serial.println(response.text());
}

void loop() {

}