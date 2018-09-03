//
// Created by 李志磊 on 2018/9/1.
//

#include "http.h"

#include <stdlib.h>
#include <string.h>

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct imgBlob *mem = (struct imgBlob *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

struct imgBlob* getBlob(char * url) {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    struct imgBlob* chunk = malloc(sizeof(struct imgBlob));

    chunk->memory = malloc(1);
    chunk->size = 0;

    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)chunk);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return chunk;
}
