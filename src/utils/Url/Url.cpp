#include "Url.h"

Url::Url() {}

Url parseUrl(const char* url) {
    // Parsing URL.
    LCBUrl lcbUrl;
    lcbUrl.setUrl(url);
    Url parsed;
    parsed.scheme = lcbUrl.getScheme();
    parsed.username = lcbUrl.getUserName();
    parsed.password = lcbUrl.getPassword();
    parsed.host = lcbUrl.getHost();
    parsed.path = "/" + lcbUrl.getPath();
    parsed.afterPath = lcbUrl.getAfterPath();
    parsed.query = lcbUrl.getQuery();
    parsed.fragment = lcbUrl.getFragment();
    parsed.port = lcbUrl.getPort();

    return parsed;
}