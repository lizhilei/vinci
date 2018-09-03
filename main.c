//
// Created by 李志磊 on 2018/9/1.
//


#include "fcgi_config.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>



#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
extern char **environ;
#include "fcgi_stdio.h"
#include <sys/types.h>
#include "magick/api.h"

#include "http.h"
#include "magic.h"


static void PrintEnv(char *label, char **envp)
{
    printf("%s:<br>\n<pre>\n", label);
    for ( ; *envp != NULL; envp++) {
        printf("%s\n", *envp);
    }
    printf("</pre><p>\n");
}

int main () {



//    FILE *fp = NULL;
//    char buf[11] = "";
//
//    Image *image1 = (Image *) NULL;

//
//
//
//    FCGI_FILE* fp123;
//    size_t size;
//    int
//        arg = 1,
//        exit_status = 0,
//        width = 0,
//        height = 0;
//    char
//        infile[MaxTextExtent],
//        outfile[MaxTextExtent],
//        outfile1[MaxTextExtent];
//
//
//
//
//
//    (void) strncpy(infile, "https://wx3.sinaimg.cn/mw690/005WChxigy1fus6j8vd6tj31oh2ipkjo.jpg", MaxTextExtent-1 );
//    (void) strncpy(outfile, "/Users/zhilei3/CLionProjects/Vinci/img/xxx.jpg", MaxTextExtent-1 );
//    (void) strncpy(outfile1, "/Users/zhilei3/CLionProjects/Vinci/img/xxx1.jpg", MaxTextExtent-1 );


//    struct imgBlob *blob = getBlob(infile);
//    DEL_BLOB(blob)



    char **   initialEnv = environ;
    char **   params = malloc(3 * sizeof(char*));
    char *    ptr = NULL;
    int       num = 0;
    int       width = 0,
              height = 0;
    char *    pic_name;
    char *    script_name;
    char      source_path[MaxTextExtent],
              save_path[MaxTextExtent],
              img_url[MaxTextExtent];
    char * res;
    size_t  size;
    char  line[1024];

    struct imgBlob * blob;

    FILE *    fp;

    Image *image = (Image *) NULL;
    Image *image_r = (Image *) NULL;

    InitializeMagick(NULL);


    while (FCGI_Accept() >= 0) {
        num = 0;
        strncpy(source_path, "./simg/", MaxTextExtent-1 );
        strncpy(save_path, "./img/", MaxTextExtent-1 );
        strncpy(img_url, "http://img.xczfw.cn/", MaxTextExtent-1 );
        script_name = getenv("SCRIPT_NAME");
        ptr = strtok(script_name, "/");
        while (ptr != NULL && num < 3) {
            params[num++] = ptr;
            printf("ptr=%s\r\n", ptr);
            ptr = strtok(NULL, "/");
        }
        width = atoi(params[0]);
        height = atoi(params[1]);
        pic_name = params[2];

        if (width <= 0 || height <= 0) {
            goto size_error;
        }

        if(!strlen(pic_name)) {
            goto image_error;
        }

        strcat(save_path, params[0]);
        strcat(save_path, "/");
        strcat(save_path, params[1]);
        strcat(save_path, "/");
        strcat(save_path, pic_name);
        fp = fopen(save_path,"rb");
        if (fp != NULL) {
            blob = malloc(sizeof(struct imgBlob));
            blob->memory = malloc(1);
            blob->size = 0;

            while(!feof(fp)) {
                size = fread(line, 1,1024,fp);
                blob->memory = realloc(blob->memory, blob->size + size);
                memcpy(&(blob->memory[blob->size]), line, size);
                blob->size += size;
            }
            fclose(fp);
            goto echo_img;
        }


        strcat(source_path, pic_name);
        image = readFile(source_path);
        if ((image != (Image *) NULL)) {
            goto deal_img;
        }

        strcat(img_url, pic_name);
        blob = getBlob(img_url);
        image = blob2image( blob->memory, blob->size);
        writeFile(source_path, image);
        DEL_BLOB(blob)
        if (image != (Image *) NULL) {
            goto deal_img;
        } else {
            goto image_error;
        }

        deal_img:
        image_r = chop(image, width, height);
//        writeFile(save_path, image_r);

        blob = malloc(sizeof(struct imgBlob));
        blob->memory = image2blob(image_r, &blob->size);
        DestroyImage(image);
        DestroyImage(image_r);
        goto echo_img;


        echo_img:
        printf("HTTP/1.0 200 OK\r\n");
        printf("content-type: image/jpeg\r\n");
        printf("\r\n");
        fwrite(blob->memory, 1, blob->size, FCGI_stdout);


        goto next;

        size_error:
        printf("HTTP/1.0 200 OK\r\n");
        printf("\r\n");
        printf("size_error");
        goto next;

        image_error:
        printf("HTTP/1.0 200 OK\r\n");
        printf("\r\n");
        printf("image_error");
        goto next;

        ex:
        printf("HTTP/1.0 200 OK\r\n");
        printf("\r\n");
        printf("ex_error");

        goto next;

        next:
        if(blob != NULL) {
            DEL_BLOB(blob);
        }
        printf("\r\n");

    } /* while */

    program_exit:
    return 0;
}
