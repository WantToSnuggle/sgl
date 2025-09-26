#include "lm_cmd.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#if ( __linux__)
#include <sys/types.h>
#include <unistd.h>
#endif


static int lm_rmfile(const char *path) {
    return remove(path);
}


int lm_rm(const char *dir_name)
{
    DIR *dir;
    struct dirent *entry;
    char path[1024];
    struct stat path_stat;

    if (stat(dir_name, &path_stat) == 0) {
        if (S_ISREG(path_stat.st_mode)) {
            return lm_rmfile(dir_name);
        }
    }
    else {
        return 0;
    }

    dir = opendir(dir_name);
    if (!dir) {
        return -1;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; 
        }
        
        snprintf(path, sizeof(path), "%s/%s", dir_name, entry->d_name);
        lm_rmfile(path);
    }
    closedir(dir);
    
    if (rmdir(dir_name) != 0) {
        return -1;
    }
    
    return 0;
}


int lm_mkdir(const char *dir_name)
{
#if (_WIN32)
    return mkdir(dir_name);
#elif ( __linux__)
    return mkdir(dir_name, 0777);
#endif
}


#define BUFFER_SIZE (1024 * 128)  // 128 KB


int lm_copy_file(const char *source_path, const char *dest_path) 
{
    #if (_WIN32)
    #define    FSEEK  fseeko
    #define    FTELL  ftello
    #elif ( __linux__)
    #define    FSEEK  fseek
    #define    FTELL  ftell
    #endif
    
    if(source_path == NULL || dest_path == NULL) {
        LM_LOG_ERROR("missing destination path");
        return -1;
    }

    FILE *src = fopen(source_path, "rb");
    if (!src) {
        LM_LOG_ERROR("can not open file: %s\n", source_path);
        return -1;
    }

    FILE *dst = fopen(dest_path, "wb");
    if (!dst) {
        LM_LOG_ERROR("can not create file: %s\n", dest_path);
        fclose(src);
        return -1;
    }

    char *buffer = malloc(BUFFER_SIZE);
    if(!buffer) {
        LM_LOG_ERROR("can not allocate memory\n");
    }

    memset(buffer, 0, BUFFER_SIZE);

    size_t bytes_read;
    unsigned long long total_bytes = 0;

    FSEEK(src, 0, SEEK_END);
    off_t file_size = FTELL(src);
    rewind(src);

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, src)) > 0) {
        fwrite(buffer, 1, bytes_read, dst);
        total_bytes += bytes_read;

        if (file_size > 0 && total_bytes % (file_size / 100) == 0) {
            fflush(stdout);
        }
    }

    fclose(src);
    fclose(dst);
    free(buffer);
    return 0;
}


void lm_echo(const char *info)
{
    printf("%s\n", info);
}


void lm_echo_red(const char *info)
{
    printf("\x1b[31m%s\x1b[0m\n", info);
}


void lm_echo_green(const char *info)
{
    printf("\x1b[32m%s\x1b[0m\n", info);
}


void lm_echo_blue(const char *info)
{
    printf("\x1b[34m%s\x1b[0m\n", info);
}
