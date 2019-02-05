#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct {
   int fd; 
} mFile;

mFile *mfopen(const char* filename, const char *mode) {
      int filePerm; //will hold file permissions
			int fd;
      mFile f, *pf;
      pf = &f;
		  if(strcmp(mode, "r") == 0) {
 			 	filePerm = O_RDONLY;
			}
      else if(strcmp(mode, "w") == 0) {
			  filePerm = O_WRONLY;
			}     
      fd = open(filename, filePerm);
      if(fd < 0) {
 					return NULL;
			}
      pf->fd = fd; //mFile struct variable points at file descriptor
		  return pf;	 
};

int mfread(void *ptr, size_t size, size_t nitems, mFile *fptr) {
    ssize_t numBytes = size * nitems;
    int fd = fptr->fd;
		ssize_t r = read(fd, ptr, numBytes);
    return r;
  
};

int mfwrite(void *ptr, size_t size, size_t nitems, mFile *fptr) {
		ssize_t numBytes = size *nitems;
		int fd = fptr->fd;
		ssize_t w = write(fd, ptr, numBytes);
		return w;
}

int mfclose(mFile *fptr) {
    int fd = fptr->fd;
		int c = close(fd);
    if(c == 0) {
			return 0;
		}
    return -1;
}


int main(int argc, char **argv) {
      mFile *file;
      file = mfopen(argv[1], "r");
      int write = mfwrite(buffer, 1, 12, file);
      mfclose(file);
}
