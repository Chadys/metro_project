#include "header.h"

int main(){
	int i;

	if (signal(SIGINT,ctrlC) == SIG_ERR)
		perror("signal");
	i=atexit(clean_mem);
	if(i)
		fprintf(stderr, "Could not register function clean_mem\n");

	init_metro("tokyo.metro");
	return 0;
}