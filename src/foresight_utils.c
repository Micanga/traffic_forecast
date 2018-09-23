#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

int startmonthweekday(int month){
    if(month == JAN)
	   return 4;
    return 0;
}

int endmonthday(int month){
    if(month == FEB) 
        return 28;
    else if((month+1) % 2 == 0) 
        return 31;
    return 30;
}

int msdtrainning( int test_day, int test_month, int ntrainning_days, int month){
    int startday = test_day;
    //1. Calculando o dia que o treino comecara...
    while(ntrainning_days-- > 0){
        startday -= 7;
        if(startday <= 0){
            startday = endmonthday(test_month-1) + startday;
            if(test_month == month) return 1;
            else test_month = (test_month == 0) ? DEC : test_month-1;
        }
    }
    if(test_month != month) 
        return 35;
    return startday;
}

FILE *monthfile_open(int month, char *highway_name){
    FILE *fmonth = NULL;
    char *string = NULL;
    switch(month){
        case JAN:
            //abrindo o arquivo de treino
            string = (char *)malloc(sizeof(char)*(strlen(highway_name)+13));
            strcpy(string,"all_data/");
            strcat(string,highway_name);
            strcat(string,"Jan");
            fmonth = fopen(string,"r");
            free(string);
        break;
            
        case FEB:
            //abrindo o arquivo de treino
            string = (char *)malloc(sizeof(char)*(strlen(highway_name)+13));
            strcpy(string,"all_data/");
            strcat(string,highway_name);
            strcat(string,"Feb");
            fmonth = fopen(string,"r");
            free(string);
        break;
            
        case MAR:
            //abrindo o arquivo de treino
            string = (char *)malloc(sizeof(char)*(strlen(highway_name)+13));
            strcpy(string,"all_data/");
            strcat(string,highway_name);
            strcat(string,"Mar");
            fmonth = fopen(string,"r");
            free(string);
        break;
    }
    if(fmonth)
        fseek(fmonth,0,SEEK_SET);
    return fmonth;
}

double *vetorizeDayflow(FILE *file){
    int h = 0;
    char *aux = NULL, *string = NULL;
    double *flow = NULL;

    flow = (double *)calloc(96,sizeof(double));
    for(h = 0 ; h < 96 ; h++){
         string = readLine(file,10);
         if(string){
             aux = tokenize(string,8,'\0');
             if(aux){
                flow[h] = atof(aux);
                free(aux);
             }
             free(string);
         }
    }
    
    return flow;
}