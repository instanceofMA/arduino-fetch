#include "Fetch.h"
#include "debug.h"
#include <WiFiClientSecure.h>

void setupHTTPS(WiFiClientSecure& client, RequestOptions options) {
    // Set Fingerprint or Certificate for https.
    #ifdef ESP8266
    if(options.fingerprint == "" && options.caCert == "") {
        DEBUG_FETCH("[INFO] No fingerprint or caCert is provided. Using the INSECURE mode for connection!");
        client.setInsecure();
    }
    else if(options.caCert != "") client.setTrustAnchors(new X509List(options.caCert.c_str()));
    else client.setFingerprint(options.fingerprint.c_str());
    #elif defined(ESP32)
    if(options.caCert == "") {
        DEBUG_FETCH("[INFO] No CA Cert is provided. Using the INSECURE mode for connection!");
        client.setInsecure();
    }
    else client.setCACert(options.caCert.c_str());
    #endif
}

bool connectToServer(WiFiClient& client, const char* host, unsigned int port) {
    // Connecting to server.
    bool connectionSuccess = client.connect(host, port);
    DEBUG_FETCH("Connection Success is: %d.", connectionSuccess);

    return connectionSuccess;
}

bool connectToServer(WiFiClientSecure& client, const char* host, unsigned int port) {
    // Connecting to server.
    bool connectionSuccess = client.connect(host, port);
    DEBUG_FETCH("Connection Success is: %d.", connectionSuccess);

    // If there are errors, print and return.
    DEBUG_FETCH("Connection Error is: %d.", client.getLastSSLError());
    if(client.getLastSSLError()) {
        char error[256];
        client.getLastSSLError(error, 256);
        Serial.println(error);
        return false;
    }

    return true;
}

void sendRequest(WiFiClient& client, Url& url, RequestOptions& options) {
    // Forming request.
    String request =
        options.method + " " + url.path + url.afterPath + " HTTP/1.1\r\n" +
        "Host: " + url.host + "\r\n" +
        options.headers.text() +
        options.body + "\r\n\r\n";

    DEBUG_FETCH("-----REQUEST START-----\n%s\n-----REQUEST END-----", request.c_str());

    // Sending request.
    client.print(request);
}

void sendRequest(WiFiClientSecure& client, Url& url, RequestOptions& options) {
    // Forming request.
    String request =
        options.method + " " + url.path + url.afterPath + " HTTP/1.1\r\n" +
        "Host: " + url.host + "\r\n" +
        options.headers.text() +
        options.body + "\r\n\r\n";

    DEBUG_FETCH("-----REQUEST START-----\n%s\n-----REQUEST END-----", request.c_str());

    // Sending request.
    client.print(request);
}

Response receiveResponse(WiFiClient& client) {
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
            // Sets the "ok" field if request was successful.
            if(response.status >= 200 && response.status < 300) response.ok = true;
            continue;
        }

        response.headers += line + "\n";
        // If headers end, move on.
        if(line == "\r") break;
    }

    DEBUG_FETCH("-----HEADERS START-----\n%s\n-----HEADERS END-----", response.headers.text().c_str());

    // Getting response body.
    while(client.available()) {
        response.body += client.readStringUntil('\n');
    }

    return response;
}

Response receiveResponse(WiFiClientSecure& client) {
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
            // Sets the "ok" field if request was successful.
            if(response.status >= 200 && response.status < 300) response.ok = true;
            continue;
        }

        response.headers += line + "\n";
        // If headers end, move on.
        if(line == "\r") break;
    }

    DEBUG_FETCH("-----HEADERS START-----\n%s\n-----HEADERS END-----", response.headers.text().c_str());

    // Getting response body.
    while(client.available()) {
        response.body += client.readStringUntil('\n');
    }

    return response;
}

WiFiClient makeHTTPRequest(Url& url, RequestOptions& options) {
    WiFiClient client;
    // Retry every 15 seconds.
    client.setTimeout(15000);

    if(connectToServer(client, url.host.c_str(), url.port))
        sendRequest(client, url, options);

    return client;
}

