#include "Headers.h"

Headers::Headers() {
    _headers["User-Agent"] = "arduino-fetch";
    _headers["Content-Type"] = "application/x-www-form-urlencoded";
    _headers["accept"] = "*/*";
    _headers["Connection"] = "close";
}

String Headers::text() const {
    String str = "";

    for(std::map<String, String>::const_iterator itr = _headers.begin(); itr != _headers.end(); ++itr)
        str += itr->first + ": " + itr->second + "\r\n";

    return str += "\r\n";
}

size_t Headers::printTo(Print& p) const {
    size_t r = 0;

    r += p.printf("%s", text().c_str());

    return r;
}

String& Headers::operator[](const String& headerName) {
    return _headers[headerName];
}