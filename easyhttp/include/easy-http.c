#include <curl/curl.h>
#include <string.h>
#include "easy-http.h"
#include "log.h"
#include "utils.h"

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb; // 接收到数据的总大小
    struct MemoryStruct *mem = (struct MemoryStruct *) userdata; // 将userdata转换为MemoryStruct结构体指针

    // 重新分配内存以存储新的数据
    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        // Out of memory
        error("Not enough memory (realloc returned NULL)");
        return 0; // 如果内存分配失败，返回0表示出错
    }

    memcpy(&(mem->memory[mem->size]), ptr, realsize);
    mem->size += realsize; // 更新已存储数据的大小
    mem->memory[mem->size] = 0; // 添加字符串结束符
    return realsize; // 返回接收到的数据大小
}

HEADERS *addHeader(HEADERS *headers, cString key, cString value) {
    HEADERS *newHeader = (HEADERS *) malloc(sizeof(HEADERS));
    if (newHeader == NULL) {
        error("Memory allocation failed.");
        return NULL;
    }

    newHeader->key = key;
    newHeader->value = value;
    newHeader->next = NULL;

    if (headers == NULL)
        return newHeader;

    HEADERS *temp = headers;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = newHeader;
    return headers;
}

POST_DATA *addData(POST_DATA *postData, cString key, cString value) {
    POST_DATA *newData = (POST_DATA *) malloc(sizeof(POST_DATA));
    if (newData == NULL) {
        error("Memory allocation failed.");
        return NULL;
    }

    newData->key = key;
    newData->value = value;
    newData->next = NULL;

    if (postData == NULL)
        return newData;

    POST_DATA *temp = postData;
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = newData;
    return postData;
}

void freeHeaders(HEADERS *headers) {
    HEADERS *current = headers;
    while (current != NULL) {
        HEADERS *temp = current;
        current = current->next;
        free(temp);
    }
}

void freePostData(POST_DATA *postData) {
    POST_DATA *current = postData;
    while (current != NULL) {
        POST_DATA *temp = current;
        current = current->next;
        free(temp);
    }
}

HTTP_RESPONSE get(cString url, HEADERS *headers) {
    CURL *curl;
    CURLcode res;

    HTTP_RESPONSE httpResponse;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    if (chunk.memory == NULL) {
        error("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    chunk.size = 0;

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headerList = NULL;
        HEADERS *temp = headers;
        while (temp != NULL) {
            char str[248] = "";
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
                fprintf(stderr, "curl_easy_getinfo() failed when get status: %s\n", curl_easy_strerror(res));

            res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &(httpResponse.total_time));
            if (res != CURLE_OK)
                fprintf(stderr, "curl_easy_getinfo() failed when get total time: %s\n", curl_easy_strerror(res));

        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headerList);
        freeHeaders(temp);
    }
    return httpResponse;
}

HTTP_RESPONSE post(cString url, HEADERS *headers, POST_DATA *postData) {
    CURL *curl;
    CURLcode res;

    HTTP_RESPONSE httpResponse;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    if (chunk.memory == NULL) {
        error("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    chunk.size = 0;

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *headerList = NULL;
        HEADERS *temp = headers;
        while (temp != NULL) {
            char strHeader[248] = "";
            strcat(strHeader, temp->key);
            strcat(strHeader, ": ");
            strcat(strHeader, temp->value);
            headerList = curl_slist_append(headerList, strHeader);
            temp = temp->next;
        }

        char strPostData[1024] = "";
        POST_DATA *pTemp = postData;
        while (pTemp != NULL) {
            char *escapedKey = curl_easy_escape(curl, pTemp->key, 0);
            char *escapedValue = curl_easy_escape(curl, pTemp->value, 0);

            strcat(strPostData, escapedKey);
            strcat(strPostData, "=");
            strcat(strPostData, escapedValue);

            curl_free(escapedKey);
            curl_free(escapedValue);

            pTemp = pTemp->next;
            if (pTemp != NULL)
                strcat(strPostData, "&");
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerList);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPostData);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        else {
            httpResponse.text = chunk.memory;

            res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &(httpResponse.status));
            if (res != CURLE_OK)
                fprintf(stderr, "curl_easy_getinfo() failed when get status: %s\n", curl_easy_strerror(res));

            res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &(httpResponse.total_time));
            if (res != CURLE_OK)
                fprintf(stderr, "curl_easy_getinfo() failed when get total time: %s\n", curl_easy_strerror(res));

        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(headerList);
        freeHeaders(temp);
        freePostData(pTemp);
    }
    return httpResponse;
}
