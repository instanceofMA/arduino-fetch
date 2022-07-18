# Fetch for Arduino
fetch is a high level HTTP Request Library that gives you a javascript fetch like API. It also supports making asynchronous/non-blocking requests, check out [this example](https://github.com/instanceofMA/arduino-fetch/blob/main/examples/esp8266/async/get/get.ino) and [others](https://github.com/instanceofMA/arduino-fetch/blob/main/examples).

```js
ResponseOptions options;
options.method = "POST";
// options.fingerprint = "DC 78 3C 09 3A 78 E3 A0 BA A9 C5 4F 7A A0 87 6F 89 01 71 4C";
options.caCert = "";
options.headers["Content-Type"] = "application/json";
options.body = "{\"email\": \"test@test.com\", \"password\": \"test:80\"}";

/** In JSON, this would look like:
 * options = {
 *      method: "POST",
 *      fingerprint: "DC 78 3C 09 3A 78 E3 A0 BA A9 C5 4F 7A A0 87 6F 89 01 71 4C",
 *      headers: { contentType: "application/json" },
 *      body: "{\"email\": \"test@test.com\", \"password\": \"test:80\"}",
 * }
 */

Response response = fetch("https://api.grandeur.tech/auth/login/?apiKey=grandeurkywxmoy914080rxf9dh05n7e", options);
```

[Here's the link to the release article on instructables](https://www.instructables.com/Fetch-the-Easiest-Way-to-Make-HTTP-Requests-From-Y/).

# Installation

You can install fetch in your Arduino Project by:
1. Installing from the [Arduino Library Manager]([https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries#using-the-library-manager:~:text=Library%20Reference.-,Using%20the%20Library%20Manager,-To%20install%20a)) directly.
2. Download this repo as a ZIP and [import it into your project](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries#using-the-library-manager:~:text=these%20instructions.-,Importing%20a%20.zip%20Library,-Libraries%20are%20often).

# Include

```cpp
#include <Fetch.h>
```

# Usage

```cpp
Response response = fetch(const char* url, RequestOptions options);
FetchClient client = fetch(const char* url, RequestOptions options, OnResponseCallback callback);
```

# Request Options
```cpp
RequestOptions options;

/**
    {
        method: "GET" || "POST" || "HEAD" || "PUT" || "DELETE",
        headers: { "Content-Type": "application/x-www-form-urlencoded", "Content-Length": Automatic, "Host: FromURL, "User-Agent": "arduino-fetch", "Cookie": "", "Accept": "* /*", "Connection": "close", "Transfer-Encoding": "chunked" },
        body: "",
        redirect: "follow" || "manual", "error",
        follow: Integer,

    }
*/

ResponseOptions options;
options.method = "POST";
options.headers["Content-Type"] = "application/json";
options.headers["Connection"] = "keep-alive";
options.body = "email=EMAIL&password=PASSWORD";
```

<!-- ```cpp
ResponseOptions options;
options["method"] = "POST";
options["body"] = "email=EMAIL&password=PASSWORD";
``` -->

## Request Body
<!-- 
```cpp
MultipartFormBody body;
URLEncodedBody body;
JSONBody body;

body["email"] = email;
body["password"] = password;
``` -->
```cpp
String body;
// application/x-www-form-urlencoded
body = "email=EMAIL&password=PASSWORD";
// application/json
body = "{\"email\":\"EMAIL\", \"password\"=\"PASSWORD\"}";
```
## Request Headers

```cpp
Headers headers;

headers["Content-Type"] = "application/json";
headers["Connection"] = "keep-alive";
headers["Cookie"] = "abc=def,ghi=jkl"; // Not yet supported.
```
# Response

```cpp
Response response;

String data = response.text();
JSONBody data = response.json(); // Not yet supported.
const Blob = response.blob(); // Not yet supported.
const FormData = response.formData(); // Not yet supported.
unsigned int[] array = response.arrayBuffer(); // Not yet supported.

bool ok = response.ok;
int status = response.status;
String statusText = response.statusText;
bool redirected = response.redirected; // Not yet supported.
String type = response.type; // Not yet supported.
String response.headers["content-type"];
Headers headers = response.headers.raw(); // Not yet supported
const char* contentTypeHeader = headers["content-type"]; // Not yet supported.

```
