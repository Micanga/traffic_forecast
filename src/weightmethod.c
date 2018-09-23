#include <stdlib.h>
#include <stdio.h>
#include <string.h>	
#include <math.h>
#include "utils.h"
#include "highway.h"
#include "weightmethod.h"

#define N_DAYS_OF_WEEK 7
#define START_PERIOD 1
#define N_PERIODS 96

t_coef *weightedtraining(char *highway_name, int test_day, int test_month, int *ntrainning_days, double weight){
    //0. Variables
    t_coef *coef = NULL;
    double *f = NULL;
    int month, day = 0, hour = 0, index = 0, ntests = 0;
    int month_start_day_trainning = 0, month_end_day = 0;
    FILE *ftrainning = NULL;

    //1. Creating and initializing the support structures to make the training
    coef = createCoef();

    //2. Starting TRAINING
    for(month = JAN ; month <= MAR ; month++){    
        //a. Opening the training files
        ftrainning = monthfile_open(month,highway_name);                                        //monthfile
        index = startmonthweekday(test_month);                                                  //start weekday
        month_end_day = endmonthday(test_month);                                                //end month day
        month_start_day_trainning = msdtrainning(test_day,test_month,*ntrainning_days,month);   //first training day
        
        //b. Verifying the existence of opened file
    	if(ftrainning && (filesize(ftrainning) > 1)){
            for(day = START_PERIOD ; day <= month_end_day && (day < test_day || month < test_month) ; day++){
                //i. Vectorizing day flow
                f = vetorizeDayflow(ftrainning);

                //ii. Verifying the day (if is a training day)
                if(day >= month_start_day_trainning){
        		    if( day % N_DAYS_OF_WEEK == test_day % N_DAYS_OF_WEEK) 
                        ntests += 1;
                    
        	        coef->day[index].coef[0] = 1;
        
                    //iii. TRAINING
                    for(hour = 1 ; hour < N_PERIODS; hour++)
                        coef->day[index].coef[hour] = 1 + ((f[hour] - f[hour-1])/f[hour-1]);
                    *ntrainning_days -= 1;
                }
        		//iv. Freeing the vectorized day flow and advancing a day
        		if(f) 
                 free(f);
    	        index = (index + 1) % N_DAYS_OF_WEEK;
    	    }
        }
        
        //c. Closing the month file
        if(ftrainning) 
            fclose(ftrainning);
    }
    
    //3. Checking if at least test was performed
    if(ntests == 0){
        destroyCoef(coef);
        return NULL;
    }

    *ntrainning_days = ntests;

    //4. Finishing the training and freeing the used memory
    return coef;
}