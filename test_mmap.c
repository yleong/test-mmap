#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

void handle_err(int err);
int get_filesize(int fd);
int open_file(char* path, int flags);
void togglecase_file(char* fp, int filesize);
int main(){
    int fd = open_file("/home/notroot/test_mmap/somefile", O_RDWR);
    if(!fd) { return 1;}
    int filesize = get_filesize(fd);
    if(!filesize) { return 1;}
    char* fp = mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(MAP_FAILED == fp) { handle_err(errno); return 1;}
    togglecase_file(fp, filesize);
    munmap(fp, filesize);
    close(fd);
    return 0;
}
void handle_err(int err){
    printf("failed with errno: %d\n", err);
    switch(err){
        case EACCES:
            printf("access denied\n");
            break;
        case ENFILE:
            printf("max number of open files reached\n");
            break;
        default:
            printf("some other kind of error happened\n");
    }
}
int get_filesize(int fd){
    struct stat buf;
    if (-1 == fstat(fd, &buf)) { handle_err(errno); }
    return buf.st_size;
}
int open_file(char* path, int flags){
    int fd = open(path, flags);
    if(-1 == fd) { 
        int errsv = errno;
        handle_err(errsv);
    }
    return fd;
}
void togglecase_file(char* fp, int filesize){
    for(int i = 0; i < filesize; i++){
        if (isupper(fp[i])){
            fp[i] = tolower(fp[i]);
        } else if( islower(fp[i])){
            fp[i] = toupper(fp[i]);
        }
    }
}
