#include "header.h"


trajets dijkstra_listbis(unsigned int current, unsigned int end, trajets path, unsigned int);
void get_common_lines(unsigned int a, unsigned int b, unsigned int lines[][2]);
trajets dijkstra_list_sort(trajets, unsigned int);
unsigned int find_new_shortest(trajet* path);
void change_paths(trajets, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, unsigned int, char);

// Get shortest path with the graph of successors' list
trajets dijkstra_list(unsigned int begin, unsigned int end){
    trajets paths = {NULL, 0};
    
    paths.trajet = malloc(metro.nsta*sizeof(trajet));
    if(!paths.trajet){
        perror("malloc");
        return paths;
    }
    paths.ntrajets = 1;
    // put UINT_MAX for every (path+i)->from, (path+i)->distance and (path+i)->line, UCHAR_MAX for every (path+i)->unused and -1 for every (path+i)->direction
    memset(paths.trajet, UCHAR_MAX, metro.nsta*sizeof(trajet));
    
    paths.trajet[begin].distance=0;
    paths.trajet[begin].direction=0;
    return dijkstra_listbis(begin, end, paths, 0);
}


trajets dijkstra_listbis(unsigned int current, unsigned int end, trajets paths, unsigned int current_path){
    unsigned int distance, i, j, n, lines[metro.nli+1][2], extra_paths = paths.ntrajets;
    succ *corr;
    void * test;
    trajet *path;
    
    path = paths.trajet+current_path*metro.nsta;
    path[current].unused = 0;
        
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
                
                get_common_lines(current, corr->car->num, lines);
                if(lines[0][0] == UINT_MAX) // if correspondence on foot
                    distance ++;
            }
            if(distance < path[corr->car->num].distance){
                if(!lines[0][1]){ // if line doesn't change
                    change_paths(paths, current_path, extra_paths, corr->car->num, distance, current, n, path[current].direction);
                }
                else{
                    change_paths(paths, current_path, extra_paths, corr->car->num, distance, current, lines[0][0], lines[0][1]-2);
                    for(i=0; lines[0][1]-2 /*at least one line */ && lines[i+1][0] != UINT_MAX; i++); // count number of extra lines
                    if(i){
                        paths.ntrajets += i;
                        test = realloc(paths.trajet, paths.ntrajets*metro.nsta*sizeof(trajet));
                        if(!test){
                            free(paths.trajet);
                            perror("realloc");
                            return (trajets){NULL,0};
                        }
                        paths.trajet = test;
                        for(j=paths.ntrajets-i; i; i--, j++){
                            memcpy(paths.trajet+j*metro.nsta, paths.trajet+current_path*metro.nsta, metro.nsta*sizeof(trajet));
                            paths.trajet[j*metro.nsta+corr->car->num].distance = distance;
                            paths.trajet[j*metro.nsta+corr->car->num].from = current;
                            paths.trajet[j*metro.nsta+corr->car->num].line = lines[i][0];
                            paths.trajet[j*metro.nsta+corr->car->num].direction = lines[i][1]-2;
                        }
                    }
                }
            }
            else if(distance == path[corr->car->num].distance){
                for(i=0; lines[0][1] /*line has changed */ && lines[0][1]-2 /*at least one line in common */ && lines[i+1][0] != UINT_MAX; i++); // count number of extra lines
                paths.ntrajets += i+1;
                test = realloc(paths.trajet, paths.ntrajets*metro.nsta*sizeof(trajet));
                if(!test){
                    free(paths.trajet);
                    perror("realloc");
                    return (trajets){NULL,0};
                }
                paths.trajet=test;
                if(!lines[0][1]){ //line hasn't changed
                    memcpy(paths.trajet+(paths.ntrajets-1)*metro.nsta, paths.trajet+current_path*metro.nsta, metro.nsta*sizeof(trajet));
                    paths.trajet[(paths.ntrajets-1)*metro.nsta+corr->car->num].distance = distance;
                    paths.trajet[(paths.ntrajets-1)*metro.nsta+corr->car->num].from = current;
                    paths.trajet[(paths.ntrajets-1)*metro.nsta+corr->car->num].line = n;
                    paths.trajet[(paths.ntrajets-1)*metro.nsta+corr->car->num].direction = path[current].direction;
                }
                else{
                    for(j=paths.ntrajets-i-1, i=0; j<paths.ntrajets; j++, i++){
                        memcpy(paths.trajet+j*metro.nsta, paths.trajet+current_path*metro.nsta, metro.nsta*sizeof(trajet));
                        paths.trajet[j*metro.nsta+corr->car->num].distance = distance;
                        paths.trajet[j*metro.nsta+corr->car->num].from = current;
                        paths.trajet[j*metro.nsta+corr->car->num].line = lines[i][0];
                        paths.trajet[j*metro.nsta+corr->car->num].direction = lines[i][1]-2;
                    }
                }
            }
        }
        corr = corr->cdr;
    }
    
    paths = dijkstra_listbis(find_new_shortest(paths.trajet+current_path*metro.nsta), end, paths, current_path);
    for(i=extra_paths;paths.ntrajets && i<paths.ntrajets;i++){
        paths = dijkstra_listbis(find_new_shortest(paths.trajet+i*metro.nsta), end, paths, i);
    }
    if(!paths.ntrajets)
        return paths;
    return dijkstra_list_sort(paths, end);
}

