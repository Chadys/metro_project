#include "header.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void user_search(void *r_mode){
    trajets paths;
    unsigned int begin, end;
    SEARCH_MODE s_mode;

    s_mode = get_s_mode();

    printf("You need to choose the station of departure.\n");
    begin = get_station();
    printf("You need to choose the station of arrival.\n");
    end = get_station();
    paths = dijkstra(begin, end, *(RUN_MODE*)r_mode, s_mode);

    display_paths(begin, end, paths, s_mode);

    if(paths.ntrajets){
        paths.ntrajets = 0;
        free(paths.trajet);
    }
    printf("\nHere you are ! Back to a new search, you can still quit anytime with 'q'\n\n");
}

int main(){
    int i;
    trajets paths;
    unsigned int begin, end;
    SEARCH_MODE s_mode;
    RUN_MODE r_mode;
    double time_spent;
    clock_t start, stop;

    if (signal(SIGINT,ctrlC) == SIG_ERR)
        perror("signal");
    i=atexit(quit);
    if(i)
        print_error("Could not register function clean_mem");

    
    r_mode = get_r_mode();
    
    start = clock();
    if(!init_metro("tokyo.metro", r_mode)){
        if(i)
            quit();
        exit(EXIT_FAILURE);
    }
    stop = clock();
    time_spent = (double)(stop - start) / CLOCKS_PER_SEC;
    printf("Graph init tooks %f second(s)\n", time_spent);
    display_metro(r_mode);
    
    
    if (r_mode == COMPARE){
        s_mode = get_s_mode();
        
        // calculate and display time taken with list of successors
        start = clock();
        for(begin=0; begin<metro.nsta; begin++){
            for(end=begin+1; end<metro.nsta; end++){
                paths = dijkstra(begin, end, SUCC_LIST, s_mode);
        
                if(paths.ntrajets){
                    paths.ntrajets = 0;
                    free(paths.trajet);
                }
            }
        }
        stop = clock();
        time_spent = (double)(stop - start) / CLOCKS_PER_SEC;
        printf("Dijkstra with a graph by list of successors took %f second(s) to calculate all possible paths\n", time_spent);
               
        // calculate and display time taken with matrix
        start = clock(); 
        for(begin=0; begin<metro.nsta; begin++){
            for(end=begin+1; end<metro.nsta; end++){
                paths = dijkstra(begin, end, MATRIX, s_mode);
        
                if(paths.ntrajets){
                    paths.ntrajets = 0;
                    free(paths.trajet);
                }
            }
        }
        stop = clock();
        time_spent = (double)(stop - start) / CLOCKS_PER_SEC;
        printf("Dijkstra with a graph by matrix took %f second(s) to calculate all possible paths\n", time_spent);
    }
    
    
    else {

#ifdef __EMSCRIPTEN__
        // void emscripten_set_main_loop(em_callback_func func, System* func_arg, int fps, int simulate_infinite_loop);
        emscripten_set_main_loop_arg(user_search, &r_mode, 0, 1); //setting 0 or a negative value as the fps will use the browser’s requestAnimationFrame mechanism to call the main loop function
#else
        for(;;){
            user_search(&r_mode);
        }
#endif
    }
    
    if(i)
        quit();
    
    exit(EXIT_SUCCESS);
}
