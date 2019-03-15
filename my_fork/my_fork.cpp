#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
int main(int argc, char **argv) {
	char op[6] = "*%/+-";
	srand(time(NULL));
	if(argc != 3) {
	   char error[28] = "usage: exec filename.txt N\n";
	   write(2, error, strlen(error));
	   exit(1);
	}
	int N = atoi(argv[2]);
	pid_t fk = fork();
	switch(fk) {
	     case 0:
		FILE *file;
		file = fopen(argv[1], "w");
		for(int i = 0; i < N; i++) {
		  int randNum1 = rand() % 10;
		  int randNum2 = rand() % 10;
		  int pickOp = rand() % 5;
		  fprintf(file, "%d %c %d\n", randNum1, op[pickOp], randNum2);
		}
		fclose(file);
		exit(1);
	     default:
		wait(NULL);
		FILE *fileRead;
		fileRead = fopen(argv[1], "r");	
		int op1 = 0;
		int op2 = 0;
		int res = 0;
		char operat;
		while(fscanf(fileRead, "%d %c %d", &op1, &operat, &op2) != EOF) {
			if(operat == '+') {
			    res = op1 + op2;
			}
			else if(operat == '-') {
			    res = op1 - op2;
			}
			else if(operat == '*') {
			    res = op1 * op2;
			}
			else if(operat == '/') {
			    if(op2 == 0) {
			    	res = -1;
			    }
			    else {    
			    	res = op1 / op2;
			    }
			}
			else if(operat == '%') { 
			    if(op2 == 0) {
			    	res = -1;
			    }
			    else {    
			    	res = op1 % op2;
			    }
			}
			printf("%d %c %d = %d\n", op1, operat, op2, res);
		}

	}
}
