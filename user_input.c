#include "header.h"


char get_valid_entry(char *);

RUN_MODE get_r_mode(){
    char c;
    
    printf("Welcome stranger, to this Tokyo Metro pathfinding !\n\nWould you like for this program to be in\nList of successors mode (enter 'l')\nMatrix mode (enter 'm')\nor to compare the performance of the two ? (enter 'c').\nYou can quit anytime by entering 'q'\n");

    c = get_valid_entry("lmcq");
    switch(c){
        case 'l':
           return SUCC_LIST;
        case 'm':
            return MATRIX;
        case 'c':
            return COMPARE;
        default:
            exit(EXIT_SUCCESS);
    }
}

SEARCH_MODE get_s_mode(){
    char c;
    
    printf("Now, would you like for this search to find the quickest way(s) possible (enter 'w') or those with the least transfers (enter 'c') ?\n");
    
    c = get_valid_entry("wcq");
    switch(c){
        case 'w':
           return QUICKEST;
        case 'c':
            return LEAST_TRANSFERS;
        default:
            exit(EXIT_SUCCESS);
    }
}

char get_valid_entry(char *valid){
    char c, temp;
    
    for(;;){
        c=fgetc(stdin);
        if(strchr(valid, c) != NULL){
            for(temp = fgetc(stdin); temp == ' ' || temp == '\t'; temp = fgetc(stdin));
            if(temp != '\n' && temp != EOF){
                print_error("Invalid entry, please try again.");
                while(temp != '\n' && temp != EOF)
                    temp=fgetc(stdin);
            }
            else
                break;
        }
        else if(c!=EOF && c!='\n'){
            print_error("Invalid entry, please try again.");
            while(c != '\n' && c != EOF)
                c=fgetc(stdin);
        }
    }
    return c;
}

unsigned int get_station(){
    unsigned int digits = count_digits_number(metro.nsta), i;
    char c, station[digits+1]; // create a buffer which can contain the same number of char as the number of digit of the number of stations + '\0'
    
    printf("Please enter a station number :\n");
    
    for(i=0, c=fgetc(stdin);;c=fgetc(stdin)){        
        if(c == 'q' && i==0)
            exit(EXIT_SUCCESS);
        
        if(c == '\n' || c == EOF){
            if(i==0)
                continue;
            station[i]='\0';
            i = (unsigned int) strtoul(station,NULL,10);
            if(i>=metro.nsta){
                i=digits;
                ungetc(c, stdin);
            }
            else
                return i;
        }
        else if(!isdigit(c)){
            print_error("Invalid entry, not a number, please try again.");
            for(c=fgetc(stdin); c!='\n' && c!=EOF; c=fgetc(stdin));
            i=0;
            continue;
        }
        if(i==digits){
            print_error("Invalid entry, number is too big, please try again.");
            for(c=fgetc(stdin); c!='\n' && c!=EOF; c=fgetc(stdin));
            i=0;
            continue;
        }
        station[i]=c;
        i++;
    }
}