/*
 * PROBLEM :
15.
Gérer un réseau de transports en commun, par exemple le métro de Taipei, via un graphe (Mat et Lis).
Résoudre dans ce cas le problème du plus court chemin, algorithme de Dijkstra.
*/

#ifndef _METRO_
#define _METRO_

/**************************************************************************************************/
/***** LIBRARIES AND DEFINE *****/

#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CHAR_READ 500 //max char of a text file read at once


/**************************************************************************************************/
/***** STRUCTURES *****/

/*** graph by successors' list ***/

typedef struct noeud{
    unsigned int num;
    struct succ *next;
} noeud;

typedef struct succ{
    noeud * car;
    struct succ * cdr;
} succ;

typedef struct graphlist{
    noeud * noeuds;
    unsigned int nb;
} graphlist;


/*** graph by matrix ***/

typedef struct graphmatrix{
    char *mat;
    unsigned int nb;
} graphmatrix;


/*** All subway's info ***/


typedef struct line{
    char symbol;
    char *name;
} line;

typedef struct station{
    unsigned int line[2]; //the line this station belongs to, line[0] is the lines index, line[1] is the station's number on that line
    char * name;
} station;

typedef struct subway{
    char * name;
    station * stations;
    unsigned int nsta;
    line * lines;
    unsigned int nli;
} subway;


/*** Dijkstra specific ***/

typedef struct trajet {
    unsigned int from;
    unsigned int distance;
    unsigned char unused;
} trajet;


/*** Other ***/

typedef struct memory{
    void *adr;
    struct memory * next;
} memory;


/**************************************************************************************************/
/***** ENUM *****/

typedef enum RUN_MODE{
        SUCC_LIST,
        MATRIX,
        COMPARE
} RUN_MODE;

typedef enum SEARCH_MODE{
        QUICKEST,
        LEAST_TRANSFERS
} SEARCH_MODE;

// All options to customize a terminal
enum TERMINAL_STYLES{
    RESET,
    BOLD,
    DARK,
    ITALIC,
    UNDERLINE,
    EXPANDED,
    BG,
    TRANSPARENT,
    STROKE,

    BLACK,
    RED,
    GREEN,
    BROWN,
    DARKBLUE,
    PURPLE,
    DARKCYAN,
    GRAY,

    DARKGRAY,
    BRIGHTRED,
    BRIGHTGREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,

    BG_BLACK,
    BG_RED,
    BG_GREEN,
    BG_BROWN,
    BG_DARKBLUE,
    BG_PURPLE,
    BG_DARKCYAN,
    BG_GRAY,

    BG_DARKGRAY,
    BG_BRIGHTRED,
    BG_BRIGHTGREEN,
    BG_YELLOW,
    BG_BLUE,
    BG_MAGENTA,
    BG_CYAN,
    BG_WHITE
};

static inline char *codeFromStyle(enum TERMINAL_STYLES s)
{
    static char *strings[] = {"\e[0m", "\e[1m", "\e[2m", "\e[3m", "\e[4m", "\e[6m", "\e[7m", "\e[8m", "\e[9m",
        "\e[30m", "\e[31m", "\e[32m", "\e[33m", "\e[34m", "\e[35m", "\e[36m", "\e[37m",
        "\e[90m", "\e[91m", "\e[92m", "\e[93m", "\e[94m", "\e[95m", "\e[96m", "\e[97m",
        "\e[40m", "\e[41m", "\e[42m", "\e[43m", "\e[44m", "\e[45m", "\e[46m", "\e[47m",
        "\e[100m", "\e[101m", "\e[102m", "\e[103m", "\e[104m", "\e[105m", "\e[106m", "\e[107m"};

    return strings[s];
}


/**************************************************************************************************/
/***** GLOBALS *****/

/* init_metro.c */
// Contain all the subway info (name of stations and lines, ...)
extern subway metro;
// Contain the graph by list of successors
extern graphlist graph_list;
// Contain the graph by matrix
extern graphmatrix matrix;

/* utils.c */
// Linked list of all allocated memory in this program, to free all easily
extern memory *garbage_collector;


/**************************************************************************************************/
/***** PROTOTYPES *****/

/* init_metro.c */
// The subway network file parser, which construct the graph or indicate errors
char init_metro(char* filename, RUN_MODE);

/* dijkstra.c */
// The Dijkstra algorithm to find all shortest path
trajet *dijkstra(unsigned int, unsigned int, RUN_MODE, SEARCH_MODE);

/* display.c */
// All display functions
void display_metro(RUN_MODE);
void display_path(unsigned int, unsigned int, trajet*, SEARCH_MODE);

/* user_input.c */
// Management of interactions with the user
RUN_MODE get_r_mode();
SEARCH_MODE get_s_mode();
unsigned int get_station();

/* utils.c */
// Miscellanous functions
void *getmem(size_t, size_t);
void quit();
void ctrlC(int);
size_t count_lines(FILE *);
char * ordinal_suffix(unsigned int);
unsigned int count_digits_number(unsigned int);
void print_error(const char *);

#endif