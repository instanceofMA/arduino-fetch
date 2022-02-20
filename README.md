# Fetch for Arduino
fetch is a high level HTTP Request Library that gives you a javascript fetch like API.

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

# Include

```cpp
#include <Fetch.h>
```

# Usage

```cpp
Response response = fetch(const char* url, RequestOptions options);
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
