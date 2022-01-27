#include "ArduinoFetch.h"

void setup() {
    Serial.begin(9600);

    RequestOptions options;
    options.method = "GET";
    Response response = fetch("https://api.grandeur.tech/", options);

    Serial.println(response.text());
}

void loop() {

}