void get_common_lines(unsigned int a, unsigned int b, unsigned int lines[][2]){
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

void change_paths(trajets paths, unsigned int current_path, unsigned int extra_paths, unsigned int corr, unsigned int distance, unsigned int from, unsigned int line, char direction){
    unsigned int i;
    
    paths.trajet[current_path*metro.nsta+corr].distance = distance;
    paths.trajet[current_path*metro.nsta+corr].from = from;
    paths.trajet[current_path*metro.nsta+corr].line = line;
    paths.trajet[current_path*metro.nsta+corr].direction = direction;
    for(i=extra_paths; i<paths.ntrajets; i++){
        paths.trajet[i*metro.nsta+corr].distance = distance;
        paths.trajet[i*metro.nsta+corr].from = from;
        paths.trajet[i*metro.nsta+corr].line = line;
        paths.trajet[i*metro.nsta+corr].direction = direction;  
    }
}
    

unsigned int find_new_shortest(trajet *path){
    unsigned int i, n, d;
    
    for(i=0; i<metro.nsta; i++)
        if(path[i].unused){
            d=path[i].distance;
            n=i;
            i++;
            break;
        };
    for(; i<metro.nsta; i++)
        if(path[i].unused && d>path[i].distance){
            d=path[i].distance;
            n=i;
        };
    return n;
}


trajets dijkstra_list_sort(trajets t1, unsigned int end){
    unsigned int shortest_distance, i, j, size;
    unsigned char to_keep[t1.ntrajets];
    trajets final;
    
    memset(to_keep, 0, t1.ntrajets*sizeof(char));
    shortest_distance = t1.trajet[end].distance;
    to_keep[0] = 1;
    size=1;
    for (i=1; i<t1.ntrajets; i++){
        if(t1.trajet[i*metro.nsta+end].distance == shortest_distance){
            size++;
            to_keep[i] = 1;
        }
        else if(t1.trajet[i*metro.nsta+end].distance < shortest_distance){
            for(j=0; j<i; j++)
                to_keep[j] = 0;
            shortest_distance = t1.trajet[i*metro.nsta+end].distance;
            to_keep[i] = 1;
            size=1;
        }
    }
    
    final.ntrajets = size;
    final.trajet = malloc(size*sizeof(trajet[metro.nsta]));
    if (!final.trajet){
        free(t1.trajet);
        return (trajets){NULL, 0};
    }
    for(i=0, j=0; i<size; i++){
        for(; j<size; j++){
            if(to_keep[j]){
                    memcpy(final.trajet+i, t1.trajet+j*metro.nsta, metro.nsta*sizeof(trajet));
            }
        }
    }
    free(t1.trajet);
    return final;
}