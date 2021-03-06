#include "header.h"


subway metro = {NULL, NULL, 0, NULL, 0};
graphlist graph_list = {NULL, 0};
graphmatrix matrix = {NULL, 0};

char init_name(FILE *);
char init_lines(FILE *, size_t);
char init_stations(FILE *, size_t, RUN_MODE);
char init_station_line(FILE*);
char init_station_graph(FILE *, size_t, RUN_MODE);


// Initialise the whole metro, station, lines and graph included
char init_metro(char * filename, RUN_MODE mode){
    FILE * file;
    size_t line_size, station_size;
    int c;

    file=fopen(filename, "r");
    if(!file){
        perror("fopen");
        return 0;
    }
    count_lines(file); // to jump over the name of the metro

    line_size = count_lines(file);
    if(!line_size){
        print_error("Invalid subway file, wrong lines format");
        fclose(file);
        return 0;
    }
    metro.lines = getmem(line_size, sizeof(line));
    if (!metro.lines){
        fclose(file);
        return 0;
    }
    station_size = count_lines(file);
    if(!station_size){
        print_error("Invalid subway file, wrong stations format");
        fclose(file);
        return 0;
    }
    metro.stations = getmem(station_size, sizeof(station));
    if (!metro.stations){
        fclose(file);
        return 0;
    }
    while((c = fgetc(file)) != EOF){
        if (c != '\n'){
            print_error("Invalid subway file format, extra text");
            fclose(file);
            return 0;   
        }
    }
    rewind(file);

    if(!init_name(file) || !init_lines(file, line_size) || !init_stations(file, station_size, mode)){
        fclose(file);
        return 0;
    }
    fclose(file);
    return 1;
}

// Initialise the name of the metro
char init_name(FILE * file){
    char buffer [MAX_CHAR_READ];
    size_t size;

    if (!fgets(buffer, MAX_CHAR_READ, file)){
        perror("fgets");
        return 0;
    }
    buffer[MAX_CHAR_READ-1] = '\0'; //to avoid undefined behaviour of strlen
    size=strlen(buffer);
    if(buffer[size-1] != '\n'){
        print_error("Invalid subway file, name is too long");
        return 0;
    }
    if(!size){
        print_error("Invalid subway file, missing name");
        return 0;
    }
    metro.name = getmem(size+1, sizeof(char));
    if (!metro.name)
        return 0;
    strncpy(metro.name, buffer, size+1);
    if (!fgets(buffer, MAX_CHAR_READ, file)){
        perror("fgets");
        return 0;
    }
    if(buffer[0]!='\n'){
        print_error("Invalid subway file, missing carriage return between name and lines");
        return 0;
    }
    return 1;
}

