//
// Created by 李志磊 on 2018/9/1.
//

#ifndef VINCI_HTTP_H
#define VINCI_HTTP_H
#include "curl/curl.h"

#pragma comment ( lib, "libcurl.lib" )
#pragma comment ( lib, "ws2_32.lib" )
#pragma comment ( lib, "winmm.lib" )
#pragma comment ( lib, "wldap32.lib" )

struct imgBlob {
    char *memory;
    size_t size;
};

#define DEL_BLOB(blob) free(blob->memory); \
    free(blob);\
    blob = (struct imgBlob *) NULL;

struct imgBlob* getBlob(char * url);

#endif //VINCI_HTTP_H
