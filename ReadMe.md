<div align="center">
<h1 align="center">libeasyhttp</h1>

[![LICENSE](https://img.shields.io/badge/License-gpl3.0-red)]()

<p>简易的http请求库</p>
</div>

## 🎈使用方法
```C
#include "libeasyhttp/easy-http.h"

int main(void) {
    HEADERS *headerList = NULL;
    headerList = addHeader(headerList, "Content-Type", "application/text");
    headerList = addHeader(headerList, "User-Agent", "ASXE");
    
    HTTP_RESPONSE httpResponse = get("https://www.asxe.vip/", headerList);
    prinft("%s", httpResponse.text);
    getchar();
    return 0;
}
```

## 🎉许可证

GPL3.0

