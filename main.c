#include "header.h"

int main(){
    int i;
    trajets paths;
    unsigned int begin, end;
    RUN_MODE mode;

    if (signal(SIGINT,ctrlC) == SIG_ERR)
        perror("signal");
    i=atexit(clean_mem);
    if(i)
        print_error("Could not register function clean_mem");

    

    mode = MATRIX;
    if(init_metro("tokyo.metro", mode))
      display_metro(mode);
    
    
    
    for(begin=0; begin<metro.nsta; begin++){
        for(end=begin+1; end<metro.nsta; end++){
            paths = dijkstra(begin, end, mode);
            
            display_paths(begin, end, paths);
        
            if(paths.ntrajets){
                paths.ntrajets = 0;
                free(paths.trajet);
            }
        }
    }
    
    if(i)
        clean_mem();    
    return 0;
}