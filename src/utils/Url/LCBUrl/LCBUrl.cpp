/*
    LCBUrl.cpp -  Library for handling URLs - This library will allow
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

// Include this library's description file

#include "LCBUrl.h"

// Constructor/Destructor ////////////////////////////////////////////////
// Handle the creation, setup, and destruction of instances

LCBUrl::LCBUrl(const String &newUrl)
{
    initRegisters();
    if (newUrl.length() > 0)
        setUrl(newUrl);
}

// Public Methods //////////////////////////////////////////////////////////////
// Functions available in sketches, this library, and other libraries

bool LCBUrl::setUrl(const String &newUrl)
{
    bool retVal = false;
    if (newUrl.length() > 0)
    {
        initRegisters();
        rawurl = newUrl;
    }
    if (rawurl.length() != 0)
    {
        if (getUrl().length() != 0)
        {
            retVal = true;
        }
    }
    return retVal;
}

String LCBUrl::getUrl() // Returned parsed/normalized URL
{
    if (url.length() == 0)
    {
        url = "";
        url.concat(getScheme());
        url.concat(F("://"));
        url.concat(getAuthority());
        url.concat(F("/"));
        url.concat(getPath());
        if (getQuery() != "")
        {
            url.concat(F("?"));
            url.concat(getQuery());
        }
        if (getFragment() != "")
        {
            url.concat(F("#"));
            url.concat(getFragment());
        }
        if ((getScheme() == "") || (getHost() == ""))
        {
            return url;
        }
    }
    return url;
}

String LCBUrl::getIPUrl() // Return cleaned URL with IP instead of FQDN
{
    if (ipurl.length() == 0)
    {
        ipurl = "";
        ipurl.concat(getScheme()); // http or https
        ipurl.concat(F("://"));
        ipurl.concat(getIPAuthority()); // Username, password, host and port
        ipurl.concat(F("/"));
        ipurl.concat(getPath()); // Path
        if (getQuery() != "") // Add a query string
        {
            ipurl.concat(F("?"));
            ipurl.concat(getQuery());
        }
        if (getFragment() != "") // Add a fragment
        {
            ipurl.concat(F("#"));
            ipurl.concat(getFragment());
        }
        if ((getScheme() == "") || (getHost() == "")) // No idea what I was thinking here
        {
            return ipurl;
        }
    }
    return ipurl;
}

String LCBUrl::getScheme() // Returns URL scheme
{ // Currrently only finds http and https as scheme
    if (scheme.length() == 0)
    {
        scheme = "";
        int loc = getCleanTriplets().indexOf(F(":"));
        if (loc > 0)
        {
            String s = getCleanTriplets().substring(0, loc);
            s.toLowerCase();
            if ((s == F("http")) || (s == F("https")))
            {
                scheme = s;
            }
        }
    }
    return scheme;
}

String LCBUrl::getUserInfo() // Return username:passsword
{
    // UserInfo will be anything to the left of the last @ in authority
    if (userinfo.length() == 0)
    {
        userinfo = "";
        String tempUrl = getRawAuthority();
        if (tempUrl)
        {
            int loc = tempUrl.lastIndexOf(F("@"));
            if (loc > 0)
            {
                userinfo = tempUrl.substring(0, loc);
            }
        }
    }
    return userinfo;
}

String LCBUrl::getUserName() // Return username from authority
{
    // User Name will be anything to the left of : in userinfo
    if (username.length() == 0)
    {
        username = "";
        String tempUrl = getUserInfo();
        if (tempUrl)
        {
            int loc = tempUrl.lastIndexOf(F(":"));
            if (loc > 0)
            {
                username = tempUrl.substring(0, loc);
            }
        }
    }
    return username;
}

String LCBUrl::getPassword() // Return password from authority
{
    // Password will be anything to the right of : in userinfo
    if (password.length() == 0)
    {
        password = "";
        String tempUrl = getUserInfo();
        if (tempUrl)
        {
            int loc = tempUrl.lastIndexOf(F(":"));
            if (loc > 0)
            {
                password = tempUrl.substring(loc + 1);
            }
        }
    }
    return password;
}

String LCBUrl::getHost() // Return FQDN
{
    // Host will be anything between @ and : or / in authority
    if (host.length() == 0)
    {
        host = "";
        String tempUrl = getRawAuthority();
        if (tempUrl)
        {
            int startloc = tempUrl.lastIndexOf(F("@"));
            if (startloc > 0)
            {
                tempUrl = tempUrl.substring(startloc + 1, tempUrl.length());
            }
            int endloc = tempUrl.indexOf(F(":"), 0);
            if (endloc > 0)
            {
                tempUrl = tempUrl.substring(0, endloc);
            }
            host = tempUrl;
        }
    }
    host.toLowerCase();
    return host;
}

unsigned int LCBUrl::getPort() // Port will be any integer between : and / in authority
{
    if (port == 0)
    {
        String tempUrl = getRawAuthority();
        if (tempUrl.length() > 0)
        {
            int startloc = tempUrl.lastIndexOf(F(":"));
            int endloc = tempUrl.lastIndexOf(F("/"));
            if (startloc != -1)
                tempUrl = tempUrl.substring(startloc + 1);
            if (endloc != -1)
                tempUrl = tempUrl.substring(0, endloc - 1);
            if (startloc != -1)
                port = tempUrl.toInt();
        }
    }
    if (port == 0)
    {
        if (getScheme() == F("http"))
            port = 80;
        if (getScheme() == F("https"))
            port = 443;
    }
    return port;
}

String LCBUrl::getAuthority() // Return username:password@fqdn:port
{
    if (authority.length() == 0)
    {
        authority = "";
        if (getUserName().length() > 0)
        {
            authority = getUserName();
        }
        if (getPassword().length() > 0)
        {
            authority.concat(F(":"));
            authority.concat(getPassword());
        }
        if (authority.length() > 0)
        {
            authority.concat(F("@"));
        }
        authority.concat(getHost());
        if (getPort() > 0)
        {
            if (
                ((getScheme() == F("http")) && (port != 80)) ||
                ((getScheme() == F("https")) && (port != 443)))
            {
                authority.concat(F(":"));
                authority.concat(String(getPort()));
            }
        }
    }
    return authority;
}

String LCBUrl::getIPAuthority() // Returns {username (optional)}:{password (optional)}@{fqdn}
{
    if (ipauthority.length() == 0)
    {
        ipauthority = "";
        if (getUserName().length() > 0)
        {
            ipauthority = getUserName();
        }
        if (getPassword().length() > 0)
        {
            ipauthority.concat(F(":"));
            ipauthority.concat(getPassword());
        }
        if (ipauthority.length() > 0)
        {
            ipauthority.concat(F("@"));
        }
        if (ipaddress == (IPAddress)INADDR_NONE)
        {
            ipauthority.concat(getIP(getHost().c_str()).toString());
        }
        else
        {
            ipauthority.concat(ipaddress.toString());
        }
        if (getPort() > 0)
        {
            if (
                ((getScheme() == F("http")) && (port != 80)) ||
                ((getScheme() == F("https")) && (port != 443)))
            {
                ipauthority.concat(F(":"));
                ipauthority.concat(String(getPort()));
            }
        }
    }
    return ipauthority;
}

String LCBUrl::getPath() // Get all after host and port, before query and frag
{
    if (path.length() == 0)
    {
        path = getPathSegment();
        // TODO: Remove dot segments per 5.2.4
    }
    return path;
}

String LCBUrl::getQuery() // Get text after '?' and before '#'
{
    if (query.length() == 0)
    {
        query = "";
        String tempUrl = getAfterPath();
        int queryloc = tempUrl.lastIndexOf(F("#"));
        if (tempUrl.startsWith(F("?")))
            query = tempUrl.substring(1, queryloc);
        else
            query = tempUrl.substring(0, queryloc);
    }
    return query;
}

String LCBUrl::getFragment() // Get all after '#'
{
    if (fragment.length() == 0)
    {
        fragment = "";
        String tempUrl = getAfterPath();
        int fragloc = tempUrl.lastIndexOf(F("#"));
        if (fragloc != -1)
        {
            fragment = tempUrl.substring(fragloc + 1);
        }
    }
    return fragment;
}

// Private Methods /////////////////////////////////////////////////////////////
// Functions only available to other functions in this library

String LCBUrl::getStripScheme() // Remove scheme and "://" discriminately
{
    if (stripscheme.length() == 0)
    {
        stripscheme = "";
        String tempUrl = getCleanTriplets();
        int length = getCleanTriplets().length();
        int slength = getScheme().length();
        // Remove scheme and ://
        if ((slength > 0) && (length > slength + 3))
        {
            tempUrl = tempUrl.substring(slength); // Remove scheme;
            for (int i = 0; i <= 3; i++)
            { // Remove "://" discriminately
                if (
                    (isAlphaNumeric(tempUrl.charAt(i))) ||
                    (tempUrl.charAt(i) == '-') ||
                    (tempUrl.charAt(i) == '.') ||
                    (tempUrl.charAt(i) == '_') ||
                    (tempUrl.charAt(i) == '~'))
                {
                    tempUrl = tempUrl.substring(i);
                    stripscheme = tempUrl;
                }
            }
        }
    }
    return stripscheme;
}

String LCBUrl::getRawAuthority() // Authority is similar to "lbussy@raspberrypi.local:80"
{
    if (rawauthority.length() == 0)
    {
        rawauthority = "";
        String tempUrl = getStripScheme();
        if ((tempUrl) && (tempUrl.length() > 0))
        {
            int loc = tempUrl.indexOf(F("/"), 0);
            if (loc > 0)
            {
                rawauthority = tempUrl.substring(0, loc);
            }
            else
            {
                rawauthority = tempUrl;
            }
        }
    }
    return rawauthority;
}

String getDotSegmentsClear()
{
    // TODO:  https://tools.ietf.org/html/rfc3986#section-5.2.4
    return "TODO";
}

String LCBUrl::getAfterAuth() // Get anything after the authority
{
    if (afterauth.length() == 0)
    {
        afterauth = "";
        String tempUrl = getStripScheme();
        unsigned int length = getRawAuthority().length();

        if (tempUrl.length() > length + 1)
        {
            tempUrl.remove(length + 1); // Remove authority
            afterauth = tempUrl;
        }
    }
    return afterauth;
}

String LCBUrl::getAfterPath() // Get anything after the path
{
    if (afterpath.length() == 0)
    {
        afterpath = "";
        String tempUrl = getCleanTriplets();
        int queryloc = tempUrl.lastIndexOf(F("?"));
        if (queryloc != -1)
        {
            afterpath = tempUrl.substring(queryloc);
        }
        else
        {
            int queryloc = tempUrl.lastIndexOf(F("#"));
            if (queryloc != -1)
            {
                afterpath = tempUrl.substring(queryloc);
            }
        }
    }
    return afterpath;
}

String LCBUrl::getCleanTriplets() // Convert URL encoded triplets
{
    workingurl = rawurl;
    unsigned int i = workingurl.length();
    while (i != 0)
    {
        int loc = workingurl.lastIndexOf(F("%"), i);
        if (loc != -1)
        {
            String triplet = rawurl.substring(loc + 1, loc + 3);
            triplet.toUpperCase();
            const char *hex = triplet.c_str();

            // Convert hex string to a character
            int x;
            char *endptr;
            x = strtol(hex, &endptr, 16);
            char character = char(x);

            // Check for characters which should be decoded
            if (
                (isAlphaNumeric(character)) ||
                (character == '-') ||
                (character == '.') ||
                (character == '_') ||
                (character == '~'))
            {
                String before = workingurl.substring(0, loc);
                String after = workingurl.substring(loc + 3);
                workingurl = before + character + after;
            }
            i--;
        }
        else
        {
            i = 0;
        }
    }
    return workingurl;
}

String LCBUrl::getPathSegment() // Path will be between the / after host and ?
{
    if (pathsegment.length() == 0)
    {
        String tempUrl = getStripScheme();

        int startloc = tempUrl.indexOf(F("/"));
        if (startloc > 0)
        {
            tempUrl = tempUrl.substring(startloc + 1);
        }
        else
        {
            tempUrl = "";
        }

        int endloc = tempUrl.lastIndexOf(F("?"));
        if (endloc != -1)
        {
            tempUrl = tempUrl.substring(0, endloc);
        }
        else
        {
            int endloc = tempUrl.lastIndexOf(F("#"));
            if (endloc != -1)
            {
                tempUrl = tempUrl.substring(0, endloc - 1);
            }
        }
        pathsegment = tempUrl;
    }
    // TODO: Remove dot segments per 5.2.4
    return pathsegment;
}

void LCBUrl::initRegisters() // Clear out the internals to allow the object to be re-used
{
    rawurl = "";
    url = "";
    ipurl = "";
    workingurl = "";
    scheme = "";
    stripscheme = "";
    rawauthority = "";
    afterauth = "";
    userinfo = "";
    username = "";
    password = "";
    host = "";
    ipaddress = INADDR_NONE;
    port = 0;
    authority = "";
    ipauthority = "";
    pathsegment = "";
    path = "";
    afterpath = "";
    query = "";
    fragment = "";
}

// Utility Methods //////////////////////////////////////////////////////////////
// These do not directly influence or change the core library properties

bool LCBUrl::isMDNS() // (deprecated) Determine if FQDN is mDNS
{
    return isMDNS(getHost().c_str());
}

bool LCBUrl::isMDNS(const char *hostName) // Determine if FQDN is mDNS
{
    // Check for a valid mDNS name

	// Split and check labels
	char * label;
	char * lastLabel = "\0";
    int labelCount = 0;
    char hn[strlen(hostName) + 1];
    strlcpy(hn, hostName, strlen(hostName) + 1);
	label = strtok(hn, ".");
	while (label != NULL)
	{
        labelCount++;
		lastLabel = label;
		if (! isValidLabel(label))
			return false;
		label = strtok (NULL, ".");
	}

    // Cannot have more than two labels (plus "local")
    // https://github.com/lathiat/nss-mdns/blob/master/README.md#etcmdnsallow
    if (labelCount > 3)
        return false;

    // Must end in ".local"
	if (strcmp(lastLabel, "local") != 0)
		return false;

    return true;
}

IPAddress LCBUrl::getIP() // (deprecated) Return IP address of FQDN (helpful for mDNS)
{
    return getIP(getHost().c_str());
}

IPAddress LCBUrl::getIP(const char * hostName) // Return IP address of FQDN (helpful for mDNS)
{
    IPAddress returnIP = INADDR_NONE;

    // First try to resolve the address fresh
    if (isMDNS(hostName))
    { // Host is an mDNS name
        char hn[strlen(hostName) + 1];
        strlcpy(hn, hostName, sizeof(hn));
        hn[strlen(hn)-6] = 0;

#ifdef ESP8266
        int result = WiFi.hostByName(hostName, returnIP);

        if (result == 1)
        {
            if (returnIP != INADDR_NONE)
            {
                ipaddress = returnIP;
            }
        }
#else
        struct ip4_addr addr;
        addr.addr = 0;
        esp_err_t err = mdns_query_a(hn, 2000, &addr);

        if (err == ESP_OK)
        {
            char ipstring[16];
            snprintf(ipstring, sizeof(ipstring), IPSTR, IP2STR(&addr));
            returnIP.fromString(ipstring);
            if (returnIP != INADDR_NONE)
            {
                ipaddress = returnIP;
            }
        }
#endif
    }
    else
    { // Host is not an mDNS name
        if (WiFi.hostByName(hostName, returnIP) == 1)
        {
            ipaddress = returnIP;
        }
    }

    // If we got a new IP address, we will use it.  Otherwise
    // we will use last known good (if there is one), falls back
    // to INADDR_NONE
    return ipaddress;
}

bool LCBUrl::isValidIP(const char * hostName)
{
    // Check if hostName is a valid IP address
    return IPAddress().fromString(hostName);
}

int LCBUrl::labelCount(const char * hostName)
{
    // Return count of labels in a hostname
	char * label;
    int labelCount = 0;
    char hn[strlen(hostName)];
    strlcpy(hn, hostName, strlen(hostName));
	label = strtok(hn, ".");
	while (label != NULL)
	{
        labelCount++;
		label = strtok (NULL, ".");
	}
    return labelCount;
}

bool LCBUrl::isANumber(const char * str)
{
    char* p;
    strtol(str, &p, 10);
    if (*p) {
        return false;
    }
    return true;
}

bool LCBUrl::isValidLabel(const char *label)
{
    // Check that hostname label is valid

	// Is at least 1 and no more than 63
	if (strlen(label) < 1 || strlen(label) > 63)
		return false;

	// Does not begin or end with hyphen
	if (label[0] == '-' || label[strlen(label) - 1] == '-')
		return false;

	// Does not contain all numbers
	if (isANumber(label))
		return false;

	// Contains only letters, numbers and hyphen
    for (int i = 0; i < strlen(label); i++)
    {
        if (! isalnum(label[i]) && label[i] != '-')
            return false;
    }
    return true;
}

bool LCBUrl::isValidHostName(const char *hostName)
{
	// This will generally follow RFC1123 and RFC1034

	// Check for min/max length (remember root label and octet count)
	if (strlen(hostName) < 1 || strlen(hostName) > 253)
		return false;

	// Check if this is a valid IP address
	if (isValidIP(hostName))
		return true;

	// Next check for mDNS
	if (isMDNS(hostName))
		return true;

	// Next, check to see if each label is valid
	char * label;
    char hn[strlen(hostName)];
    strlcpy(hn, hostName, strlen(hostName));
	label = strtok(hn, ".");
	while (label != NULL)
	{
		if (! isValidLabel(label))
			return false;
		label = strtok (NULL, ".");
	}

	return true;
}
