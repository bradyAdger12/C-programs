#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


struct Node{
	size_t size;
	Node *prev;
	Node *next;
};

struct Node *head=NULL;
void * pb;
Node *appendNode;
size_t heapBeginning;
int NODESIZE=sizeof(Node);	
int BLOCKSIZE=1024;


Node * firstFit(size_t size){
	struct Node *next=head;
	while(1){
		if(next->size > size){
		printf("mmalloc: node at %lu has enough space(%lu)\n", (size_t) next - heapBeginning, next->size);
		if(next->next != NULL) {
			appendNode = next->next;
		}
			return next;
		}else if(next->next != NULL){
			next=next->next;
		}else if(next->next==NULL){
			printf("malloc: calling sbrk..\n");
			Node *node;
			void *ptr;
			ptr = sbrk(BLOCKSIZE);
			node = (Node*) ptr;
			return node;
		}
	}
}

Node *addToFreeList(size_t size, void *startingAddress, size_t space) {
	Node *freeNode;
	Node *next = head;

	if(head == NULL) {
	    freeNode = (Node*) startingAddress;
	    freeNode->size = space - NODESIZE;
            freeNode->prev = NULL;
	    freeNode->next = NULL;
	    head = freeNode;
	    printf("mmalloc: node at %lu has enough space(%lu)\n", 0, 1024 - NODESIZE);
	    return head;         
  	}

	//add note to free list
	freeNode = (Node*) startingAddress;
	freeNode->size = space;
	freeNode->prev = NULL;
	freeNode->next = NULL; 
	head = freeNode;
	return freeNode;       
}       



void freeList() {
	printf("=== FREE LIST ===\n");
        Node *next = head;
	ssize_t nextNode = -1;
	ssize_t prevNode = -1;
       	while(next != NULL) {
		if(next->next != NULL) {
			nextNode = (size_t) next->next - heapBeginning;
		}
		else {
			nextNode = -1;
		}
		if(next->prev != NULL) {
			prevNode = (size_t) next->prev - heapBeginning;
		}
		else {
			prevNode = -1;
		}
	        printf("%lu: (%ld, %ld, %lu)\n", (size_t) next - heapBeginning, prevNode, nextNode, next->size);
		next = next->next;
        }	
	
	printf("=== END LIST ===\n");
}

void * mmalloc(size_t size){
	struct Node *allocation = NULL;
	struct Node *freeListNode = NULL;
	struct Node *foundNode = NULL;
	struct Node *getPrevNode = NULL;
	size_t leftOverSpace;
	size_t addressArithmetic;
	void *returnAddress;
	if(head==NULL){
		if(size >= BLOCKSIZE - NODESIZE) {
			printf("calling sbrk...\n");
			BLOCKSIZE = BLOCKSIZE * 2;
			pb = sbrk(BLOCKSIZE);
		}
		else {
			pb=sbrk(BLOCKSIZE);
		}
		heapBeginning = (size_t) pb;
		allocation = (Node*) pb;
		allocation->size = size;
		allocation->prev = NULL;
		allocation->next = NULL;
		printf("mmalloc: requesting %lu bytes\n", size);
		leftOverSpace = BLOCKSIZE - (allocation->size + (NODESIZE)); 
	        addressArithmetic = (size_t) allocation + NODESIZE;
	      	returnAddress = (void*) addressArithmetic;
		freeListNode = addToFreeList(size, (void*) (addressArithmetic + size), leftOverSpace);
     		printf("mmalloc: splitting %lu (%lu) into %lu (%lu) and %lu (%lu)\n",(size_t) allocation - heapBeginning, 1024-NODESIZE,
     		(size_t) allocation - heapBeginning, allocation->size, (size_t) freeListNode - heapBeginning, freeListNode->size);
        freeList();
     return returnAddress;
        }

	else {
	   printf("mmalloc: requesting %lu bytes\n", size);
	   foundNode = firstFit(size);
	   printf("malloc: splitting %lu (%lu)", (size_t) foundNode - heapBeginning, foundNode->size);
	   Node *tempNode;
	   if(foundNode->next != NULL) {
		   tempNode = foundNode->next;
	   }
	   leftOverSpace = foundNode->size - (size + (NODESIZE));
	   allocation = foundNode;
	   allocation->size = size;
	   allocation->prev = NULL;
	   allocation->next = NULL;
	   addressArithmetic = (size_t) allocation + NODESIZE;
	   returnAddress = (void*) addressArithmetic;
	   printf(" into %lu (%lu)", (size_t) allocation - heapBeginning, allocation->size);
	   freeListNode = addToFreeList(size, (void*) (addressArithmetic + allocation->size), leftOverSpace);
	   printf(" and %lu (%lu)\n", (size_t) freeListNode - heapBeginning, freeListNode->size);
	   if(appendNode != NULL) {
	   	freeListNode->next = appendNode;
	   }
	}  
        freeList();
     return returnAddress;
}

void mfree(void *freedNode){
     Node *next = head;
     size_t pointToBeginning = (size_t) freedNode - NODESIZE;
     Node *node = (Node*) pointToBeginning;
     while(next->next != NULL) {
     	next = next->next;
     }
     printf("mfree: node %lu (size %lu)\n", (size_t) node-heapBeginning, node->size);
     next->next = node;
     node->prev = next;
     freeList();
}

int main() {
	printf("main: testing perfect match...\n");
	void *ptr;
	void *ptr2;
	ptr = mmalloc(1000);
	printf("main: testing imperfect match...\n");
	mmalloc(20);
	printf("main: freeing perfect match...\n");
	mfree(ptr);
	ptr2 = mmalloc(45);
	printf("main: freeing memory...\n");
	mfree(ptr2);
}
