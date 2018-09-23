#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "highway.h"

void destroyHighway( t_highway *AL , int nlinks ){
    int i;
    for( i = 0 ; i < nlinks ; i++){
        if(AL[i].line) free(AL[i].line);
        if(AL[i].link_ref) free(AL[i].link_ref);
        if(AL[i].link_match) free(AL[i].link_match);
        if(AL[i].link_description) free(AL[i].link_description);
        if(AL[i].start_junction) free(AL[i].start_junction);
        if(AL[i].end_junction) free(AL[i].end_junction);
    }
    free(AL);
    return;
}

static char *createLinkMatch(char *string){
    int i;
    char *link_match = (char *)malloc(sizeof(char)*(strlen(string)+2));

    for( i = 0 ; i < strlen(string) ; i++)
	link_match[i] = string[i];

    link_match[i++] = ',';
    link_match[i++] = '\0';
    return link_match;
}

t_highway *createLinkCoordenates( FILE *link_coordenates, int nlinks ){
    int i;
    char *aux = NULL;
    t_highway *AL = (t_highway *)malloc(sizeof(t_highway)*nlinks);
    
    for( i = 0 ; i < nlinks ; i++ ){
        AL[i].line = readLine(link_coordenates,10);
        AL[i].link_ref = tokenize(AL[i].line,0,';');
	    AL[i].link_match = createLinkMatch(AL[i].link_ref);
        AL[i].link_description = tokenize(AL[i].line,1,';');
        AL[i].start_junction = tokenize(AL[i].line,2,';');
        AL[i].end_junction = tokenize(AL[i].line,3,';');
		aux = tokenize(AL[i].line,4,';');
        AL[i].start_x = atof(aux);
		free(aux);
		aux = tokenize(AL[i].line,5,';');
        AL[i].start_y = atof(aux);
		free(aux);
		aux = tokenize(AL[i].line,6,';');
        AL[i].end_x = atof(aux);
		free(aux);
		aux = tokenize(AL[i].line,7,';');
        AL[i].end_y = atof(aux);  
		free(aux);     
    }
    
    return AL;
}
