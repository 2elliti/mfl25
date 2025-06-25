#ifndef PARSE_H
#define PARSE_H
#include "main.h"

struct object_file{
	int fd;				// File descriptor for an objectfile.
	uint8_t *filename;		// stores name of a file. {path}
	uint8_t *buff;			// Stores object data.
	char * strtab;			// Contains offset to string table.
	Elf64_Ehdr *ehdr;		// Elf Header of elf.
	Elf64_Phdr *phdr;		// Elf program header.
	Elf64_Shdr *shdr;		// Elf section header.
	Elf64_Sym *symtab;		// Contains sym table.
};

typedef struct object_file object_file;

void par_sym_tab(object_file **);

#endif
