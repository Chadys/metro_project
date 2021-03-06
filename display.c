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
        for(j=0; j < metro.stations[i].nlines; j++){
            if(!j)
                printf("%c%u", metro.lines[metro.stations[i].lines[j][0]].symbol, metro.stations[i].lines[j][1]);
            else
                printf("/%c%u", metro.lines[metro.stations[i].lines[j][0]].symbol, metro.stations[i].lines[j][1]);
        }
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


void display_path(unsigned int begin, unsigned int end, trajet *path, unsigned int ntrajet){
    unsigned int to=end, it=end, prec, line = UINT_MAX;
    int dir = 2;
    
    //make each station keep the station they go to instead of the station they're from, for display purpose
    // !! modifications are done on the path itself, they change it, so you can't call this function twice !!
    while(to != begin){
        prec = path[it].from;
        path[it].from = to;
        to=it;
        it = prec;
    }
    it = to;
    printf("%u : %s%s%s", ntrajet, codeFromStyle(BOLD), metro.stations[it].name, codeFromStyle(RESET));
    
    while(path[it].from != it){
        if(path[path[it].from].direction != dir ||
            (path[path[it].from].line != UINT_MAX && metro.stations[path[it].from].lines[path[path[it].from].line][0] != line) ||
            (path[path[it].from].line == UINT_MAX && path[path[it].from].line != line)){
            
            line = path[path[it].from].line != UINT_MAX ? metro.stations[path[it].from].lines[path[path[it].from].line][0] : UINT_MAX;
            dir = path[path[it].from].direction;
            if(!dir)
                printf(" -> %swalk", codeFromStyle(BLACK+metro.nli%15+1));
            else
                printf(" -> %s(%s direction %s)", codeFromStyle(BLACK+line%15+1), metro.lines[line].name, dir>0 ? "MAX" : "1");
        }
        printf(" -> %s", metro.stations[path[it].from].name);
        it=path[it].from;
    }
    printf("%s\n\n", codeFromStyle(RESET));
}

void display_paths(unsigned int begin, unsigned int end, trajets paths, SEARCH_MODE mode){
    unsigned int i;
    
    if(!paths.ntrajets){
        printf("There is no path from %s to %s or an error occured\n", metro.stations[begin].name, metro.stations[end].name);
        return;
    }
    if(paths.ntrajets >1){
        if(mode == QUICKEST)
            printf("The shortest paths from %s to %s are :\n", metro.stations[begin].name, metro.stations[end].name);
        else
            printf("The paths with the least transfers (%u transfer(s)) from %s to %s are :\n", paths.trajet[end].distance/1000, metro.stations[begin].name, metro.stations[end].name);
    }
    else{
        if(mode == QUICKEST)
            printf("The shortest path from %s to %s is :\n", metro.stations[begin].name, metro.stations[end].name);
        else
            printf("The path with the least transfers (%u transfer(s)) from %s to %s is :\n", paths.trajet[end].distance/1000, metro.stations[begin].name, metro.stations[end].name);
    }
    for (i=0; i<paths.ntrajets; i++)
        display_path(begin, end, paths.trajet+i*metro.nsta, i+1);
}