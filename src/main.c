#include "main.h"

/*
DATE: 9 june 2025
AUTHOR: Mrinal Yadav

TODO:
	[] Validate e_indent {magic number}.
	[] Ensure e_type is ET_REL.	mfl works on relocatable excutable files.

*/

void print_usage(char *file){
	fprintf(stderr, "%s: missing file operand\n", file);
	fprintf(stderr, "try \'%s --help\' for more information\n", file);
}

void print_help(char *file){
	fprintf(stdout, "%s is a static linker, which is under construction.\n", file);
}


int main(int argc, char *argv[]){
	if(argc <= 1){
		print_usage(argv[0]);
		exit(1);
	}
	
	if(strcmp(argv[1], "--help") == 0) print_help(argv[0]);

	
	


}
