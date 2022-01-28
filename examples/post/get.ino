#include "recipes/WiFi.h"
#include "ArduinoFetch.h"
 
#define SSID WIFISSID
#define PASSPHRASE WIFIPASSPHRASE

void setup() {
    Serial.begin(9200);
    connectWiFi(SSID, PASSPHRASE);

    RequestOptions options;
    options.method = "POST";
    options.headers.contentType = "application/json";
    options.body = "{\"email\": \"test@test.com\", \"password\": \"test:80\"}";
    
    Response response = fetch("https://api.grandeur.tech/auth/login/?apiKey=grandeurkywxmoy914080rxf9dh05n7e", options);

    Serial.println(response.text());
}

void loop() {

}