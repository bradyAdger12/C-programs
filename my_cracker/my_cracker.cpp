#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <set>
#include <string.h>
#include <ctime>
using namespace std;
vector<char*> words;
char *password;
int numThreads = 0;

void *run(void *v) {
   long thread = (long) v; 
   while(1) {
	   char *word1 = words[rand() %  words.size()];
	   char *word2 = words[rand() %  words.size()]; 
	   char * passwordCheck = (char *) malloc(1+ strlen(word1)+ strlen(word2) );
	   strcpy(passwordCheck, word1);
	   strcat(passwordCheck, word2);
	   if(strcmp(password, passwordCheck) == 0) {
		   printf("Password cracked: %s", passwordCheck);
		   exit(0);
	   } 
	   free(passwordCheck);
   } 
}
int main(int argc, char **argv) {
    numThreads = atoi(argv[3]);
    pthread_t threads[numThreads];
    if(argc != 4) {
	fprintf(stderr, "%s\n", "usage: exec fileName seed N");
    }
    int seed = atoi(argv[2]);
    srand(seed);
    FILE *f; 
    char buffer[40];
    int size = 0;
    f = fopen(argv[1], "r");
    while(fscanf(f, "%s", buffer) != EOF) {
      char *word = strdup(buffer);
      word[strlen(word)] = '\0';
      words.push_back(word);
    }
    fclose(f);
    char *word1 = words[rand() % words.size()];
    char *word2 = words[rand() % words.size()]; 
	password = (char*) realloc(password, 1+(strlen(word1) + strlen(word2)));
	strcpy(password, word1);
	strcat(password, word2);
    printf("Password: %s\nlength: %d\n", password, strlen(password));
    for(int i =0; i < numThreads; i++) {
   	pthread_create(&threads[i], NULL, run, (void *) i);
    }
    for(int i =0; i < numThreads; i++) {
   	pthread_join(threads[i], NULL);
    }
	free(password);	
    
}
