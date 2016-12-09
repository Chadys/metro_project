#include "header.h"


int main(){
    int i;
    trajet *path;
    unsigned int begin, end;
    RUN_MODE r_mode;
    SEARCH_MODE s_mode;
    double time_spent;
    clock_t start, stop;

    if (signal(SIGINT,ctrlC) == SIG_ERR)
        perror("signal");
    i=atexit(quit);
    if(i)
        print_error("Could not register function clean_mem");

    r_mode = get_r_mode();
    
    if(!init_metro("tokyo.metro", r_mode)){
        if(i)
            quit();
        exit(EXIT_FAILURE);
    }
    display_metro(r_mode);
    
    
    if (r_mode == COMPARE){
        s_mode = get_s_mode();
        
        // calculate and display time taken with list of successors
        start = clock();
        for(begin=0; begin<metro.nsta; begin++){
            for(end=begin+1; end<metro.nsta; end++){
                path = dijkstra(begin, end, SUCC_LIST, s_mode);
        
                if(path)
                    free(path);
            }
        }
        stop = clock();
        time_spent = (double)(stop - start) / CLOCKS_PER_SEC;
        printf("Dijkstra with a graph by list of successors took %f second(s) to calculate all possible paths\n", time_spent);
               
        // calculate and display time taken with matrix
        start = clock(); 
        for(begin=0; begin<metro.nsta; begin++){
            for(end=begin+1; end<metro.nsta; end++){
                path = dijkstra(begin, end, MATRIX, s_mode);
        
                if(path)
                    free(path);
            }
        }
        stop = clock();
        time_spent = (double)(stop - start) / CLOCKS_PER_SEC;
        printf("Dijkstra with a graph by matrix took %f second(s) to calculate all possible paths\n", time_spent);
    }
    
    
    else for(;;){
        s_mode = get_s_mode();
        
        printf("You need to choose the station of departure.\n");
        begin = get_station();
        printf("You need to choose the station of arrival.\n");
        end = get_station();
        path = dijkstra(begin, end, r_mode, s_mode);
            
        display_path(begin, end, path, s_mode);
        
        if(path)
            free(path);
        printf("\nHere you are ! Back to a new search, you can still quit anytime with 'q'\n\n");
    }
    
    if(i)
        quit();
    
    exit(EXIT_SUCCESS);
}