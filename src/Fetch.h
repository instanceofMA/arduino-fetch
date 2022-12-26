// #include "utils/FormData.h"
// #include "utils/URLEncoded.h"
// #include "utils/JSON.h"
#include "utils/Url/Url.h"
#include "utils/Headers/Headers.h"
#include <Arduino.h>

#ifndef FETCH_H_
#define FETCH_H_

#define HTTP 0
#define HTTPS 1

class Body {
    private:
        String _text;
    public:
        Body();
        String operator+(String str);
        String operator=(String str);

        String text();
};

String operator+(String str, Body body);

// class Request {
//     private:
//         const char* _host;
//         const char* _path;
//         const char* _query;
//         int _port;
//         const char* _method;
//         bool _isSsl;
//         const char* _fingerprint;
//         Headers _headers;
//         Body _body;
//     public:
//         Request();
// };

class ResponseHeaders {
    private:
        String _text;
    public:
        ResponseHeaders();
        String get(String headerName);

        String& text();

        void operator+=(const String& s);
        String operator[](const String& headerName);
};

class Response: public Printable {
    private:
    public:
        bool ok;
        int status;
        String statusText;
        bool redirected;
        String type;
        ResponseHeaders headers;
        String body;

        Response();
        String text();

        size_t printTo(Print& p) const;
};

class RequestOptions {
    private:
    public:
        String method;
        Headers headers;
        Body body;
        #if defined(ESP8266)
        String fingerprint;
        #endif
        String caCert;
        RequestOptions();
};

typedef void (*OnResponseCallback)(Response response);

class FetchClient {
    private:
        short _protocol;
        WiFiClient _httpClient;
        WiFiClientSecure _httpsClient;
        OnResponseCallback _OnResponseCallback;
    public:
        FetchClient();
        FetchClient(WiFiClient client, OnResponseCallback callback);
        FetchClient(WiFiClientSecure client, OnResponseCallback callback);
        void loop();
};

Response fetch(const char* url, RequestOptions options);
FetchClient fetch(const char* url, RequestOptions options, OnResponseCallback onResponseCallback);

#endif