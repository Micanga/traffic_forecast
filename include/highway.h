#ifndef _HIGHWAY_H_
#define _HIGHWAY_H_

typedef struct highway t_highway;
struct highway{
    char *line;
    char *link_ref;
    char *link_match;
    char *link_description;
    char *start_junction;
    char *end_junction;
    double start_x;
    double end_x;
    double start_y;
    double end_y;
};

/*
* void destroy_highway( t_highway * highways, int nlinks)
* This function destroys a highway struct from memory
* @args highway_vector, n_highways
* @return void
*/
void destroyHighway( t_highway *, int );

/*
* t_highway *create_link_coordenates( FILE *source, int nlinks);
* This functions takes info of highways into a file
* @args source_file, n_highways
* @return highway_vector
*/
t_highway *createLinkCoordenates( FILE *, int );

#endif
