//
// Created by asxe on 2023/12/26.
//

#ifndef C_EASY_HTTP_H
#define C_EASY_HTTP_H
#endif

#include <stdio.h>
#include "utils.h"

struct MemoryStruct {
    string memory;
    size_t size;
};

typedef struct HTTP_RESPONSE {
    long status;
    string text;
    double total_time;
} HTTP_RESPONSE; //响应结构

typedef struct HEADERS {
    cString key;
    cString value;
    struct HEADERS *next;
} HEADERS; //头部结构

typedef struct POST_DATA {
    cString key;
    cString value;
    struct POST_DATA *next;
} POST_DATA; //post上传的数据


HEADERS *addHeader(HEADERS *headers, cString key, cString value); //headers增加
void freeHeaders(HEADERS *headers); //释放头内存
POST_DATA *addData(POST_DATA *postData, cString key, cString value);

void freePostData(POST_DATA *postData);

HTTP_RESPONSE get(cString url, HEADERS *headers); //get请求
HTTP_RESPONSE post(cString url, HEADERS *headers, POST_DATA *postData); //post请求
