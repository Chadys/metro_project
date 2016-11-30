#include "header.h"


trajets dijkstra_listbis(unsigned int, unsigned int, trajet[]);
unsigned int **get_common_lines(unsigned int a, unsigned int b, unsigned int lines[][2]);
trajets dijkstra_list_sort(trajets, trajets, unsigned int);
unsigned int find_new_shortest(trajet[]);

// Get shortest path with the graph of successors' list
trajets dijkstra_list(unsigned int begin, unsigned int end){
    trajet path[metro.nli];
      
    // put UINT_MAX for every (path+i)->from, (path+i)->distance and (path+i)->line, UCHAR_MAX for every (path+i)->unused and -1 for every (path+i)->direction
    memset(path, UCHAR_MAX, metro.nsta*sizeof(trajet));
    
    path[begin].distance=0;
    path[begin].unused=0;
    path[begin].direction=0;
    return dijkstra_listbis(begin, end, path);
}


trajets dijkstra_listbis(unsigned int current, unsigned int end, trajet path[]){
    unsigned int distance, i, j, n, lines[metro.nli+1][2];
    succ *corr;
    void * test;
    trajets paths = {NULL, 0}, final = {NULL, 0};
    
    paths.trajet = malloc(sizeof(trajet[metro.nsta]));
    if(!paths.trajet){
        perror("malloc");
        return paths;
    }
    paths.ntrajets = 1;
    (*paths.trajet) = path;
        
    if(current == end)
        return paths;
    
    corr=graph1.noeuds[current].next;
    while(corr){
        if(path[corr->car->num].unused){
            distance = 1+path[current].distance;
            if(path[current].direction){ //if current isn't the first station or one we got to by walking
                for(i=0; i<metro.stations[corr->car->num].nlines; i++){
                    if(metro.stations[corr->car->num].lines[i][0] == metro.stations[current].lines[path[current].line][0]
                      && (path[current].direction > 0 ? metro.stations[current].lines[path[current].line][1] < metro.stations[corr->car->num].lines[i][1] : metro.stations[current].lines[path[current].line][1] > metro.stations[corr->car->num].lines[i][1])){
                        lines[0][1] = 0;
                        n=i;
                        break; // if we stay on the same line without changing direction
                    }
                }
                if(i == metro.stations[corr->car->num].nlines){ //if line change
                    distance++;
                    get_common_lines(current, corr->car->num, lines);
                }
            }
            else{ //current is first station or we got to it by walking so no current line
                distance = 1+path[current].distance;
                get_common_lines(current, corr->car->num, lines);
                if(lines[0][0] == UINT_MAX)
                    distance ++;
            }
            if(distance < path[corr->car->num].distance){
                if(!lines[0][1]){ // if line doesn't change
                    for(j=0; j<paths.ntrajets; j++){
                        paths.trajet[j][corr->car->num].distance = distance;
                        paths.trajet[j][corr->car->num].from = current;
                        paths.trajet[j][corr->car->num].line = n;
                        paths.trajet[j][corr->car->num].direction = path[current].direction;
                    }
                }
                else{
                    for(i=0; i<paths.ntrajets; i++){
                        paths.trajet[i][corr->car->num].distance = distance;
                        paths.trajet[i][corr->car->num].from = current;
                        paths.trajet[i][corr->car->num].line = lines[0][0];
                        paths.trajet[i][corr->car->num].direction = lines[0][1]-2;
                    }
                    for(i=0; lines[0][1]-2 /*at least one line */ && lines[i+1][0] != UINT_MAX; i++); // count number of extra lines
                    if(i){
                        paths.ntrajets += i;
                        test = realloc(paths.trajet, paths.ntrajets*sizeof(trajet[metro.nsta]));
                        if(!test){
                            free(paths.trajet);
                            perror("realloc");
                            return (trajets){NULL,0};
                        }
                        paths.trajet = test;
                        for(j=paths.ntrajets-i; i; i--, j++){
                            paths.trajet[j][corr->car->num].distance = distance;
                            paths.trajet[j][corr->car->num].from = current;
                            paths.trajet[j][corr->car->num].line = lines[i][0];
                            paths.trajet[j][corr->car->num].direction = lines[i][1]-2;
                        }
                    }
                }
            }
            if(distance == path[corr->car->num].distance){
                for(i=0; lines[0][1] /*line has changed */ && lines[0][1]-2 /*at least one line in common */ && lines[i+1][0] != UINT_MAX; i++); // count number of extra lines
                paths.ntrajets += i+1;
                test = realloc(paths.trajet, paths.ntrajets*sizeof(trajet[metro.nsta]));
                if(!test){
                    free(paths.trajet);
                    perror("realloc");
                    return (trajets){NULL,0};
                }
                if(lines == NULL){
                    paths.trajet[paths.ntrajets-1][corr->car->num].distance = distance;
                    paths.trajet[paths.ntrajets-1][corr->car->num].from = current;
                    paths.trajet[paths.ntrajets-1][corr->car->num].line = n;
                    paths.trajet[paths.ntrajets-1][corr->car->num].direction = path[current].direction;
                }
                else{
                    paths.trajet[paths.ntrajets-i-1][corr->car->num].distance = distance;
                    paths.trajet[paths.ntrajets-i-1][corr->car->num].from = current;
                    paths.trajet[paths.ntrajets-i-1][corr->car->num].line = lines[0][0];
                    paths.trajet[paths.ntrajets-i-1][corr->car->num].direction = lines[0][1]-2;
                    for(j=paths.ntrajets-i, i=0; j<paths.ntrajets; j++, i++){
                        paths.trajet[j][corr->car->num].distance = distance;
                        paths.trajet[j][corr->car->num].from = current;
                        paths.trajet[j][corr->car->num].line = lines[i][0];
                        paths.trajet[j][corr->car->num].direction = lines[i][1]-2;
                    }
                }
            }
        }
        corr = corr->cdr;
    }
    
    for(i=0;i<paths.ntrajets;i++){
        if(!final.ntrajets)
            final = dijkstra_listbis(find_new_shortest(paths.trajet[i]), end, paths.trajet[i]);
        else final = dijkstra_list_sort(final, dijkstra_listbis(find_new_shortest(paths.trajet[i]), end, paths.trajet[i]), end);
    }
    return final;
}

