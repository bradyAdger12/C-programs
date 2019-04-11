#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define SIZE 255
int main(int argc, char **argv) { 
	int index = 0; 
	char buffer[20];	
	int status; 	
	while(1) {
		char **array = (char **) malloc(sizeof(char **) * 3);
		char command[SIZE];  
	        char *line;
		printf("MY_UNIX> ");
		fgets(command, SIZE, stdin);
		index = 0; //reset word index position
		line = strtok(command, " ");
		while(line != NULL) {
			array[index] = line;
			index++;
			line = strtok(NULL, " ");
			if(line == NULL) {
				char *lastWord = array[index - 1];
				lastWord[strlen(lastWord)-1] = '\0';
			}
		}
		array[index] = NULL;
		if(strcmp(array[0], "exit") == 0) {  
			exit(0);
		}	 
		switch(fork()) {
			case 0:
				if(strcmp(array[0], "") != 0 && strcmp(command, "exit") != 0) {
					execvp(array[0], array);
					printf("Error: Could not execute\n");	
				} 
				exit(1); //if exec fails, exit
			default:
				wait(NULL);   
				free(array);
		}   	
	} 
}



