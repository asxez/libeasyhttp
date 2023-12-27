#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include "easy-http.h"


size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb; // 接收到数据的总大小
    struct MemoryStruct *mem = (struct MemoryStruct *) userdata; // 将userdata转换为MemoryStruct结构体指针

    // 重新分配内存以存储新的数据
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        // Out of memory!
        fprintf(stderr, "Not enough memory (realloc returned NULL)\n");
        return 0; // 如果内存分配失败，返回0表示出错
    }

    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize; // 更新已存储数据的大小
    mem->memory[mem->size] = 0; // 添加字符串结束符
    return realsize; // 返回接收到的数据大小
}

void initHeaders(HEADERS *headers) {
    headers->key = NULL;
    headers->value = NULL;
    headers->next = NULL;
}

HEADERS *addHeader(HEADERS *headers, const char *key, const char *value) {
    HEADERS *newHeader = (HEADERS *) malloc(sizeof(HEADERS));
    if (newHeader == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    newHeader->key = key;
    newHeader->value = value;
    newHeader->next = NULL;

    if (headers == NULL) {
        return newHeader;
    }

    HEADERS *temp = headers;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newHeader;
    return headers;
}

void freeHeaders(HEADERS *headers) {
    HEADERS *current = headers;
    while (current != NULL) {
        HEADERS *temp = current;
        current = current->next;
        free(temp);
    }
}

HTTP_RESPONSE get(const char *url, HEADERS *headers) {
    char str[1024] = "";

    CURL *curl;
    CURLcode res;

    HTTP_RESPONSE httpResponse;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headerList = NULL;
        HEADERS *temp = headers;
        while (temp != NULL) {
            strcat(str, headers->key);
            strcat(str, ": ");
            strcat(str, headers->value);
            headerList = curl_slist_append(headerList, str);
            temp = temp->next;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else {
            httpResponse.text = chunk.memory;
            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(httpResponse.status));
            if (res != CURLE_OK)
                fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));

            res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &(httpResponse.total_time));
            if (res != CURLE_OK)
                fprintf(stderr, "curl_easy_getinfo() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headerList);
        free(chunk.memory);
        freeHeaders(temp);
    }
    return httpResponse;
}
