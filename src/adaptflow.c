#include <stdlib.h>
#include <stdio.h>
#include <string.h>	
#include <math.h>
#include "utils.h"
#include "highway.h"
#include "weightmethod.h"
#include "adaptflow.h"

#define N_DAYS_OF_WEEK 7
#define N_PERIODS 96

int adaptflow( char *highway_name, int test_day, int test_month, int ntrainning_days, 
    int npredictions, double weight, int npoints, double dweight){
    //0. Variables
    int hour, phour, day, month_endday = 31, index = 0, expected_trainning = ntrainning_days;;
    double F, expected_result, TotalFlow, *errors = NULL,*f, *hour_vec, *pf, ls_next;
    double Error, lsqrError = 0, weigError = 0, totalError = 0;
    FILE *ftesting = NULL;
    t_coef *coef = NULL;
    
    //1. Initializing the necessary variables
    ftesting = monthfile_open(month,highway_name);
    index = startmonthweekday(test_month);
    month_endday = endmonthday(test_month);

    //2. Starting the training phase (Arithmetic or Weighted Method)
    coef = weightedtraining(highway_name,test_day,test_month,&ntrainning_days,weight);

    if(!coef){
    	//printf(":: insuficient data - invalid coef ::\n");
    	if(ftesting) 
            fclose(ftesting);
	   return FALSE;
    }

    //3. Validating training (the number of training days should be correct)
    if(ntrainning_days != expected_trainning){
        //printf(":: insuficient data - %d/%d trainning days::\n",ntrainning_days,expected_trainning);
	    if(coef) 
            destroyCoef(coef);
	    if(ftesting) 
            fclose(ftesting);
        return FALSE;
    }

    //5. Starting TEST (Comparisons)
    //a. Creating the hour vector (used in lsqr method)
    hour_vec = (double *)malloc(sizeof(double)*N_PERIODS);
    for(hour = 0 ; hour < N_PERIODS ; hour++)
        hour_vec[hour] = hour;

    for(day = 1 ; day <= month_endday ; day++){
        //b. Vectorizing day flow
        f = vetorizeDayflow(ftesting);
        pf = (double *)calloc(N_PERIODS,sizeof(double));
        
	    //c. Testing the prediction if is the correct day
	    if(day == test_day){
            //i. initializing the variables
		    Error = 0; TotalFlow = 0;
		    errors = (double *)calloc(N_PERIODS,sizeof(double));
            
		    //ii. running the test
		    for(hour = 0 ; hour < N_PERIODS-npredictions ; hour++){ 
		        TotalFlow += f[hour+npredictions-1];
                expected_result = f[hour+npredictions-1];

			    if(hour != 0){
                    pf[hour-1] = f[hour-1];
                    for(phour = 0 ; phour < npredictions && phour+hour < N_PERIODS ; phour++){
        				//iii. applying the least squares method
        				if(hour+phour < npoints)
        		            ls_next = leastSquaresNextPoint(hour_vec,pf,0,hour+phour);
                        else if(npoints == 0 || hour+phour == N_PERIODS-1)
                            ls_next = f[hour-1];
        		        else
        		            ls_next = leastSquaresNextPoint(hour_vec,pf,hour+phour-npoints,hour+phour);
                            
                        //iv. Calculating the forecast point
                        F = (dweight)*(pf[hour+phour-1]*coef->day[index].coef[hour+phour]) + (1 - dweight)*(ls_next);

        				//v. Updating the prediction flows vector
                        pf[hour+phour] = F;
                    }

                    //vi. Calculating the errors
                    weigError = fabs(expected_result - f[hour-1]*coef->day[index].coef[hour+npredictions-1]);
                    lsqrError = fabs(expected_result - ls_next);

                    //vii. Updating the weight value to improove next prediction
                    totalError = weigError + lsqrError;
                    if(weigError > lsqrError)
                        dweight = 1 - (weigError/totalError);
                    else
                        dweight = (lsqrError/totalError);

                    //viii. Updating the hour 
                    Error += fabs(expected_result - F);
                    errors[hour+phour] = fabs(expected_result - F);
			    }    
  			    else{
                    errors[hour] = 0;
                }
			    
		    }

            //ix. freeing daily errors
            printf("Daily Error = %lf per cent\n",100*Error/TotalFlow);
            free(errors);
	    }
		//d. Freeing the vectorized day flow and advancing a day
        free(f);
        free(pf);
        index = ( index + 1 ) %  N_DAYS_OF_WEEK;
    }

    //5. Finishing the test, closing the opened files and freeing the used memory
    if(ftesting) 
        fclose(ftesting);
    destroyCoef(coef);
    free(hour_vec);
    return TRUE;
}
