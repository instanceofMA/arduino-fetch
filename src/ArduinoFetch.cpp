#include "ArduinoFetch.h"
#include <WiFiClientSecure.h>

// ArduinoFetch arduinoFetch;

Response fetch(const char* url, RequestOptions options) {
    // Parsing URL.
    Url parsedUrl = parseUrl(url);
    
    WiFiClientSecure client;
    // Retry every 15 seconds.
    client.setTimeout(15000);

    // Set fingerprint if https.
    if(parsedUrl.scheme == "https") {
        if(options.fingerprint == "\0") Serial.println("[Error] Provide fingerprint for HTTPS.");
        else client.setFingerprint(options.fingerprint.c_str());
    }

    // Connecting to server.
    while(!client.connect(parsedUrl.host, parsedUrl.port)) {
        delay(1000);
        // Serial.print(".");
    }

    // Forming request.
    String request =
        options.method + " " + parsedUrl.path + parsedUrl.afterPath + " HTTP/1.1\r\n" +
        "Host: " + parsedUrl.host + "\r\n" +
        "User-Agent: " + options.headers.userAgent + "\r\n" +
        "Content-Type: " + options.headers.contentType + "\r\n" +
        "Connection: " + options.headers.connection + "\r\n\r\n" +
        options.body + "\r\n\r\n";

    // Serial.println("Request is: ");
    // Serial.println(request);

    // Sending request.
    client.print(request);

    // Getting response headers.
    String headers = "";
    while(client.connected()) {
        String line = client.readStringUntil('\n');
        headers += line;
        if(line == "\r") break;
    }

    // Serial.println("-----HEADERS START-----");
    // Serial.println(headers);
    // Serial.println("-----HEADERS END-----");

    // Getting response body.
    String body = "";
    while(client.available()) {
        body += client.readStringUntil('\n');
        body += "\n";
    }

    return Response(body);
}

Headers::Headers(): contentType("application/x-www-form-urlencoded"),
    contentLength(-1), userAgent("arduino-fetch"), cookie("\0"),
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

RequestOptions::RequestOptions(): method("GET"), headers(Headers()), body(Body()), fingerprint("\0") {}

