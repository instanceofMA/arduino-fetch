#include <Arduino.h>
#include <map>

#ifndef HEADERS_H_
#define HEADERS_H_

class Headers : public Printable {
    private:
        std::map<String, String> _headers;
    public:
        Headers();

        String text() const;

        size_t printTo(Print& p) const;

        String& operator[](const String& headerName);
};

#endif