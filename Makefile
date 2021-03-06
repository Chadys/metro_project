SRC= display.c init_metro.c dijkstra.c main.c user_input.c utils.c
OBJ = $(SRC:.c=.o)
CC=gcc
CFLAGS=-Wall -W -g

metro:	$(OBJ) header.h
			$(CC) $(OBJ) -o metro

%.o:		%.c
			$(CC) $(CFLAGS) -c $<

clean:
	rm *.o *~ core