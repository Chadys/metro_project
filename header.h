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
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


/*** All subway's info ***/

/* First version with static arrays

typedef struct line{
    char symbol;
    char name [MAX_LINE_NAME];
} line;

typedef struct station{
    unsigned char line[MAX_STATION_LINES][2]; //the line this station belongs to, line[i][0] is the lines index, line[i][1] is the station's number on that line
    char name [MAX_LINE_NAME];
} station;

typedef struct metro{
    station stations [MAX_STATIONS];
    unsigned int nsta;
    line lines [MAX_LINES];
    unsigned int nli;
} metro;*/

typedef struct line{
    char symbol;
    char *name;
} line;

typedef struct station{
    unsigned int (* lines)[2]; //the line this station belongs to, line[i][0] is the lines index, line[i][1] is the station's number on that line
    unsigned int nlines;
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
    unsigned int line; // line index in this station lines' list
    char direction;
} trajet;

typedef struct trajets{
    trajet *trajet;
    unsigned int ntrajets;
} trajets;

/*** Other ***/

typedef struct memory{
    void *adr;
    struct memory * next;
} memory;


/**************************************************************************************************/
/***** GLOBALS *****/

/* init_metro.c */
// Contain all the subway info (name of stations and lines, ...)
extern subway metro;
// Contain the graph by list of successors
extern graphlist graph1;

/* utils.c */
// Linked list of all allocated memory in this program, to free all easily
extern memory *garbage_collector;


/**************************************************************************************************/
/***** PROTOTYPES *****/

/* init_metro.c */
// The subway network file parser, which construct the graph or indicate errors
char init_metro(char *);

/* dijkstra.c */
// The Dijkstra algorithm to find all shortest path
trajets dijkstra_list(unsigned int, unsigned int);

/* display.c */
// All display functions
void display_metro();
void display_paths(unsigned int, unsigned int, trajets);

/* utils.c */
// Miscellanous functions
void *getmem(size_t, size_t);
void clean_mem();
void ctrlC(int);
size_t count_lines(FILE *);
char * ordinal_suffix(unsigned int);
void print_error(const char *);


/**************************************************************************************************/
/***** ENUM *****/

typedef enum TERMINAL_STYLES{
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
} TERMINAL_STYLES;

static inline char *codeFromStyle(TERMINAL_STYLES s)
{
    static char *strings[] = {"\e[0m", "\e[1m", "\e[2m", "\e[3m", "\e[4m", "\e[6m", "\e[7m", "\e[8m", "\e[9m",
        "\e[30m", "\e[31m", "\e[32m", "\e[33m", "\e[34m", "\e[35m", "\e[36m", "\e[37m",
        "\e[90m", "\e[91m", "\e[92m", "\e[93m", "\e[94m", "\e[95m", "\e[96m", "\e[97m",
        "\e[40m", "\e[41m", "\e[42m", "\e[43m", "\e[44m", "\e[45m", "\e[46m", "\e[47m",
        "\e[100m", "\e[101m", "\e[102m", "\e[103m", "\e[104m", "\e[105m", "\e[106m", "\e[107m"};

    return strings[s];
}

#endif