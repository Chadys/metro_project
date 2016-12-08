#include "header.h"


unsigned int find_new_shortest(trajet*);


trajet *dijkstra(unsigned int current, unsigned int end, RUN_MODE r_mode, SEARCH_MODE s_mode){
    trajet *path;
    succ *corr;
    unsigned int corr_num, k, distance;
    
    if(current >= metro.nsta || end >= metro.nsta){
        print_error("Station not belonging to this network");
        return NULL;
    }
    if(current == end){
        print_error("You already are where you want to go");
        return NULL;
    }
    
    path = malloc(metro.nsta*sizeof(trajet));
    if(!path){
        perror("malloc");
        return NULL;
    }
    // put UINT_MAX for every path[station].from, path[station].distance and UCHAR_MAX for every path[station].unused
    memset(path, UCHAR_MAX, metro.nsta*sizeof(trajet));
    while(current != end){
        path[current].unused = 0;
        
        if(r_mode == SUCC_LIST){
            corr=graph_list.noeuds[current].next;
            if(corr)
                corr_num = corr->car->num;
        }
        else{
            for(k=0; k<matrix.nb; k++){
                if(matrix.mat[current*matrix.nb+k]){
                    corr_num = k;
                    break;
                }
            }
        }
        while(corr_num < metro.nsta){
            
            if(path[corr_num].unused){
            
                distance = path[current].distance;
                distance++;
                if(metro.stations[corr_num].line[0]!=metro.stations[current].line[0]){
                    if(s_mode == LEAST_TRANSFERS)
                        distance+=1000;
                    if(strcmp(metro.stations[corr_num].name, metro.stations[current].name)) //stations aren't the same, transfer on foot
                        distance++;
                }
                if(path[corr_num].distance>distance){
                    path[corr_num].distance = distance;
                    path[corr_num].from = current;
                }
            }
        
            if(r_mode == SUCC_LIST){
                corr = corr->cdr;
                if(corr)
                    corr_num = corr->car->num;
                else
                    corr_num=metro.nsta;
            }
            else{
                for(k++; k<matrix.nb; k++){
                    if(matrix.mat[current*matrix.nb+k]){
                        corr_num = k;
                        break;
                    }
                }
                if(k==matrix.nb)
                    corr_num = k;
            }
        }
        current = find_new_shortest(path);
    }
    return path;
}


unsigned int find_new_shortest(trajet *path){
    unsigned int i, n = UINT_MAX, d;
    
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