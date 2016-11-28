#include "header.h"


memory *garbage_collector = NULL;


void *getmem(size_t nitems, size_t size){
	memory *mem;

	mem = malloc(sizeof(memory));
	if (!mem){
		perror("malloc");
		return NULL;
	}
	mem->adr = malloc(nitems*size);
	if (!mem->adr){
		perror("malloc");
		free(mem);
		return NULL;
	}
	mem->next = garbage_collector;
	garbage_collector=mem;
	return mem->adr;
}

void clean_mem(){
	memory *mem;

	while(garbage_collector){
		free(garbage_collector->adr);
		mem=garbage_collector;
		garbage_collector = garbage_collector->next;
		free(mem);
	}
}

//enable the program to clean itself before a quit with ^⁻C
void ctrlC(int __attribute__ ((unused))sig){
	exit(EXIT_FAILURE);
}


// Count the lines of a file until the end or until an empty line
size_t count_lines(FILE * file){
	size_t s, size;
	char buffer [MAX_CHAR_READ];

	s=0;
	while(fgets(buffer, MAX_CHAR_READ, file) && buffer[0] != '\n'){
		s++;
		size=strlen(buffer);
		while(buffer[size-1] != '\n'){
			if (!fgets(buffer, MAX_CHAR_READ, file)){
				perror("fgets");
				fprintf(stderr, "Unexpected end of parsing, line %lu\n", s);
				return 0;
			}
			size=strlen(buffer);
		}
	}
	return s;
}

// Get the ordinal suffix of a number
char * ordinal_suffix(unsigned int n){
	unsigned int unit = n%10, ten = (n/10)%100;
  
	if (ten == 1)
		return "th";
	switch (unit){
		case 1:
			return "st";
		case 2:
			return "nd";
		case 3:
			return "rd";
		default:
			return "th";
	}
}