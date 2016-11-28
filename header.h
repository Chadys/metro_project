/* 15.
Gérer un réseau de transports en commun, par exemple le métro de Taipei, via un graphe (Mat et Lis).
Résoudre dans ce cas le problème du plus court chemin, algorithme de Dijkstra. */

#ifndef _METRO_
#define _METRO_

#include <ctype.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATIONS 200 //max number of stations
#define MAX_STATION_LINES 5 //max number of line a subway station belongs to
#define MAX_STATION_NAME 50 //max size of the name of a subway station
#define MAX_LINES 15 //max number of lines
#define MAX_LINE_NAME 50 //max size of the name of a subway line
#define MAX_CHAR_READ 500 //max char of a text file read at once


/* graph by successors' list*/

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


/*typedef struct line{
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
	unsigned int (* line)[2]; //the line this station belongs to, line[i][0] is the lines index, line[i][1] is the station's number on that line
	char * name;
} station;

typedef struct subway{
	station * stations;
	unsigned int nsta;
	line * lines;
	unsigned int nli;
} subway;

typedef struct memory{
	void *adr;
	struct memory * next;
} memory;


/* globals */
extern subway metro;
extern memory *garbage_collector;
extern graphlist graph1;

/* graphe par matrice */



/* prototypes */

/* init_metro.c */
// The subway network file parser, which construct the graph or indicate errors
char init_metro(char *);

/* utils.c */
// Miscellanous functions
void *getmem(size_t, size_t);
void clean_mem();
void ctrlC(int);
size_t count_lines(FILE *);

#endif