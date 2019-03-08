#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main(int argc, char **argv) {
	char op[6] = "*%/+-";
	int N = atoi(argv[2]);
	if(argc == 1) {
	   char error[28] = "usage: exec filename.txt N\n";
	   write(2, error, strlen(error));
	   exit(1);
	}
	pid_t fk = fork();
	switch(fk) {
	     case 0:
		FILE *file;
		file = fopen(argv[1], "w");
		for(int i = 0; i < N; i++) {
		  int randNum1 = rand() % 10;
		  int randNum2 = rand() % 10;
		  int pickOp = rand() % 5;
		  int res = 0;
		  if(op[pickOp] == '+') {
		       res = randNum1 + randNum2;
		  }
		  if(op[pickOp] == '-') {
		       res = randNum1 - randNum2;
		  }
		  if(op[pickOp] == '%') {
		       res = randNum1 & randNum2;
		  } 
		  if(op[pickOp] == '*') {
		       res = randNum1 * randNum2;
		  }
		  if(op[pickOp] == '/' && randNum2 != 0) {
		       float res2 = randNum1 / randNum2;
		       printf("%f", res2);
		       res = randNum1 / randNum2;
		  }
		  fprintf(file, "%d %c %d = %d\n", randNum1, op[pickOp], randNum2, res);
		}
		exit(1);
	     default:
		wait(NULL);	
		printf("Parent\n");
	}
}
