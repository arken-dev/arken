// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <zip.h>

#include <arken/base>
#include <arken/compress/zip.h>

static void safe_create_dir(const char *dir)
{
    if (mkdir(dir, 0755) < 0) {
        if (errno != EEXIST) {
            perror(dir);
            exit(1);
        }
    }
}

bool arken::compress::zip::decompress(const char * archive, const char * output)
{

    // TODO change arken::string
    string dirname;
    struct ::zip *za;
    struct zip_file *zf;
    struct zip_stat sb;
    char buf[100];
    int err;
    int i, len;
    int fd;
    unsigned long sum;

    if ((za = zip_open(archive, 0, &err)) == nullptr) {
        zip_error_to_str(buf, sizeof(buf), err, errno);
        fprintf(stderr, "can't open zip archive `%s': %s/n",
            archive, buf);
        return false;
    }

    for (i = 0; i < zip_get_num_entries(za, 0); i++) {
        if (zip_stat_index(za, i, 0, &sb) == 0) {
            len = strlen(sb.name);
            if (sb.name[len - 1] == '/') {
                if( dirname.empty() ) {
                  dirname = sb.name;
                }
                safe_create_dir(sb.name);
            } else {
                printf("decompress %s (%lu)\n", sb.name, sb.size);
                zf = zip_fopen_index(za, i, 0);
                if (!zf) {
                    fprintf(stderr, "boese, boese/n");
                    return false;
                }

                fd = open(sb.name, O_RDWR | O_TRUNC | O_CREAT , 0644);
                if (fd < 0) {
                    fprintf(stderr, "boese, boese/n");
                    return false;
                }

                sum = 0;
                while (sum != sb.size) {
                    len = zip_fread(zf, buf, 100);
                    if (len < 0) {
                        fprintf(stderr, "boese, boese\n");
                        exit(102);
                    }
                    write(fd, buf, len);
                    sum += len;
                }
                close(fd);
                zip_fclose(zf);
            }
        } else {
            printf("File[%s] Line[%d]\n", __FILE__, __LINE__);
        }
    }

    if (zip_close(za) == -1) {
        fprintf(stderr, "can't close zip archive `%s'\n", archive);
        return false;
    }

    if( output ) {
      //printf("rename %s => %s", dirname, output);
      rename(dirname, output);
    }

    //if( dirname ) {
      //std::cout << "limpando " << dirname;
      //delete[] dirname;
    //}

    return true;
}
