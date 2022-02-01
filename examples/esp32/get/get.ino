#include "recipes/WiFi.h"
#include "Fetch.h"
 
#define SSID YourWiFiSSID
#define PASSPHRASE YourWiFiPassphrase


void setup() {
    Serial.begin(9600);
    connectWiFi(SSID, PASSPHRASE);

    RequestOptions options;
    options.method = "GET";
    
    Response response = fetch("https://api.github.com/", options);
    
    // Printing response body as plain text.
    Serial.println();
    Serial.println(response.text());
}

void loop() {

}