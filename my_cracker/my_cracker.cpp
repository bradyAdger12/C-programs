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
set<char*> takenWords;
char *password;
int numThreads = 0;
bool go = true;

void *run(void *v) {
   long thread = (long) v; 
   while(go) {
	   char *word1 = words[rand() %  words.size()];
	   char *word2 = words[rand() %  words.size()]; 
	   char * passwordCheck = (char *) malloc(1+ strlen(word1)+ strlen(word2));
 	   if(passwordCheck == NULL) {
 		fprintf(stderr, "%s\n", "failed to allocate");
	   }
	   strcpy(passwordCheck, word1);
	   strcat(passwordCheck, word2);
           const bool is_in = takenWords.find(passwordCheck) != takenWords.end(); 
	   if(is_in) {
		continue;	
	   }     
	   else {
	     takenWords.insert(passwordCheck);	
	   }
	   if(strcmp(password, passwordCheck) == 0) {
		printf("Thread: %d -> Password cracked: %s\n", thread, passwordCheck);
		go = false;
	     } 	
	   free(passwordCheck);
  	   }
} 
int main(int argc, char **argv) {
    numThreads = atoi(argv[3]);
    int createThread = 0;
    pthread_t *threads = (pthread_t*) malloc(sizeof(pthread_t) * numThreads);
    if(threads == NULL) {
    	fprintf(stderr, "%s\n", "failed to allocate"); 
    }
    if(argc != 4) {
	fprintf(stderr, "%s\n", "usage: exec fileName seed N");
    }
    int seed = atoi(argv[2]);
    srand(seed);
    FILE *f; 
    char buffer[40];
    int size = 0;
    f = fopen(argv[1], "r");
    if(f == NULL) {
	perror("FILE");
	exit(1);	
    }
    while(fscanf(f, "%s", buffer) != EOF) {
      char *word = strdup(buffer);
      word[strlen(word)] = '\0';
      words.push_back(word);
    }
    fclose(f);
    char *word1 = words[rand() % words.size()];
    char *word2 = words[rand() % words.size()]; 
    password = (char*) malloc(1+(strlen(word1) + strlen(word2)));
    if(password == NULL) {
  	fprintf(stderr, "%s\n", "failed to allocate");
    }
    strcpy(password, word1);
    strcat(password, word2);
    printf("Password: %s\n", password);
    for(int i =0; i < numThreads; i++) {
   	if(pthread_create(&threads[i], NULL, run, (void *) i)) {
           fprintf(stderr, "%s\n", "Thread did not successfully spawn");
	}	
    }
    for(int i =0; i < numThreads; i++) {
   	if(pthread_join(threads[i], NULL) != 0) {	
           fprintf(stderr, "%s\n", "Thread did not successfully join");
	}
    }
	free(password);	
}
