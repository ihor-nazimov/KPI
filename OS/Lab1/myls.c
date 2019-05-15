#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>
#include <string.h>

#include <grp.h>
#include <pwd.h>

#include <time.h>


int main(int argc, char* argv[])
{
    char* lspath;
    char deflspath[] = ".";
    //char* fullpath;
    DIR* curDir;
    struct dirent* dirstruct;
    struct stat ststat;
    char f_mode[] = "---------";
    char* lastcwd;
    char tmp[1024];
    struct group* grp;
    struct passwd* pwd;

    char* timebuf;

    //fullpath = malloc(1024);

    //get path or set to default
    if (argc > 1) {
        lspath = argv[1];
    }
    else lspath = deflspath;

    //save current directory
//    lastcwd = getcwd(NULL, 255);

    //open directory
    if (//chdir(lspath) | 
    (curDir = opendir(lspath))==NULL){
        fprintf(stderr, "Can't open directory %s, error: %m (%d) \n", lspath, errno);
        exit(-1);
    }
    else {
        //for each entry
        while ((dirstruct = readdir(curDir)) != NULL) {
        /*    fullpath = strcat(fullpath, lspath);

            fullpath = strcat(fullpath, "/");
            fullpath = strcat(fullpath, dirstruct->d_name);
        */
            //print attributes
            
            sprintf(tmp, "%s/%s", lspath, dirstruct->d_name);
            if (!lstat(tmp, &ststat)){
                //file type
                switch (ststat.st_mode & S_IFMT){
                    case S_IFSOCK: printf("s"); break;
                    case S_IFLNK:  printf("l"); break;
                    case S_IFREG:  printf("-"); break;
                    case S_IFBLK:  printf("b"); break;
                    case S_IFDIR:  printf("d"); break;
                    case S_IFCHR:  printf("c"); break;
                    case S_IFIFO:  printf("p"); break;
                    default:       printf("?");
                }
                //file modes
                memset(f_mode, '-', 9);
                if (ststat.st_mode & S_IRUSR) f_mode[0]='r';
                if (ststat.st_mode & S_IWUSR) f_mode[1]='w';
                if (ststat.st_mode & S_IXUSR) f_mode[2]='x';
                if (ststat.st_mode & S_IRGRP) f_mode[3]='r';
                if (ststat.st_mode & S_IWGRP) f_mode[4]='w';
                if (ststat.st_mode & S_IXGRP) f_mode[5]='x';
                if (ststat.st_mode & S_IROTH) f_mode[6]='r';
                if (ststat.st_mode & S_IWOTH) f_mode[7]='w';
                if (ststat.st_mode & S_IXOTH) f_mode[8]='x';
                printf("%s ", f_mode);

                printf("%-2d ", ststat.st_nlink); //hard links

                //user name
                pwd = getpwuid(ststat.st_uid);
                if (pwd) {
                    printf("%-5s ", pwd->pw_name);
                } else
                {
                    printf("%-5d ", ststat.st_uid);
                }
                
                //group name
                grp = getgrgid(ststat.st_gid);
                if (grp) {
                    printf("%-5s ", grp->gr_name);    
                } else
                {
                    printf("%-5d ", ststat.st_gid);
                }
                
                printf("%10li ", ststat.st_size); //size in bytes

                //last change time
                //timebuf = ctime(&ststat.st_ctime);
                //timebuf[strlen(timebuf)-1] = '\0'; 
                printf("%.24s ", ctime(&ststat.st_ctime));

                printf("%8li ", dirstruct->d_ino); //index
                //printf("%8li ", ststat.st_ino);
                
                printf("%s \n", dirstruct->d_name); //name
            }
            else {
                fprintf(stderr, "Can't get attributes of %s, error: %m (%d)", dirstruct->d_name, errno);
                closedir(curDir);
                exit(-1);
            }

        }
    }

 //   chdir(lastcwd);
 //   closedir(curDir);

    return 0;
}