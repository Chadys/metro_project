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

inline void quit(){
    clean_mem();
    printf("\nBye !\n");
}


//enable the program to clean itself before a quit with ^⁻C
void ctrlC(int __attribute__ ((unused))sig){
    exit(EXIT_FAILURE);
}


// Count the lines of a file until the end or until an empty line
size_t count_lines(FILE * file){
    size_t s, size;
    char buffer [MAX_CHAR_READ];
    char end;

    s=0;
    while(fgets(buffer, MAX_CHAR_READ, file) && buffer[0] != '\n'){
        end=0;
        s++;
        if(buffer[MAX_CHAR_READ-1] == '\n')
            end=1;
        buffer[MAX_CHAR_READ-1] = '\0'; //to avoid undefined behaviour of strlen
        size=strlen(buffer);
        while(buffer[size-1] != '\n' && !end){
            if (!fgets(buffer, MAX_CHAR_READ, file)){
                perror("fgets");
                fprintf(stderr, "Unexpected end of parsing, line %lu\n", s);
                return 0;
            }
            end=0;
            if(buffer[MAX_CHAR_READ-1] == '\n')
                end=1;
            buffer[MAX_CHAR_READ-1] = '\0'; //to avoid undefined behaviour of strlen
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

// Return the number of digits of a number
inline unsigned int count_digits_number(unsigned int n){
    unsigned int digits = 0;
    
    while(n) {
        n /= 10;
        digits++;
    }
    return digits;
}

inline void print_error(const char* error){
    fprintf(stderr, "%s%sError : %s%s%s%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED), error, codeFromStyle(RESET));
}