// Initialise all the subway lines
char init_lines(FILE *file, size_t final_size /* number of lines */){
    char buffer [MAX_CHAR_READ];
    size_t size;

    while(fgets(buffer, MAX_CHAR_READ, file) && buffer[0] != '\n' && metro.nli < final_size){
        buffer[MAX_CHAR_READ-1] = '\0'; //to avoid undefined behaviour of strlen
        if((size=strlen(buffer)) < 3 || buffer[1]!='='){
            fprintf(stderr, "%s%sError : %s%sInvalid subway file, wrong line format, on line %u%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED), metro.nli+3, codeFromStyle(RESET));
            return 0;
        }
        if(buffer[size-1] != '\n'){
            fprintf(stderr, "%s%sError : %s%sInvalid subway file, line's name is too long, on line %u%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED), metro.nli+3, codeFromStyle(RESET));
            return 0;
        }
        metro.lines[metro.nli].symbol=buffer[0];
        metro.lines[metro.nli].name = getmem(size-1,sizeof(char));
        if(!metro.lines[metro.nli].name)
            return 0;
        strncpy(metro.lines[metro.nli].name, buffer+2, size-1);
        metro.lines[metro.nli].name[size-3]='\0'; // remplace le '\n' final
        metro.nli++;
    }
    if (metro.nli != final_size || buffer[0] != '\n'){
        perror("fgets");
        return 0;
    }
    return 1;
}

// Initialise all stations and the graph
char init_stations(FILE *file, size_t final_size /* number of stations */, RUN_MODE mode){
    char buffer [MAX_CHAR_READ];
    int c, i;
    
    switch(mode){
        case COMPARE:
            /* NO BREAK */
        case SUCC_LIST:
            graph_list.noeuds = getmem(final_size, sizeof(noeud));
            if (!graph_list.noeuds)
                return 0;
            if(mode != COMPARE)
                break;
        default:
            matrix.mat = getmem(final_size*final_size, sizeof(char));
            if (!matrix.mat)
                return 0;
            memset(matrix.mat, 0, final_size*final_size*sizeof(char));
    }
    while(metro.nsta < final_size){
        for(c=fgetc(file), i=0;c != ':';i++, c=fgetc(file)){
            if(i>=MAX_CHAR_READ){
                fprintf(stderr, "%s%sError : %s%sInvalid subway file, station's name is too long, on line %u%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED),  metro.nli+4+metro.nsta, codeFromStyle(RESET));
                return 0;
            }
            buffer[i]=c;
        }
        if (i==0){
            fprintf(stderr, "%s%sError : %s%sInvalid subway file, station's name missing, on line %u%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED),  metro.nli+4+metro.nsta, codeFromStyle(RESET));
            return 0;
        }
        buffer[i]='\0';
        metro.stations[metro.nsta].name = getmem(i+1,sizeof(char));
        if(!metro.stations[metro.nsta].name)
            return 0;
        strncpy(metro.stations[metro.nsta].name, buffer, i+1);
        //rempli metro.stations[metro.nsta]->line
        if(!init_station_line(file))
            return 0;
        //ajoute elt au graph
        if(!init_station_graph(file, final_size, mode))
            return 0;
        metro.nsta++;
    }
    if((metro.nsta != graph_list.nb && (mode == SUCC_LIST || mode == COMPARE)) || (metro.nsta != matrix.nb && (mode == MATRIX || mode == COMPARE))){
        print_error("Incoherence in graph's nodes and stations number");
        return 0;
    }
    return 1;
}


// Add all the lines a station belongs to and its number on each line
char init_station_line(FILE* file){
    unsigned int i=0, max_buff=metro.nli*4, j;
    char c, *line[metro.nli], buffer[max_buff]; //car une ligne est représenté par 1 lettre, 2 chiffre et 1 séparateur + '\0' final

    // Récupère la chaîne qui représente toutes les lignes auxquelles appartient une station (ex :"C08/H06/M15")
    while((c=fgetc(file)) != ' '){
        if(i>max_buff-2){ //pour laisser la place pour le '\0'
            fprintf(stderr, "%s%sError : %s%sInvalid subway file, invalid station's line format, station %s%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED),  metro.stations[metro.nsta].name, codeFromStyle(RESET));
            return 0;
        }
        buffer[i]=c;
        i++;
    }
    buffer[i]='\0';

    // Sépare les différentes lignes
    i=0;
    line[0] = strtok(buffer, "/");
    for(i = 1; line[i-1] != NULL; i++){
        if(i>=metro.nli){
            fprintf(stderr, "%s%sError : %s%sInvalid subway file, too many lines for one station, station %s%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED),  metro.stations[metro.nsta].name, codeFromStyle(RESET));
            return 0;
        }
        line[i] = strtok(NULL, "/");
    }

    // Ajoute chaque ligne à la liste de lignes de la station en cours de construction
    metro.stations[metro.nsta].lines = getmem(i-1,sizeof(unsigned int[2]));
    metro.stations[metro.nsta].nlines = i-1;
    if (!metro.stations[metro.nsta].lines)
        return 0;
    for(i=0;line[i]; i++){
        if(!isalpha(line[i][0])){
            fprintf(stderr, "%s%sError : %s%sInvalid subway file, wrong station's line format, first character is not a letter, station %s, line %s%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED),  metro.stations[metro.nsta].name, line[i], codeFromStyle(RESET));
            return 0;
        }
        for (j=1; line[i][j]!='\0'; j++)
            if(!isdigit(line[i][j])){
                fprintf(stderr, "%s%sError : %s%sInvalid subway file, wrong station's line format, station's number contain non digit character, station %s, line %s%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED),  metro.stations[metro.nsta].name, line[i], codeFromStyle(RESET));
                return 0;
            };
        if(j==1){
            fprintf(stderr, "%s%sError : %s%sInvalid subway file, wrong station's line format, missing station's number, station %s, line %s%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED),  metro.stations[metro.nsta].name, line[i], codeFromStyle(RESET));
            return 0;
        }
        line[i][0] = toupper(line[i][0]); //for forked lines (like the Marunouchi Line, having a 'M' and 'm' branch
        for(j=0;j<metro.nli;j++)
            if(metro.lines[j].symbol == line[i][0])
                break;;
        if(j == metro.nli){
            fprintf(stderr, "%s%sError : %s%sInvalid subway file, wrong station's line format, line doesn't exists, station %s, line %s%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED),  metro.stations[metro.nsta].name, line[i], codeFromStyle(RESET));
            return 0;
        }
        metro.stations[metro.nsta].lines[i][0]=j;
        metro.stations[metro.nsta].lines[i][1]=(unsigned int) strtoul(line[i]+1,NULL,10);
    }
    return 1;
}


// Add a station to the graph with a list of successors
char init_station_graph(FILE *file, size_t final_size /* number of stations */, RUN_MODE mode){
    unsigned int i = final_size, digits = count_digits_number(final_size), j=0, n;
    char c;
    succ * corr = NULL, *temp=NULL;
    char buffer[digits+1]; // create a buffer which can contain the same number of char as the number of digit of final_size + '\0'

    if(mode == SUCC_LIST || mode == COMPARE)
        graph_list.noeuds[metro.nsta].num=metro.nsta;
    c=fgetc(file);
    while (c!='\n'){
        j++;
        for(i=0 ; !strchr(" \n",c) ; i++, c=fgetc(file)){
            if(!isdigit(c)){
                fprintf(stderr, "%s%sError : %s%sInvalid subway file, transfer %d of station %s is not a number%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED),  j, metro.stations[metro.nsta].name, codeFromStyle(RESET));
                return 0;
            }
            if(i>=digits){
                fprintf(stderr, "%s%sError : %s%sInvalid subway file, the number of the %d%s transfer of station %s is higher than the number of existing stations%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED),  j, ordinal_suffix(j), metro.stations[metro.nsta].name, codeFromStyle(RESET));
                return 0;
            }
            buffer[i]=c;
        }
        buffer[i]='\0';
        if(strlen(buffer)!=0){
            n = (unsigned int) strtoul(buffer,NULL,10);
            if(n >= final_size){
                fprintf(stderr, "%s%sError : %s%sInvalid subway file, the number of the %d%s transfer of station %s is higher than the number of existing stations%s\n",codeFromStyle(BOLD), codeFromStyle(RED), codeFromStyle(RESET), codeFromStyle(RED),  j, ordinal_suffix(j), metro.stations[metro.nsta].name, codeFromStyle(RESET));
                return 0;
            }
            switch(mode){
                case COMPARE:
                    /* NO BREAK */
                case SUCC_LIST:
                    corr = getmem(1,sizeof(succ));
                    if(!corr)
                        return 0;
                    corr->car = graph_list.noeuds + n;
                    corr->cdr = temp;
                    temp = corr;
                    
                    if(mode != COMPARE)
                        break;
                default:
                    matrix.mat[metro.nsta*final_size+n]=1;
            }
        }
        if(c == ' ')
            c=fgetc(file);
    }
    if(mode == SUCC_LIST || mode == COMPARE){
        graph_list.noeuds[metro.nsta].next=corr;
        graph_list.nb++;
    }
    if(mode == MATRIX || mode == COMPARE){
        matrix.nb++;
    }
        
    return 1;
}


/* First try with static arrays

char init_metro(char * filename){
    char buffer [MAX_CHAR_READ], *test, *tokens[MAX_CHAR_READ], *lines[MAX_CHAR_READ];
    size_t buffer_size, name_size;
    FILE * file;
    int i, j;

    file=fopen(filename, "r");
    if(!file){
        perror("fopen");
        return 0;
    }
    metro.nli=0;
    metro.nsta=0;

    // SUBWAY LINES
    while(test=fgets(buffer, MAX_CHAR_READ, file) && buffer[0] != '\n'){
        if(metro.nli>=MAX_LINES){
            fprintf(stderr,"Too many subway lines.\n")
            return 0;
        }
        if(buffer_size=strlen(buffer) < 3 || buffer[1]!='='){
            fprintf(stderr, "Invalid subway line, on line %u\n", metro.nli);
            return 0;
        }
        metro.lines[metro.nli].symbol=buffer[0];
        name_size = buffer_size-2 > MAX_LINE_NAME ? MAX_LINE_NAME : buffer_size-2;
        strncpy(metro.lines[metro.nli].name, buffer+2, name_size);
        metro.lines[metro.nli].name[name_size-1]='\0'; // remplace le '\n' final
        while(buffer[buffer_size-1] != '\n'){
            test=fgets(buffer, MAX_CHAR_READ, file);
            if (!test){
                perror("fgets");
                fprintf(stderr, "Unexpected end of parsing, line %u\n", metro.nli);
                return 0;
            }
            buffer_size=strlen(buffer);
        }
        metro.nli++;
    }

    // SUBWAY STATIONS
    while(test=fgets(buffer, MAX_CHAR_READ, file)){
        if(metro.sta>=MAX_STATIONS){
            fprintf(stderr,"Too many subway stations.\n")
            return 0;
        }
        buffer_size=strlen(buffer);
        name_size = strcspn(buffer, ":");
        if (!name_size || name_size == buffer_size || name_size +2 < buffer_size){
            fprintf(stderr, "Invalid subway station, on line %u\n", metro.nli+1+metro.nsta);
            return 0;
        }
        tokens[0] = strtok(buffer+name_size+1, " ");
        for(i = 1; tokens[i-1] != NULL; i++)
            tokens[i] = strtok(NULL, " ");

        lines[0] = strtok(tokens[0], "/");
        for(i = 1; lines[i-1] != NULL; i++)
            lines[i] = strtok(NULL, "/");

        for(i = 0; lines[i] != NULL; i++){
            if(strlen(lines[i])<2){
                fprintf(stderr, "Invalid subway station, on line %u\n", metro.nli+1+metro.nsta);
                return 0;
            }
            for (j=0 ; j < metro.nli || metro.lines[j].symbol != lines[i][0] ; j++);
            if (j == metro.nli){
                fprintf(stderr, "Invalid subway station, on line %u, subway line not found\n", metro.nli+1+metro.nsta);
                return 0;
            }
            metro.stations[metro.nsta].lines[i][0] = j;
            metro.stations[metro.nsta].lines[i][1] = (unsigned char) strtoul(lines[i]+1,NULL,10);
        }

        if (name_size>MAX_STATION_NAME) name_size=MAX_STATION_NAME;
        strncpy(metro.stations[metro.nsta].name, buffer, name_size);
        metro.nsta++;
    }
}
*/