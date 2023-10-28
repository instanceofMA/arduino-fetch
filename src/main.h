#include <Arduino.h>
#include "Fetch.h"
#include "secrets.h"

#define LED 2

WiFiServer server(80);

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
const char *testDataUrl = TEST_DATA_URL;