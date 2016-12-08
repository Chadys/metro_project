#include "header.h"


void display_lines(){
    unsigned int i;

    printf("\n%sLINES :\n\n", codeFromStyle(GREEN));

    for(i=0; i<metro.nli; i++)
        printf("%s%c = %s%s\n", codeFromStyle(GRAY), metro.lines[i].symbol, codeFromStyle(WHITE), metro.lines[i].name);
}

void display_stations(RUN_MODE mode){
    unsigned int i, j;
    succ *corr;

    printf("\n\n%sSTATIONS :\n\n", codeFromStyle(GREEN));

    for(i=0;i<metro.nsta;i++){
        printf("%s%d : %s %s(", codeFromStyle(BLUE), i, metro.stations[i].name, codeFromStyle(DARKBLUE));
        printf("%c%u", metro.lines[metro.stations[i].line[0]].symbol, metro.stations[i].line[1]);
        printf(")%s Transfer(s) :%s", codeFromStyle(PURPLE), codeFromStyle(MAGENTA));
        
        switch(mode){
            case MATRIX:
                for(j=0; j<matrix.nb; j++)
                    if(matrix.mat[i*matrix.nb+j])
                        printf(" %s", metro.stations[j].name);;
                break;
            default:
                corr=graph_list.noeuds[i].next;
                while(corr){
                    printf(" %s", metro.stations[corr->car->num].name);
                    corr=corr->cdr;
                }
        }
        printf("\n");
    }
}

void display_metro(RUN_MODE mode){
    printf("\n%s%s%s%s\n", codeFromStyle(BOLD), codeFromStyle(BRIGHTGREEN), metro.name, codeFromStyle(RESET));
    display_lines();
    display_stations(mode);
    printf("%s\n\n", codeFromStyle(RESET));
}


void display_path(unsigned int begin, unsigned int end, trajet *path, SEARCH_MODE mode){
    unsigned int to=end, it=end, prec, line;
    int dir;
    
    if(!path){
        printf("There is no path from %s to %s or an error occured\n", metro.stations[begin].name, metro.stations[end].name);
        return;
    }
    if(mode == QUICKEST)
        printf("The shortest path from %s to %s is :\n", metro.stations[begin].name, metro.stations[end].name);
    else
        printf("The path with the least transfer(s) (%u transfer(s)) from %s to %s is :\n", path[end].distance/1000, metro.stations[begin].name, metro.stations[end].name);   
    
    
    //make each station keep the station they go to instead of the station they're from, for display purpose
    while(to != begin){
        prec = path[it].from;
        path[it].from = to;
        to=it;
        it = prec;
    }
    it = to;
    
    line = UINT_MAX;
    printf("%s%s%s%s", codeFromStyle(BOLD), codeFromStyle(BLACK+metro.stations[it].line[0]%15+1), metro.stations[it].name, codeFromStyle(RESET));
    while(path[it].from != it){
        if(line != metro.stations[path[it].from].line[0]){
            dir = metro.stations[it].line[0] == metro.stations[path[it].from].line[0] ? (metro.stations[it].line[1] < metro.stations[path[it].from].line[1] ? 1 : -1) : 0;
            line = metro.stations[path[it].from].line[0];
            if(!dir)
                printf(" -> %swalk%s", codeFromStyle(BLACK+metro.nli%15+1), codeFromStyle(BLACK+line%15+1));
            else
                printf(" -> %s(%s direction %s)", codeFromStyle(BLACK+line%15+1), metro.lines[line].name, dir>0 ? "MAX" : "1");
        }
        printf(" -> %s", metro.stations[path[it].from].name);
        it=path[it].from;
    }
    printf("%s\n\n", codeFromStyle(RESET));
}