#include "Fetch.h"
#include <WiFiClientSecure.h>

// Fetch fetch;

Response fetch(const char* url, RequestOptions options) {
    // Parsing URL.
    Url parsedUrl = parseUrl(url);
    
    WiFiClientSecure client;
    // Retry every 15 seconds.
    client.setTimeout(15000);

    // Set fingerprint if https.
    if(parsedUrl.scheme == "https") {
        if(options.fingerprint == "") Serial.println("[Error] Provide fingerprint for HTTPS.");
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
    Response response;
    for(int nLine = 1; client.connected(); nLine++) {
        // Reading headers line by line.
        String line = client.readStringUntil('\n');
        // Parse status and statusText from line 1.
        if(nLine == 1) {
            response.status = line.substring(line.indexOf(" ")).substring(0, line.indexOf(" ")).toInt();
            response.statusText = line.substring(line.indexOf(String(response.status)) + 4);
            response.statusText.trim();
            continue;
        }

        response.headers.text += line + "\n";
        // If headers end, move on.
        if(line == "\r") break;
    }

    // Serial.println("-----HEADERS START-----");
    // Serial.println(response.headers.text);
    // Serial.println("-----HEADERS END-----");

    // Getting response body.
    while(client.available()) {
        response.body += client.readStringUntil('\n');
    }

    return response;
}

Headers::Headers(): contentType("application/x-www-form-urlencoded"),
    contentLength(-1), userAgent("arduino-fetch"), cookie(""),
    accept("*/*"), connection("close"), transferEncoding("") {}

ResponseHeaders::ResponseHeaders(): text("") {}

String ResponseHeaders::get(String headerName) {
    String tillEnd = this->text.substring(this->text.lastIndexOf(headerName + ": "));
    return tillEnd.substring(tillEnd.indexOf(" ") + 1, tillEnd.indexOf("\n") - 1);
}

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

// Fetch::Fetch(const char* url, RequestOptions options) :
//     _url(url), _options(options) {}

Response::Response(): ok(false), status(200), statusText("OK"),
    redirected(false), type(""), headers(ResponseHeaders()), body("") {}

String Response::text() {
    return this->body;
}

size_t Response::printTo(Print& p) const {
    size_t r = 0;

    r += p.printf(
        (String("{") +
            "\n\t\"ok\": %d" +
            "\n\t\"status\": %d" +
            "\n\t\"statusText\": \"%s\"" +
            "\n\t\"body\": \"%s\"" +
        "\n}").c_str(),
        ok, status, statusText.c_str(), body.c_str());

    return r;
}

RequestOptions::RequestOptions(): method("GET"), headers(Headers()), body(Body()), fingerprint("") {}
