#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <sys/wait.h>
int main(int argc, char **argv) { 
    int **pdList = (int**) malloc(sizeof(int**) * argc-2);
	char **argList;
	char *line;  
	int numPipes = 0; 
    for(int k = 0; k < argc-2; k++) {
		int pd[2];
		numPipes++;
		pdList[k] = pd;
		pipe(pdList[k]);
    }
	printf("Number of Pipes: %d\n", numPipes);
    for(int i = 1; i < argc; i++) { 
	  argList = (char**) malloc(sizeof(char**) * 10); //holds arguments
	  line = strtok(argv[i], "_"); 
	  int index = 0;
	  while(line != NULL) {
		argList[index] = line;
		index++;
		line = strtok(NULL, "_"); 
		if(line == NULL) {
			argList[index] = NULL;
		}
	  } 	  
	  switch(fork()) {
		case 0: 
			if(i == 1) {  
			    if(numPipes == 0) {
					execvp(argList[0], argList);
				}
				else {
				printf("beginning of pipe\n");
					if(numPipes > 1) {
						for(int d = i; d < numPipes; d++) {
							close(pdList[d][0]); 
							close(pdList[d][1]);
						} 
					}
				close(pdList[0][0]);
				dup2(pdList[0][1],1);
				execvp(argList[0], argList);
				}	
			}
			else if(i > 1 && i < argc - 1 && argc > 3) {
				int leftPipe = i-2;
				int rightPipe = i-1;
				for(int m = 0; m < numPipes; m++) {
					if(m != leftPipe && m != rightPipe) {
						close(pdList[m][0]); 
						close(pdList[m][1]);
					}
				} 
				close(pdList[leftPipe][1]);
				close(pdList[rightPipe][0]);
				dup2(pdList[leftPipe][0],0);
				dup2(pdList[rightPipe][1],1);		
				execvp(argList[0], argList);
			}	

			else {
				printf("end pipe\n");
				for(int d = 0; d < numPipes-1; d++) {
						close(pdList[d][1]);
						close(pdList[d][0]);
				}
				close(pdList[numPipes-1][1]);
				dup2(pdList[numPipes-1][0],0);   
				execvp(argList[0], argList);
			}
		}	
		free(argList);
	  } 
	  
	  for(int k = 0; k < numPipes; k++) {
			close(pdList[k][1]);
			close(pdList[k][0]);
	  }
      for(int i = 0; i < argc - 1; i++) {
		  wait(NULL);
	  }
		
}

        

