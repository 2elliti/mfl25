#ifndef SDL_SET
#define SDL_SET
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_CAPACITY 999999
#define STRING_LENGTH 1024
#define STRONG_STRENGTH 1
#define WEAK_STRENGTH 0

unsigned long hash(unsigned char *str){
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; 

    return hash % DEFAULT_CAPACITY;
}


struct node {
	char *symbol;
	struct node *next;
	struct node *prev;
};

typedef struct node node;

struct hash_set{
	node *node_table;
};

typedef struct hash_set hash_set;

// Will give pointer to set.
hash_set *init_set(){
	hash_set *set = (hash_set *)malloc(sizeof(hash_set));
	if(set == NULL){
		perror("Error while allocating memory");
		exit(1);
	}

	set->node_table = (node *)malloc(sizeof(node) * DEFAULT_CAPACITY);

	if(set->node_table == NULL){
		perror("Error While allocating memory");
		exit(1);
	}

	return set;
}

// Will insert inside the hash set.
ssize_t insert(hash_set *set, unsigned char * symbol){

	if(set == NULL || symbol == NULL){
		return -1;
	}

	// Get hash first.
	unsigned long hash_id = hash(symbol);

	// Now find the node.
	node *nd = (set->node_table+hash_id);

	

	// NOw we have a node.
	// Iterate through the node table.
	// need a better way of allocating memory. 
	while(nd != NULL){
		if(nd->symbol == NULL){
			// If symbol is null, means that no  data is there.
			// Just add symbol to it.
			nd->symbol = malloc(strlen(symbol)+1);
			strcpy(nd->symbol, symbol);
			return 1;
		}else{
			// If symbol is not null, means there is a symbol associated with this hash.
			if(strcmp(nd->symbol, symbol) == 0){
				//If it happens to be the same symbol,return 0
				return 0;
			}else{
				// If they are not equal Just move on.
				nd = nd->next;
			}
		}	

	}

	// If execution has reached to this point, this means that Whole linked  list was full.
	// Better choice would be to make new node and attach to the linked list.		
	
	node *new_node = (node *)malloc(sizeof(node));
	new_node->symbol = malloc(strlen(symbol)+1);
	
	strcpy(new_node->symbol, symbol);
	return 1;	// Return 1 for successfull

}


// Will find inside the hash set.
ssize_t find(hash_set *set, unsigned char *symbol){
	// For find it would return -1 if doesnt exist or 0 if  exist.
	
	if(set == NULL || symbol == NULL){
		return -1;
	}
	
	unsigned long hash_id = hash(symbol);
	node *nd = (set->node_table + hash_id);


	// Now  we need to parse this set to check whether it exists or not.
	
	while(nd != NULL){
		if(nd->symbol == NULL){
			nd = nd->next;
		}else{
			if(strcmp(nd->symbol, symbol) == 0){
				return 0;
			}else{
				return -1;
			}
		}
	}

	return -1;

}







#endif
