#include "recipes/WiFi.h"
#include "ArduinoFetch.h"
 
#define SSID YourWiFiSSID
#define PASSPHRASE YourWiFiPassphrase
#define FINGERPRINT "DC 78 3C 09 3A 78 E3 A0 BA A9 C5 4F 7A A0 87 6F 89 01 71 4C"

void setup() {
    Serial.begin(9200);
    connectWiFi(SSID, PASSPHRASE);

    RequestOptions options;
    options.method = "POST";
    options.headers.contentType = "application/json";
    options.body = "{\"email\": \"test@test.com\", \"password\": \"test:80\"}";
    options.fingerprint = FINGERPRINT;
    
    Response response = fetch("https://api.grandeur.tech/auth/login/?apiKey=grandeurkywxmoy914080rxf9dh05n7e", options);
    
    Serial.println(response);
    Serial.printf("Connection Header: \"%s\"\n", response.headers.get("Content-Type").c_str());
    Serial.printf("Connection Header: \"%s\"\n", response.headers.get("Connection").c_str());
}

void loop() {

}