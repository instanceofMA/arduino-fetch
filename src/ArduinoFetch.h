#include "utils/FormData.h"
#include "utils/URLEncoded.h"
#include "utils/JSON.h"
#include <Arduino.h>

class Headers {
    private:
    public:
        String contentType;
        unsigned long contentLength;
        String host;
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

class Response {
    private:
    String _text;
    public:
    Response();
    Response(String body);
    String text();
};

class RequestOptions {
    private:
    public:
        String method;
        Headers headers;
        Body body;
        RequestOptions();
};

// class ArduinoFetch {
//     private:
//         const char* _url;
//         RequestOptions _options;
//     public:
//         ArduinoFetch();
//         ArduinoFetch(const char* url, RequestOptions options);
// };

Response fetch(const char* url, RequestOptions options);