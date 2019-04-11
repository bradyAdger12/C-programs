#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
int getFileCount(FILE **file) {
	int fileCount = 0;
	while(file[fileCount] != NULL) {
		fileCount++;
	}
	return fileCount;
}
int main(int argc, char **argv) {
    char buffer[255];
    if(argc == 1) {
	while(fgets(buffer, 255, stdin) != NULL) {
		fprintf(stdout, "stdout: %s", buffer);	
		fprintf(stderr, "stderr: %s", buffer);	
	}
    }
    else {
	char *file;
        int fileCount = 0;
	bool isAppend = false;
	FILE **files = (FILE**) malloc(sizeof(FILE**) * argc-1);
	char **permissions = (char**) malloc(sizeof(char**) * argc-1);
	

	//open all files and add dup2 to arguments requiring file redirection
	for(int k = 1; k < argc; k++) {
		if(strcmp(argv[k], "-append") == 0) {
			isAppend = true;
		}
	}
	for(int i = 1; i < argc; i++) {
		file = strchr(argv[i], '=');
		file++;
		if(strstr(argv[i], "in=") != NULL) {
			files[i-1] = fopen(file, "r");
			if(files[i-1] == NULL) {
			  printf("Cant open file: %s\n", file);
			  exit(1);
			}
			permissions[i-1] = "r";
			
		}
		else if(strstr(argv[i], "out=") != NULL) {
			if(isAppend) {
			   files[i-1] = fopen(file, "a");
			   if(files[i-1] == NULL) {
			  	printf("Cant open file: %s\n", file);
			  	exit(1);
			   }
			   permissions[i-1] = "a"; //used as placeholder	
			}
			else {
			   files[i-1] = fopen(file, "w");
			   if(files[i-1] == NULL) {
			    	printf("Cant open file: %s\n", file);
			  	exit(1);
			   }
			   permissions[i-1] = "w"; //used as placeholder
			}

			dup2(fileno(files[i-1]), 1);
		}
		else if(strstr(argv[i], "err=") != NULL) {
			if(isAppend) {
                          files[i-1] = fopen(file, "a");
			  if(files[i-1] == NULL) {
			  	printf("Cant open file: %s\n", file);
			  	exit(1);
			  }
			  permissions[i-1] = "a"; //used as placeholder	
			}
			else {
                          files[i-1] = fopen(file, "w");
			  if(files[i-1] == NULL) {
			  	printf("Cant open file: %s\n", file);
			  	exit(1);
			  }
			  permissions[i-1] = "w"; //used as placeholder
			}

			dup2(fileno(files[i-1]), 2);
                }
	}
	
	fileCount = getFileCount(files);
	for(int i = 0; i < fileCount; i++) {
		if(strcmp(permissions[i], "r") == 0) {
			while(fgets(buffer, 255, files[i]) != NULL) {
				fprintf(stdout, "stdout: %s", buffer);	
				fprintf(stderr, "stderr: %s", buffer);	
			}
		}
		fclose(files[i]);	
	}

	free(files);
	free(permissions);
    }    	
}
