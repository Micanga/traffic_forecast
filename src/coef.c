#include <stdlib.h>
#include <stdio.h>
#include "coef.h"

void destroyCoef( t_coef *coef){
    int counter;
    for(counter = 0 ; counter < 7 ; counter++){
        free(coef->day[counter].coef);
    }
    free(coef->day);
    free(coef);
    return;
}

t_coef *createCoef(){
    //1. Creating the coeficients
    int day, hour;
    t_coef *coef = NULL;

        coef = (t_coef *)malloc(sizeof(t_coef));
        coef->day = (t_day *)malloc(sizeof(t_day)*7);
    
    //2. Initializing the coeficients
    for(day = 0 ; day < 7 ; day++){
        coef->day[day].coef = (double *)malloc(sizeof(double)*96);
	   for(hour = 0 ; hour < 96 ; hour++)
	    coef->day[day].coef[hour] = 1.0;
    }
    
    return coef;
}
