#include "header.h"

int main(){
    int i;
    trajets paths;
    unsigned int begin, end;

    if (signal(SIGINT,ctrlC) == SIG_ERR)
        perror("signal");
    i=atexit(clean_mem);
    if(i)
        fprintf(stderr, "Could not register function clean_mem\n");

    if(init_metro("tokyo.metro"))
      display_metro();
    
    begin = 8;
    end = 15;
    paths = dijkstra_list(begin, end);
    
    display_paths(begin, end, paths);
    free(paths.trajet);
    return 0;
}