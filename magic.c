//
// Created by 李志磊 on 2018/9/1.
//

#include "magic.h"
#include <string.h>
#include <libgen.h>
#include <mhash.h>
#include <sys/stat.h>


Image * readFile(char *path) {
    ImageInfo *imageInfo;
    imageInfo=CloneImageInfo(0);
    ExceptionInfo exception;
    GetExceptionInfo(&exception);
    Image * img = ReadImage(imageInfo, &exception);
    DestroyImageInfo(imageInfo);
    DestroyExceptionInfo(&exception);
    return img;
}

int create_dir(const char *dir) {
    char tmp [MaxTextExtent] = "";
    char pDir [MaxTextExtent] = "";
    strcat(tmp, dir);
    strcat(pDir,dirname(tmp));

    if(access(dir, 0)!=0)
    {
        if(access(pDir, 0)!=0)
        {
            create_dir(pDir);
        }
        if(mkdir(dir, 0777)==-1)
        {
            printf("mkdir   error\n");
            return -1;
        }
    }
    return 1;
}

unsigned int writeFile(const char *path, Image *image) {
    ImageInfo *imageInfo;
    imageInfo=CloneImageInfo(0);
    ExceptionInfo exception;
    GetExceptionInfo(&exception);
    char dir [MaxTextExtent] ="";
    char tmp [MaxTextExtent] = "";
    strcat(tmp, path);
    strcat(dir,dirname(tmp));
    if (!create_dir(dir)) {
        printf("mkdir   error\n");
        return -1;
    }

    (void) strcpy(image->filename, path);
    if (!WriteImage (imageInfo,image))
    {
        CatchException(&image->exception);
        return 0;
    }
    return 1;
}

Image * blob2image(const void *blob, const size_t length) {
    ImageInfo *imageInfo;
    imageInfo=CloneImageInfo(0);
    ExceptionInfo exception;
    GetExceptionInfo(&exception);
    Image * img  = BlobToImage(imageInfo, blob, length, &exception);
    DestroyImageInfo(imageInfo);
    DestroyExceptionInfo(&exception);
    return img;
}

void * image2blob(Image *image, size_t *length) {
    ImageInfo *imageInfo;
    imageInfo=CloneImageInfo(0);
    ExceptionInfo exception;
    GetExceptionInfo(&exception);

    void * blob = ImageToBlob(imageInfo, image, length, &exception);

    DestroyImageInfo(imageInfo);
    DestroyExceptionInfo(&exception);
    return blob;
}

Image * chop(const Image *image, int width, int height) {
    RectangleInfo *chop_info = malloc(sizeof(RectangleInfo));
    ExceptionInfo exception;
    GetExceptionInfo(&exception);

    Image *image2 = (Image *) NULL;
    Image *image1 = (Image *) NULL;
    double height_f = (double)height;
    double width_f = (double)width;

    if (image->rows / height_f * width_f > image->columns * 1.0) {
        chop_info->height = (int) ((image->rows - (image->columns / width_f * height_f)) / 2);
        chop_info->width = 0;
        chop_info->x = 0;
        chop_info->y = 0;
        image1 = ChopImage(image, chop_info, &exception);
        chop_info->y = (image->columns / width_f * height_f);
        image2 = ChopImage(image1, chop_info, &exception);
    } else if (image->rows / height_f * width_f < image->columns * 1.0){
        chop_info->width = (int) ((image->columns - (image->rows / height_f * width_f)) / 2);
        chop_info->height = 0;
        chop_info->x = 0;
        chop_info->y = 0;
        image1 = ChopImage(image, chop_info, &exception);
        chop_info->x = (image->rows / height_f * width_f);
        image2 = ChopImage(image1, chop_info, &exception);
    } else{
        chop_info->width = 0;
        chop_info->height = 0;
        chop_info->x = 0;
        chop_info->y = 0;
        image2 = ChopImage(image, chop_info, &exception);
    }
    DestroyImage(image1);
    free(chop_info);
    DestroyExceptionInfo(&exception);
    return image2;
}