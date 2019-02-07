#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct {
   int fd; 
} mFILE;

mFILE *mfopen(const char* filename, const char *mode) {
      int flag; //will hold file permissions
			int fd; //file descriptor
      int mode_t; 
      mFILE f, *pf; //declare struct and pointer to struct
      pf = &f;

      //open file for read only
		  if(strcmp(mode, "r") == 0) {
 			 	flag = O_RDONLY;
        mode_t = S_IRUSR;
			}

			//open file for reading and writing
      else if(strcmp(mode, "r+") == 0) {
				flag = O_RDWR;
        mode_t = S_IRUSR | S_IWUSR;
			}
      
      //open file for writing only
      else if(strcmp(mode, "w") == 0) {
			  flag = O_WRONLY | O_CREAT | O_TRUNC;
        mode_t = S_IWUSR;
			}
      
      //open for reading and writing
      else if(strcmp(mode, "w+") == 0) {
				flag = O_RDWR | O_CREAT;
        mode_t = S_IWUSR | S_IRUSR;
			}
      
			//open for appending
			else if(strcmp(mode, "a") == 0) {
					flag = O_WRONLY | O_CREAT | O_APPEND;
					mode_t = S_IWUSR;
			}
      
      //open for reading and appending
      else if(strcmp(mode, "a+") == 0) {
					flag = O_RDWR | O_CREAT | O_APPEND;
					mode_t = S_IWUSR | S_IRUSR;
			}

      //open file and check if present      
      if(fd < 0) {
 					return NULL;
			}
      fd = open(filename, flag, mode_t);
      //set structs file descriptor to open files descriptor
      pf->fd = fd; //mFile struct variable points at file descriptor
		  return pf;	 
};

//write to file
int mfread(void *ptr, size_t size, size_t nitems, mFILE *fptr) {
    ssize_t numBytes = size * nitems;
    int fd = fptr->fd;
		ssize_t r = read(fd, ptr, numBytes);
    return r;
  
};

//write to file
int mfwrite(void *ptr, size_t size, size_t nitems, mFILE *fptr) {
		ssize_t numBytes = size *nitems;
		int fd = fptr->fd;
		ssize_t w = write(fd, ptr, numBytes);
		return w;
}

//close file
int mfclose(mFILE *fptr) {
    int fd = fptr->fd;
		int c = close(fd);
    if(c == 0) {
			return 0;
		}
    return -1;
}



int main(int argc, char **argv) {
     mFILE *file;
     int bytesRead;

     //example of "w"
     char buffer[20] = "Hello World";
     file = mfopen("a.txt", "w");
     mfwrite(buffer, 1, 20, file); 
     mfclose(file);

		//example of "w+"
	   char buffer2[35] = "this is a newly created file";	
     char buffer3[35];
     file = mfopen("b.txt", "w+");
     mfwrite(buffer2, 1, 35, file);
     lseek(file->fd, 0, SEEK_SET); //set pointer to beginning of file
     bytesRead = mfread(buffer3, 1, 35, file);
     printf("Bytes read from file b.txt: %d\n", bytesRead);
     mfclose(file); 

     //example of "r"
     char buffer4[20]; 
     file = mfopen("a.txt", "r"); 
     bytesRead = mfread(buffer4, 1,20, file);
     printf("Bytes read from file a.txt: %d\n", bytesRead); 
     mfclose(file);
     
     //example of "r+"
     char buffer5[20] = "this is a test";
     char buffer6[20];
		 file = mfopen("c.txt", "r+");
     mfwrite(buffer5, 1, 20, file);
     lseek(file->fd, 0, SEEK_SET); //set pointer to beginning of file
	   bytesRead = mfread(buffer6, 1, 20, file);
     printf("Bytes read from file c.txt: %d\n", bytesRead);
		 mfclose(file); 
     
     //example of "a"
     file = mfopen("a.txt", "a");
     char buffer7[20] = "This is a program";
     mfwrite(buffer7, 1, 20, file);
     mfclose(file);

     //example of "a+"
     file = mfopen("a.txt", "a+");
     char buffer8[30] = "for demonstration purposes";
     char buffer9[50]; 
     mfwrite(buffer8, 1, 30, file);
     lseek(file->fd, 0, SEEK_SET); //set pointer to beginning of file
     bytesRead = mfread(buffer9, 1, 50, file);
     printf("Bytes read from file a.txt: %d\n", bytesRead); 
     mfclose(file);
}  
