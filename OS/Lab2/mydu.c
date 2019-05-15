#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>
#include <string.h>


#define MAXPATH 1024

long int items=0;

unsigned long getDirSize(char* name) {
    DIR* curDir;
    struct dirent* dirstruct;
    struct stat ststat;
    unsigned long dirSize = 0;
    char* fullpath;

    fullpath = malloc(MAXPATH+2);
    //open directory
    if ((curDir = opendir(name))==NULL){
        fprintf(stderr, "Can't open directory %s, error: %m (%d) \n", name, errno);
        free(fullpath);
        return dirSize;
    }
    else {
        //for each entry
        while ((dirstruct = readdir(curDir)) != NULL) {
            items++;
        
            strncpy(fullpath, name, MAXPATH+1);
            strncat(fullpath, "/", MAXPATH+1);
            strncat(fullpath, dirstruct->d_name, MAXPATH+1);
            if (strlen(fullpath) >= MAXPATH) {
                fprintf(stderr, "File name exeed maximum length %d in\n%s\n", MAXPATH, fullpath);
                continue;
            }
            if (!lstat(fullpath, &ststat)){
                switch (ststat.st_mode & S_IFMT){
                    case S_IFREG:  
                        dirSize += ststat.st_size;
                        break;
                    case S_IFDIR:  
                        //skip current and parent directories
                        if ( strncmp(dirstruct->d_name, ".", MAXPATH) && 
                             strncmp(dirstruct->d_name, "..", MAXPATH) ) {
                                 dirSize += ststat.st_size;
                                 dirSize += getDirSize(fullpath);
                             }
                        break;
                    default:
                        dirSize += ststat.st_size;
                        
                }
            }
            else {
                fprintf(stderr, "Can't get attributes of %s, error: %m (%d)", dirstruct->d_name, errno);
                closedir(curDir);
                free(fullpath);
                return dirSize;
            }

        }    
    }
    closedir(curDir);
    free(fullpath);
    return dirSize;
}

int main(int argc, char* argv[])
{
    char* lspath;
    char deflspath[] = ".";
    char* lastcwd;

    long int dirSize = 0;
    char buf[1024];
    struct stat ststat;

    //get path or set to default
    if (argc > 1) {
        lspath = argv[1];
    }
    else lspath = deflspath;

    //save current directory
    lastcwd = getcwd(NULL, 255);

    if (lstat(lspath, &ststat)) {
        fprintf(stderr, "Can't get attributes of %s, error: %m (%d)", lspath, errno);
    };

    if ((ststat.st_mode & S_IFMT) == S_IFDIR) {
        printf("Directory: %s\n", lspath);
        printf("Overall size: %li\n", getDirSize(lspath));
        printf("Overall items: %li\n", items);
    } else {
        printf("File: %s\n", lspath);
        printf("Size: %li\n", ststat.st_size);
    }


    chdir(lastcwd);
 
    return 0;
}