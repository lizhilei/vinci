//
// Created by 李志磊 on 2018/9/1.
//

#ifndef VINCI_MAGIN_H
#define VINCI_MAGIN_H

#include "magick/api.h"

Image * readFile(char *path);
Image * blob2image( const void *blob, const size_t length);
Image * chop(const Image *image, int width, int height);
unsigned int writeFile(const char *path, Image *image);
void * image2blob(Image *image, size_t *length);


#endif //VINCI_MAGIN_H
