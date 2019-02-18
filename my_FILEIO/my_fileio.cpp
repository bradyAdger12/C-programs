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
      mFILE *pf = (mFILE*) malloc(sizeof(mFILE*)); //declare struct and pointer to struct

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
    ssize_t r;
	 r = read(fd, ptr, numBytes);
    if(r == -1) {
			perror("ERROR");
	 }
    return r;
  
};

//write to file
int mfwrite(void *ptr, size_t size, size_t nitems, mFILE *fptr) {
    size_t w;
	  int fd = fptr->fd;
  
    //check size of data type entered
    if(size == 1) {
		//allocate bytes for buffer based on a char* casted ptr
      char *buffer = (char*) malloc(size * strlen((char*)ptr));
      
      //have buffer point at casted ptr
      buffer = (char*) ptr;
 
		w = write(fd, buffer, strlen(buffer)); //write bytes out to disk
    }

	 if(w == -1) {
		perror("ERROR");
	 } 
   
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
     int bytesWritten;
     char *buffer = (char*) malloc(sizeof(char*)*255);

		if(argc > 1) {
			printf("%s", "Too many arguments\n");
			exit(1);
		}
  
     //example of "w"
     strcpy(buffer, "Hello World");
     file = mfopen("a.txt", "w");
     bytesWritten = mfwrite(buffer, 1, 255, file); 
	  printf("Bytes written to file a.txt: %d\n", bytesWritten);
     mfclose(file);

		//example of "w+"
	  strcpy(buffer, "this is a newly created file");	
     char buffer1[255];
     file = mfopen("b.txt", "w+");
     bytesWritten = mfwrite(buffer, 1, 255, file);
	  printf("Bytes written to file b.txt: %d\n", bytesWritten);
     lseek(file->fd, 0, SEEK_SET); //set pointer to beginning of file
     bytesRead = mfread(buffer1, 1, 255, file);
     printf("Bytes read from file b.txt: %d\n", bytesRead);
     mfclose(file); 

     //example of "r"
     char buffer2[255]; 
     file = mfopen("a.txt", "r"); 
     bytesRead = mfread(buffer2, 1,255, file);
     printf("Bytes read from file a.txt: %d\n", bytesRead); 
     mfclose(file);
     
     //example of "r+"
     strcpy(buffer, "this is a test");
     char buffer3[255];
	  file = mfopen("c.txt", "r+");
     bytesWritten = mfwrite(buffer, 1, 255, file);
	  if(bytesWritten == -1) {
			perror("Directory must contain file 'c.txt'");	
	  }
	  printf("Bytes written to file c.txt: %d\n", bytesWritten);
     lseek(file->fd, 0, SEEK_SET); //set pointer to beginning of file
	  bytesRead = mfread(buffer3, 1, 255, file);
     printf("Bytes read from file c.txt: %d\n", bytesRead);
	  mfclose(file); 
     
     //example of "a"
     file = mfopen("a.txt", "a");
     char buffer4[255] = "This is a program";
     bytesWritten = mfwrite(buffer4, 1, 255, file);
	  printf("Bytes written to file a.txt: %d\n", bytesWritten);
     mfclose(file);

     //example of "a+"
     file = mfopen("a.txt", "a+");
     strcpy(buffer, "for demonstration purposes");
     char buffer5[255]; 
     bytesWritten = mfwrite(buffer, 1, 255, file);
	  printf("Bytes written to file a.txt: %d\n", bytesWritten);
     lseek(file->fd, 0, SEEK_SET); //set pointer to beginning of file
     bytesRead = mfread(buffer5, 1, 255, file);
     printf("Bytes read from file a.txt: %d\n", bytesRead); 
     mfclose(file);
}  
