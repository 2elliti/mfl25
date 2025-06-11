#include "main.h"

/*
DATE: 9 june 2025
AUTHOR: Mrinal Yadav

TODO:
	[] Validate e_indent {magic number}.
	[] Ensure e_type is ET_REL.	mfl works on relocatable excutable files.

*/

void print_usage(uint8_t *file){
	fprintf(stderr, "%s: missing file operand\n", file);
	fprintf(stderr, "try \'%s --help\' for more information\n", file);
}

void print_help(uint8_t *file){
	fprintf(stdout, "%s is a static linker, which is under construction.\n", file);
}

object_file *parse_obj_file(uint8_t *file){
	int fd;		// File descriptor
	off_t offset;	// File offset;
	object_file *obj = (object_file *)malloc(sizeof(object_file));	
	if((fd = open(file, O_RDONLY)) == -1 ){
		perror("open");
		exit(1);
	}	
	obj->fd = fd;
	
	if((offset = lseek(fd, 0, SEEK_END)) == -1){
		perror("lseek");
		exit(1);
	}
	
	if(lseek(fd, 0, SEEK_SET) == -1){
		perror("lseek");
		exit(1);
	}

	uint8_t *buff = malloc(sizeof(uint8_t) * (offset+2));
	
	if(read(fd,buff,offset) == -1){
		perror("read");
		exit(1);
	}	
	
	buff[offset+1] = '\0';	
	obj->buff = buff;

	return obj;	

}

bool validate_elf(object_file *obj){
	Elf64_Ehdr *hdr = (Elf64_Ehdr *)obj->buff;
	if(hdr->e_ident[0] != 0x7f){
		fprintf(stderr, "%s is not an elf format\n", obj->filename);
		return false;
	}
	if(hdr->e_type != ET_REL){
		fprintf(stderr,"%s is not relocatable object file\n", obj->filename);
		return false;
	}
	return true;
}

int main(int argc, uint8_t *argv[]){
	int obj_num = 0;			// Total number of object files

	if(argc <= 1){
		print_usage(argv[0]);
		exit(1);
	}
	
	if(strcmp(argv[1], "--help") == 0) print_help(argv[0]);

	object_file **modules = malloc(sizeof(object_file *) * (argc+1));
		
	if(modules == NULL){
		perror("malloc");
		exit(1);
	}	
	
	for(int curr = 0; curr < argc; curr++){
		if(strstr(argv[curr], ".o")){
			modules[obj_num] = parse_obj_file(argv[curr]);					
			obj_num++;
		}
	}
	
	modules[obj_num + 1] = NULL;
		
	for(int curr = 0; modules[curr] != NULL; curr++){
		if(validate_elf(modules[curr])){
			fprintf(stdout,"Object file %s: PASSED\n", modules[curr]->filename);
		}else{
			fprintf(stderr,"Object file %s: FAILE\n", modules[curr]->filename);
		}
	}	

}
