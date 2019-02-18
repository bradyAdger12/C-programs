#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

void archiveFile(const char* file, FILE *mtar) {
   FILE *rFile;

	//declare struct
	struct stat statbuf, *sbuf;
	int fileStat = stat(file, &statbuf);
   sbuf = &statbuf;

	//initialize buffer
	char *buffer = (char*) malloc(statbuf.st_size);

	//open file and read contents into buffer
   rFile = fopen(file, "r");
	fread(buffer, 1, statbuf.st_size, rFile);
   
   //date variables
	time_t dateTime;
   struct tm ts; //converted timestamp

	//check to see if regular file
   if(fileStat == -1 || !S_ISREG(statbuf.st_mode)) {
	 	perror("ERROR");
		exit(1);	
	}
   
	//write out statbuf, filename, and file contents to .mtar file
	fwrite(sbuf, sizeof(struct stat), 1, mtar);
   fwrite(file, 1, statbuf.st_size, mtar);
	fwrite(buffer, 1, statbuf.st_size, mtar);
   printf("Archiving %s: Bytes(%ld)\n", file, statbuf.st_size);

	fclose(rFile);
   
	//convert unix time to regular time
   //converstion taken from www.epochconverter.com/programming/c
 	//time(&dateTime);
   //ts = *localtime(&dateTime);
	//strftime(tbuf, sizeof(tbuf), "%b %d %H:%M", &ts);
}   

void readArchive(FILE *mtar) {
   struct stat statbuf;
   int statbuffSize = 144; //statbuffs are 144 bytes
   long int numFiles = 0; 
   long int numBytes = 0;

   fseek(mtar, 0, SEEK_END); //used to capture number of bytes in tar file
   numFiles = ftell(mtar) / statbuffSize; //used to get number of files
	fseek(mtar, 0, SEEK_SET);// place pointer at beginning of file
	for(int i = 0; i < numFiles; i++) {

		//read from .mtar to gather filename,numbytes,and contents
		fread(&statbuf, sizeof(struct stat), 1, mtar);
		printf("File Size: %ld\n", statbuf.st_size);
      char *fileName = (char*) malloc(statbuf.st_size); //allocate buf
      fread(fileName, 1, statbuf.st_size, mtar);
		printf("File Name: %s\n", buffer);
      char *contents = (char*) malloc(statbuf.st_size); //allocate contents
		fread(contents, 1, statbuf.st_size, mtar);
		printf("File Contents: %s\n", contents);

		free(contents); //deallocate contents
      free(fileName); //deallocate fileName
	}
}
	





int main(int argc, char **argv) {
	FILE *tarFile;
   FILE *tarWFile;

   //check to see if no arguments(after executable)
	if(argc < 3) {
      char buffer[255] = "Usage: -a file.mtar file1 [file2...]\nUsage: -x file.mtar\n";
		write(2, buffer, strlen(buffer));
      exit(1);
	}
  
   //if argument equals archive
	if(strcmp(argv[1], "-a") == 0) {
		if(argc < 4) {
			char buffer[255] = "Need at least 1 file(ex. file.txt)\n";
			write(2, buffer, strlen(buffer));
		}
      
 		//open tar file for appending
		tarFile = fopen(argv[2], "w");
  	   if(tarFile == NULL) {
			perror("ERROR");
			exit(1);
   	}
		for(int i = 3; i < argc; i++) {
			archiveFile(argv[i], tarFile);
		}
		fclose(tarFile);
	}


   //if command argument equals x
	if(strcmp(argv[1], "-x") == 0) {

		//handle incorrect arguments
		if(argc != 3) {
			char buffer[255] = "Incorrect number of arguments\n";
			write(2, buffer, strlen(buffer));
      }

		//open file for reading
		tarWFile = fopen(argv[2], "r");
  	   if(tarWFile == NULL) {
			perror("ERROR");
			exit(1);
		}
	   readArchive(tarWFile);
	}
}



