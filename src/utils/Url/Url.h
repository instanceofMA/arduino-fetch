#include <LCBUrl.h>

#ifndef URL_H_
#define URL_H_

class Url {
    private:
    public:
        String scheme;
        String username;
        String password;
        String host;
        String path;
        String afterPath;
        String query;
        String fragment;
        uint port;
        Url();
};

Url parseUrl(const char* url);

#endif