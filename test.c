//
// Created by asxe on 2024/2/3.
//
#include <stdio.h>
#include "easyhttp/include/easy-http.h"

int main(void) {
    HEADERS *headerList = NULL;

    headerList = addHeader(headerList, "Content-Type", "text/html; charset=utf-8");
    headerList = addHeader(headerList, "User-Agent", "ASXE");

    HTTP_RESPONSE httpResponse = get("https://www.asxe.vip/", headerList);
    printf("%s", httpResponse.text);
    return 0;
}
