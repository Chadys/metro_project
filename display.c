#include "header.h"


void display_lines(){
    unsigned int i;

    printf("\nLINES :\n\n");

    for(i=0; i<metro.nli; i++)
        printf("%c = %s\n", metro.lines[i].symbol, metro.lines[i].name);
}

void display_stations(){
    unsigned int i, j;
    succ *corr;

    printf("\n\nSTATIONS :\n\n");

    for(i=0;i<graph1.nb;i++){
        printf("%s (", metro.stations[i].name);
        for(j=0; j < metro.stations[i].nlines; j++){
            if(!j)
                printf("%c%u", metro.lines[metro.stations[i].lines[j][0]].symbol, metro.stations[i].lines[j][1]);
            else
                printf("/%c%u", metro.lines[metro.stations[i].lines[j][0]].symbol, metro.stations[i].lines[j][1]);
        }
        printf(") Correspondence(s) :");
        corr=graph1.noeuds[i].next;
        while(corr){
            printf(" %s", metro.stations[corr->car->num].name);
            corr=corr->cdr;
        }
        printf("\n");
    }
}

void display_metro(){
    printf("%s\n\n", metro.name);
    display_lines();
    display_stations();
}


void display_path(unsigned int begin, unsigned int end, trajet *path){
    unsigned int to=end, it=end, prec, line = UINT_MAX;
    int dir = 2;
    
    while(to != begin){
        prec = path[it].from;
        path[it].from = to;
        to=it;
        it = prec;
    }
    it = to;
    printf("%s", metro.stations[it].name);
    while(path[it].from != it){
        if(metro.stations[path[it].from].lines[path[path[it].from].line][0] != line || path[path[it].from].direction != dir){
            line = metro.stations[path[it].from].lines[path[path[it].from].line][0];
            dir = path[path[it].from].direction;
            if(!dir)
                printf(" -> walk");
            else
                printf(" -> (%s direction %s)", metro.lines[line].name, dir>0 ? "MAX" : "1");
        }
        printf(" -> %s", metro.stations[path[it].from].name);
        it=path[it].from;
    }
    printf("\n");
}

void display_paths(unsigned int begin, unsigned int end, trajets paths){
    unsigned int i;
    
    if(!paths.ntrajets){
        printf("There is no path from %s to %s or an error occured\n", metro.stations[begin].name, metro.stations[end].name);
        return;
    }
    if(paths.ntrajets >1)
        printf("The shortest paths from %s to %s are :\n", metro.stations[begin].name, metro.stations[end].name);
    else
        printf("The shortest path from %s to %s is :\n", metro.stations[begin].name, metro.stations[end].name);
    for (i=0; i<paths.ntrajets; i++){
        display_path(begin, end, paths.trajet+i*metro.nsta);
    }
}