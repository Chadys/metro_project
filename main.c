#include "header.h"

int main(){
    int i;
    trajets paths;
    unsigned int begin, end;

    if (signal(SIGINT,ctrlC) == SIG_ERR)
        perror("signal");
    i=atexit(clean_mem);
    if(i)
        print_error("Could not register function clean_mem");

    if(init_metro("tokyo.metro"))
      display_metro();
    
    
//     begin = 99;
//     end = 24;
    
    for(begin=0; begin<metro.nsta; begin++){
        for(end=begin+1; end<metro.nsta; end++){
            paths = dijkstra_list(begin, end);
            
            display_paths(begin, end, paths);
        
            if(paths.ntrajets){
                paths.ntrajets = 0;
                free(paths.trajet);
            }
        }
    }
    
    return 0;
}