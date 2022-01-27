# Arduino Fetch
High level HTTP Request Library that gives a javascript fetch like API.

# Include

```cpp
#include <ArduinoFetch.h>
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
        headers: { "Content-Type": "application/urlencoded", "Content-Length": Automatic, "Host: FromURL, "User-Agent": "arduino-fetch", "Cookie", "Accept": "* /*", "Connection": "close", "Transfer-Encoding": "chunked" },
        body: {},
        redirect: "follow" || "manual", "error",
        follow: Integer,

    }
*/

options["method"] = "POST";
options["headers"]["Content-Type"] = "application/json";
options["body"]["email"] = email;
options["body"]["password"] = password;
```
## Request Body

```cpp
MultipartFormBody body;
URLEncodedBody body;
JSONBody body;

body["email"] = email;
body["password"] = password;
```

## Request Headers

```cpp
Headers headers;

headers["Content-Type"] = "application/json";
```

# Response

```cpp
Response response;

JSONBody data = response.json();
const char* data = response.text();
const Blob = response.blob();
const FormData = response.formData();
unsigned int[] array = response.arrayBuffer();

bool ok = response.ok;
int status = response.status;
const char* statusText = response.statusText;
bool redirected = response.redirected;
const char* type = response.type;
const char* response.headers.get("content-type");
Headers headers = response.headers.raw();
const char* contentTypeHeader = headers["content-type"];

```
