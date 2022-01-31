/*
    LCBUrl.h -  Library for handling URLs - This library will allow
                handling and manipulation of URLs according to RFC3986.
*/

/*
    MIT License

    Copyright (c) 2019-2021 Lee C. Bussy

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

// Ensure this library description is only included once

#ifndef _LCBURL_H
#define _LCBURL_H

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#endif
#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#endif

#include <stdio.h>
#include <string.h>
#include <Arduino.h>

// Library interface description
class LCBUrl
{
    // User-accessible "public" interface
public:
    LCBUrl(const String &newUrl = "");
    ~LCBUrl(){};
    bool setUrl(const String &newUrl);
    String getUrl();
    String getIPUrl();
    String getScheme();
    String getUserInfo();
    String getUserName();
    String getPassword();
    String getHost();
    unsigned int getPort();
    String getAuthority();
    String getIPAuthority();
    String getPath();
    String getAfterPath();
    String getQuery();
    String getFragment();

    // Utility functions
    bool isMDNS() __attribute__ ((deprecated));
    bool isMDNS(const char *hostName);
    IPAddress getIP() __attribute__ ((deprecated));
    IPAddress getIP(const char * hostName);
    bool isValidIP(const char * hostName);
    int labelCount(const char * hostName);
    bool isANumber(const char * str);
    bool isValidLabel(const char *label);
    bool isValidHostName(const char *hostName);

    // Library-accessible "private" interface
private:
    String getRawUrl();
    String getCleanTriplets();
    String getDotSegmentsClear();
    String getStripScheme();
    String getRawAuthority();
    String getAfterAuth();
    String getPathSegment();
    void initRegisters();
    String rawurl;
    String url;
    String ipurl;
    String workingurl;
    String scheme;
    String stripscheme;
    String rawauthority;
    String afterauth;
    String userinfo;
    String username;
    String password;
    String host;
    IPAddress ipaddress;
    unsigned int port;
    String authority;
    String ipauthority;
    String pathsegment;
    String path;
    String afterpath;
    String query;
    String fragment;
};

#endif // _LCBURL_H
