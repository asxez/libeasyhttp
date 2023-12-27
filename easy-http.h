//
// Created by asxe on 2023/12/26.
//

#ifndef C_EASY_HTTP_H
#define C_EASY_HTTP_H
#endif

#include <stdio.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

typedef struct HTTP_RESPONSE {
    long status;
    char *text;
    double total_time;
} HTTP_RESPONSE; //响应结构

typedef struct HEADERS {
    const char *key;
    const char *value;
    struct HEADERS *next;
} HEADERS; //头部结构

typedef struct POST_DATA {
    const char *key;
    const char *value;
    struct POST_DATA *next;
} POST_DATA; //post上传的数据


HEADERS *addHeader(HEADERS *headers, const char *key, const char *value); //headers增加
void freeHeaders(HEADERS *headers); //释放头内存
POST_DATA *addData(POST_DATA *postData, const char *key, const char *value);

void freePostData(POST_DATA *postData);

HTTP_RESPONSE get(const char *url, HEADERS *headers); //get请求
HTTP_RESPONSE post(const char *url, HEADERS *headers, POST_DATA *postData); //post请求
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata); //回调函数
