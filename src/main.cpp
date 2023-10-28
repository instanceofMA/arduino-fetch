#include "main.h"

void setup()
{
    Serial.begin(9600);
    delay(10);

    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);

    // Connect to WiFi network
    DEBUG_PLN();
    DEBUG_PLN();
    DEBUG_P(F("Connecting to "));
    DEBUG_PLN(ssid);

    WiFi.begin(ssid, password);

    int counter = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        if (counter > 20)
        {
            break;
        }
        digitalWrite(LED, !digitalRead(LED));
        delay(500);
        Serial.println(F("."));
        counter++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        DEBUG_PLN();
        Serial.println(F("WiFi connected!"));

        // Start the server
        server.begin();
        DEBUG_PLN(F("Server started"));

        // Print the IP address
        DEBUG_P(F("Use this URL to connect: "));
        DEBUG_P(F("http://"));
        DEBUG_P(WiFi.localIP());
        DEBUG_PLN(F("/"));
    }
    else
    {
        Serial.println(F("WiFi failed to connect"));
    }

    DEBUG_PLN(F("Updating schedule..."));

    RequestOptions options;
    options.method = F("GET");
    options.headers[F("Connection")] = F("keep-alive");
    options.headers[F("accept")] = F("text/plain");

    DEBUG_PLN(F("Sending the request..."));

    Response response = fetch(testDataUrl, options);

    if (response.status != 200)
    {
        return;
    }

    String testData = response.text();

    DEBUG_PLN(testData);

    DEBUG_PLN(F("Finished"));
}

void loop()
{
}