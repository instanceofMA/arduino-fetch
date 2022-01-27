#include "ArduinoFetch.h"
#include <WiFiClientSecure.h>
#include <LCBUrl.h>

// ArduinoFetch arduinoFetch;

const char fingerprint[] PROGMEM = "DC 78 3C 09 3A 78 E3 A0 BA A9 C5 4F 7A A0 87 6F 89 01 71 4C";

Response fetch(const char* url, RequestOptions options) {
    // Parsing URL.
    LCBUrl lcbUrl;
    lcbUrl.setUrl(url);
    String scheme = lcbUrl.getScheme();
    String host = lcbUrl.getHost();
    String path = "/" + lcbUrl.getPath();
    String afterPath = lcbUrl.getAfterPath();
    unsigned int port = lcbUrl.getPort();

    Serial.printf("scheme: %s, host: %s, path: %s, pathSegment: %s, port: %u\n", scheme.c_str(), host.c_str(), path.c_str(), afterPath.c_str(), port);
    
    WiFiClientSecure client;
    // Retry every 15 seconds.
    client.setTimeout(15000);
    // Set fingerprint if https.
    if(scheme == "https") client.setFingerprint(fingerprint);

    delay(1000);

    // Connecting to server.
    while(!client.connect(host, port)) {
        delay(1000);
        Serial.print(".");
    }

    Serial.println("Request is: ");
    Serial.println(
        options.method + " " + path + afterPath + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" +
        "Content-Type: " + options.headers.contentType + "\r\n" +
        "Connection: close\r\n\r\n" +
        options.body + "\r\n\r\n"
    );

    // Sending request.
    client.print(
        options.method + " " + path + afterPath + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" +
        "Content-Type: " + options.headers.contentType + "\r\n" +
        "Connection: close\r\n\r\n" +
        options.body + "\r\n\r\n"
    );

    // Getting response headers.
    String headers = "";
    while(client.connected()) {
        String line = client.readStringUntil('\n');
        headers += line;
        if(line == "\r") break;
    }

    Serial.println("-----HEADERS START-----");
    Serial.println(headers);
    Serial.println("-----HEADERS END-----");

    // Getting response body.
    String body = "";
    while(client.available()) {
        body += client.readStringUntil('\n');
        body += "\n";
    }

    return Response(body);
}

Headers::Headers(): contentType("application/x-www-form-urlencoded"),
    contentLength(-1), host("\0"), userAgent("arduino-fetch"), cookie("\0"),
    accept("*/*"), connection("close"), transferEncoding("\0") {}

Body::Body(): _text("") {}

String Body::text() {
    return this->_text;
}

String Body::operator+(String str) {
    return this->_text + str;
}

String Body::operator=(String str) {
    return this->_text = str;
}

String operator+(String str, Body body) {
    return str + body.text();
}

// ArduinoFetch::ArduinoFetch(const char* url, RequestOptions options) :
//     _url(url), _options(options) {}

Response::Response(String body): _text(body) {}

String Response::text() {
    return this->_text;
}

RequestOptions::RequestOptions(): method("GET"), headers(Headers()), body(Body()) {}