WiFiClientSecure makeHTTPSRequest(Url& url, RequestOptions& options) {
    WiFiClientSecure client;
    // Retry every 15 seconds.
    client.setTimeout(15000);

    setupHTTPS(client, options);

    if(connectToServer(client, url.host.c_str(), url.port))
        sendRequest(client, url, options);

    return client;
}

Response fetch(const char* url, RequestOptions options) {
    // Parsing URL.
    Url parsedUrl = parseUrl(url);

    Response response;

    if(parsedUrl.scheme == "http") {
        WiFiClient client;
        // Retry every 15 seconds.
        client.setTimeout(15000);

        if(connectToServer(client, parsedUrl.host.c_str(), parsedUrl.port))
            sendRequest(client, parsedUrl, options);

        response = receiveResponse(client);
        // Stopping the client.
        client.stop();
    }
    else if(parsedUrl.scheme == "https") {
        WiFiClientSecure client;
        // Retry every 15 seconds.
        client.setTimeout(15000);

        setupHTTPS(client, options);

        if(connectToServer(client, parsedUrl.host.c_str(), parsedUrl.port))
            sendRequest(client, parsedUrl, options);

        response = receiveResponse(client);
        // Stopping the client.
        client.stop();
    }
    else DEBUG_FETCH("[ERROR] Protocol is not supported. Please only use HTTP or HTTPS.");

    return response;
}

FetchClient fetch(const char* url, RequestOptions options, OnResponseCallback onResponseCallback) {
    // Parsing URL.
    Url parsedUrl = parseUrl(url);

    if(parsedUrl.scheme == "http") {
        WiFiClient client;
        // Retry every 15 seconds.
        client.setTimeout(15000);

        if(connectToServer(client, parsedUrl.host.c_str(), parsedUrl.port))
            sendRequest(client, parsedUrl, options);

        return FetchClient(client, onResponseCallback);
    }
    else if(parsedUrl.scheme == "https") {
        WiFiClientSecure client;
        // Retry every 15 seconds.
        client.setTimeout(15000);

        setupHTTPS(client, options);

        if(connectToServer(client, parsedUrl.host.c_str(), parsedUrl.port))
            sendRequest(client, parsedUrl, options);

        return FetchClient(client, onResponseCallback);
    }
    
    DEBUG_FETCH("[ERROR] Protocol is not supported. Please only use HTTP or HTTPS.");
    return FetchClient();
}

FetchClient::FetchClient() :
_protocol(HTTP), _httpClient(), _httpsClient(), _OnResponseCallback(NULL) {}

FetchClient::FetchClient(WiFiClient client, OnResponseCallback onResponseCallback) :
_protocol(HTTP), _httpClient(client), _httpsClient(), _OnResponseCallback(onResponseCallback) {}

FetchClient::FetchClient(WiFiClientSecure client, OnResponseCallback onResponseCallback) :
_protocol(HTTPS), _httpClient(), _httpsClient(client), _OnResponseCallback(onResponseCallback) {}

void FetchClient::loop() {
    if(_protocol == HTTP && _httpClient.available()) {
        DEBUG_FETCH("[Info] Receiving response.");
        Response response = receiveResponse(_httpClient);

        // Stopping the client.
        _httpClient.stop();

        _OnResponseCallback(response);
    }
    else if(_protocol == HTTPS && _httpsClient.available()) {
        DEBUG_FETCH("[Info] Receiving response.");
        Response response = receiveResponse(_httpsClient);

        // Stopping the client.
        _httpsClient.stop();

        _OnResponseCallback(response);
    }
}

ResponseHeaders::ResponseHeaders(): _text("") {}

String ResponseHeaders::get(String headerName) {
    String tillEnd = _text.substring(_text.lastIndexOf(headerName + ": "));
    return tillEnd.substring(tillEnd.indexOf(" ") + 1, tillEnd.indexOf("\n") - 1);
}

String& ResponseHeaders::text() {
    return _text;
}

void ResponseHeaders::operator+=(const String& s) {
    _text += s;
}

String ResponseHeaders::operator[](const String& headerName) {
    return get(headerName);
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

Response::Response(): ok(false), status(0), statusText(""),
    redirected(false), type(""), headers({}), body("") {}

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

RequestOptions::RequestOptions(): method("GET"), headers(Headers()), body(Body()),
#if defined(ESP8266)
fingerprint(""),
#endif
caCert("") {}