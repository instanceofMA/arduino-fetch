// #include "utils/FormData.h"
// #include "utils/URLEncoded.h"
// #include "utils/JSON.h"
#include "utils/Url/Url.h"
#include <Arduino.h>

#ifndef FETCH_H_
#define FETCH_H_

class Headers {
    private:
    public:
        String contentType;
        unsigned long contentLength;
        String userAgent;
        String cookie;
        String accept;
        String connection;
        String transferEncoding;
        Headers();
};

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
    public:
        String text;

        ResponseHeaders();
        String get(String headerName);
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

// class Fetch {
//     private:
//         const char* _url;
//         RequestOptions _options;
//     public:
//         Fetch();
//         Fetch(const char* url, RequestOptions options);
// };

Response fetch(const char* url, RequestOptions options);

#endif