/*
TODO:
	1. Add hash collision checking and mitigation.
	2. Impliment chained links! Each member oh hash table will point to head of linked list.
	3. Will make easier to mitigate the hash collision.	
*/


#ifndef SDL_SET
#define SDL_SET
#include<string.h>

#define DEFAULT_CAPACITY 999999999
#define STRING_LENGTH 1024
#define STRONG_STRENGTH 1
#define WEAK_STRENGTH 0


unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % DEFAULT_CAPACITY;
}

struct mset {
	size_t size;
	char **symbols;
};

typedef struct mset mset; 

mset *m_set_init(){
	mset *set = (mset *) malloc(sizeof(mset));
	set->symbols = (char **) malloc(sizeof(char *) * DEFAULT_CAPACITY);
}

ssize_t m_set_add(mset *set, char *symbol){
	if(symbol == NULL){
		fprintf(stderr, "Null symbol detected! Exiting...\n");
		exit(1);
	}
	
	unsigned long symbol_id = hash(symbol);

	if(set->symbols[symbol_id] != NULL) return strcmp(set->symbols[symbol_id], symbol) == 0 ? 0 : -1;
	size_t symbol_len = strlen(symbol);
	set->symbols[symbol_id] = (char *)malloc(sizeof(char) * STRING_LENGTH);
	strcpy(set->symbols[symbol_id], symbol);
	return 0;
}

ssize_t m_set_find(mset *set, char *symbol){
	if(symbol == NULL){
		fprintf(stderr, "Null symbol detected! Exiting...\n");
		exit(1);
	}

	unsigned long symbol_id = hash(symbol);
	
	if(set->symbols[symbol_id] == NULL) return 1;

	return strcmp(set->symbols[symbol_id], symbol) == 0 ? 0 : -1;
}


#endif
