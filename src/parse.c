#include "parse.h"

void par_sym_tab(object_file **module){
	for(int curr = 0; module[curr] != NULL; curr++){
		object_file *obj = module[curr];
		for(int shnum = 0; shnum < obj->ehdr->e_shnum; shnum++){
			if(obj->shdr[shnum].sh_type == SHT_SYMTAB){
				obj->strtab = (char *)&obj->buff[obj->shdr[obj->shdr[shnum].sh_link].sh_offset];
				obj->symtab = (Elf64_Sym *)&obj->buff[obj->shdr[shnum].sh_offset];
				for(int curr_sym = 0; curr_sym < obj->shdr[shnum].sh_size/sizeof(Elf64_Sym); curr_sym++){
					char *symbol = &obj->strtab[obj->symtab[curr_sym].st_name];
					printf("Symbolfound: %s \n", symbol);
				}
					
			}
		}

	}

}
