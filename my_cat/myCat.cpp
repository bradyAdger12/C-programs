#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv) {
   FILE *file;
   char buffer[255];
   if(argc < 2) {

      //create buffer to write to
      char buf[255];

      //read user input from command line and 'echo'
      while(scanf("%c", buf) != EOF) {
        printf("%s", buf);
      }
   }
   else {
     for(int i = 1; i < argc; i++) {
        file = fopen(argv[i], "r");
        //check to see if file exists. If not, exit.
        if(file == NULL) {
 	    perror("failed"); 
            exit(1);
        }
        //read from file and write to buffer. print results
        while(fscanf(file, "%s", buffer) != EOF) {
            printf("%s ", buffer);
        }
        printf("\n");
        fclose(file);//close file after use
     }
  }
}
