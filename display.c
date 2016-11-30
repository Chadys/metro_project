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