unsigned int **get_common_lines(unsigned int a, unsigned int b, unsigned int lines[][2]){
    unsigned int i, j, n;
    //lines[i][0] = b's line index ; lines[i][1] = direction from a to b
    //if lines[i][0] == UINT_MAX, end of list (explain the metro.nli+1);
    
    n=0;
    for (i=0; i<metro.stations[a].nlines; i++){
        for  (j=0; j<metro.stations[b].nlines; j++){
            if (metro.stations[b].lines[j][0] == metro.stations[a].lines[i][0]){
                lines[n][0] = j;
                if (metro.stations[a].lines[i][1] < metro.stations[b].lines[j][1])
                    lines[n][1] = 3; // true direction will be had with dir-2, to avoid another structure I needed to keep it as an unsigned int
                else
                lines[n][1] = 1;
                n++;
            }
        }
    }
    lines[n][0] = UINT_MAX;
    lines[n][1] = 2; //needed in case there is no common lines, lines[0] is always assignable, direction will be 0
}


unsigned int find_new_shortest(trajet path[]){
    unsigned int i, n, d;
    
    for(i=0; i<metro.nsta; i++)
        if(path[i].unused){
            d=path[i].distance;
            n=i;
            break;
        };
    for(; i<metro.nsta; i++)
        if(d>path[i].distance){
            d=path[i].distance;
            n=i;
        };
    return n;
}


trajets dijkstra_list_sort(trajets t1, trajets t2, unsigned int end){
    unsigned int shortest_distance;
    unsigned char to_keep[t1.ntrajets+t2.ntrajets], to_trash[t1.ntrajets+t2.ntrajets];
    
    shortest_distance = (*t1.trajet)[end].distance;
    return (trajets){NULL, 0};
}