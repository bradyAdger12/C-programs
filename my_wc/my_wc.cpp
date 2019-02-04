#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char **argv) {
    int twc = 0; //total word count
    int tlc = 0; //total line count
    long int tcc = 0; //total character count
    bool multFiles = false;
    if(argc < 2) {
       fprintf(stderr, "%s", "insufficient arguments\n");
       exit(1);
    }
    if(argc > 2) {
      multFiles = true;  //test to see if there are multiple files(used to determine format)
    }
    
    printf("%s", "wc = WORD COUNT lc = LINE COUNT cc = CHARACTER COUNT\n--------------------------\n");

    for(int i = 1; i < argc; i++) {
       FILE *file;
       char buffer[100];
       char *ptr;
       int wc = 0; //word count for file
       int lc = 0; //line count for file
       long int cc = 0; //character count for file
       file = fopen(argv[i], "r");
       if(file == NULL) {
			 		perror("ERROR");
					exit(1);
			 } 
       //while loop that looks at every string in file
       while(fgets(buffer, 100, file) != NULL) {
           ptr = strtok(buffer, " "); //points to a first string in line
	         lc++; //increment line count
           //while loop that looks at every character on a line
           while(ptr != NULL) {
                //strip off newlines
                if(strchr(ptr, '\n') != NULL) {
                   long int strip = strlen(ptr);
                   ptr[strip-1] = '\0';
                }
                cc+=strlen(ptr); //add characters from string to string count
 	            	wc++; //add entire word to word count 
                ptr = strtok(NULL, " ");    
           }
       }
       printf("wc: %d lc: %d cc: %ld FILE: %s\n", wc, lc, cc, argv[i]);
       if(multFiles) {
           twc += wc;
           tlc += lc;
           tcc += cc;
       }
       fclose(file);   
    }
    if(multFiles) {
      	 printf("twc: %d tlc: %d tcc: %ld\n", twc, tlc, tcc);
    }													

}
