<div align="center">
<h1 align="center">libeasyhttp</h1>

[![LICENSE](https://img.shields.io/badge/License-gpl3.0-red)]()

<p>简易的http请求库</p>
</div>

## 🎈使用方法

1. HTTP_RESPONSE get(const char *url, HEADERS *headers);
```C
#include "libeasyhttp/easy-http.h"

int main(void) {
    HEADERS *headerList = NULL;
    
    headerList = addHeader(headerList, "Content-Type", "text/html; charset=utf-8");
    headerList = addHeader(headerList, "User-Agent", "ASXE");
    
    HTTP_RESPONSE httpResponse = get("https://www.asxe.vip/", headerList);
    prinft("%s", httpResponse.text);
    return 0;
}
```

2. HTTP_RESPONSE post(const char *url, HEADERS *headers, POST_DATA *postData);

```C
#include "libeasyhttp/easy-http.h"

int main(void) {
    HEADERS *headerList = NULL;
    POST_DATA *postData = NULL;
    
    headerList = addHeader(headerList, "User-Agent", "ASXE");
    
    postData = addData(postData, "q", "测试");
    
    HTTP_RESPONSE httpResponse = post("http://127.0.0.1:1314/search/", headerList, postData);
    printf("%s", httpResponse.text);
    return 0;
}
```

## 🎉许可证

GPL3.0

