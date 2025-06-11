#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <stdbool.h>

struct object_file{
	int fd;			// File descriptor for an objectfile.
	uint8_t *filename;		// stores name of a file.
	uint8_t *buff;		// Stores object data.
};

typedef struct object_file object_file;


