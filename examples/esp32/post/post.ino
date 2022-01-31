#include "recipes/WiFi.h"
#include "Fetch.h"
 
#define SSID YourWiFiSSID
#define PASSPHRASE YourWiFiPassphrase

void setup() {
    Serial.begin(9600);
    connectWiFi(SSID, PASSPHRASE);

    RequestOptions options;
    options.method = "POST";
    options.headers.contentType = "application/json";
    options.body = "{\"email\": \"test@test.com\", \"password\": \"test:80\"}";
    
    Response response = fetch("https://api.grandeur.tech/auth/login/?apiKey=grandeurkywxmoy914080rxf9dh05n7e", options);
    
    Serial.println(response);
    Serial.printf("Connection Header: \"%s\"\n", response.headers.get("Content-Type").c_str());
    Serial.printf("Connection Header: \"%s\"\n", response.headers.get("Connection").c_str());
}

void loop() {

}