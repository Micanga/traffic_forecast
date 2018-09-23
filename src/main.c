#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "utils.h"
#include "highway.h"
#include "adaptflow.h"

#define BEST_NPOINTS 5
#define BEST_DWEIGHT 0.60
#define BEST_WEIGHT 0.60
#define BEST_N_TRAINNING_DAYS 3
#define NPREDICTIONS 1
#define N_HIGHWAYS 2510

int existAL(char *string, t_highway *link_info){
    int i = 0;
    for(i = 0 ; i < N_HIGHWAYS ; i++){
	if(strcmp(string,link_info[i].link_ref) == 0)
	    return TRUE;
    }
    return FALSE;
}

// ADAPTFLOW METHOD
void adaptFlowMethodTest(int nfiles, t_highway *link_info){
    int counter = 0, day, month, sucess_flag = TRUE;
    int npoints, ntrainning_days;
    int n_start_points = 0, n_end_points = 0;
    double weight = 0, dweight = 0;
    double startweight = 0, endweight = 0;
    double startdweight = 0, enddweight = 0;
    char *link_name = NULL;	
    FILE *ftesting = NULL;

    clock_t start_time = 0, end_time = 0;
   
    //1. Starting the test process
	while(counter++ < nfiles){
		//a. Defining the links to test
		printf(" Prediction number (%d/%d) - Adapt Flow Method Test\n",counter,nfiles);
	    if(nfiles != N_HIGHWAYS){
	        printf("- Enter link reference: ");
	        link_name = readLine(stdin,10);
	    }
	    else{
	    	link_name = strcopy(link_info[counter-1].link_ref);
	    	printf("- Standard name number %d: %s.\n",counter,link_name);
	    }
	        
		//b. Checking the link existence
		if(existAL(link_name,link_info)){ 
		    //c. Defining the number of training days in process
	        if(nfiles != N_HIGHWAYS){
	        	printf(" - Enter the number of traning days: ");
	        	ntrainning_days = readInt(stdin,1,5);
	        }
	        else{
	        	printf(" - Standard value to number of training days: %d\n",BEST_N_TRAINNING_DAYS);
	        	ntrainning_days = BEST_N_TRAINNING_DAYS;
	        }

	        //d. Defing the dynamic weight of the test process
	        if(nfiles != N_HIGHWAYS){
	        	printf(" - Enter the start test dynamic weight: ");
	        	scanf("%lf%*c",&startdweight);
	        	printf(" - Enter the end test dynamic weight: ");
	        	scanf("%lf%*c",&enddweight);
	        }
	        else{
	        	printf(" - Standard value to dynamic weight: %.2lf\n",BEST_DWEIGHT);
	        	startdweight = BEST_DWEIGHT;
	        	enddweight = BEST_DWEIGHT;
	        }

	        //e. Defing the npoints of lsm process
	        if(nfiles != N_HIGHWAYS){
	        	printf(" - Enter the start npoints: ");
	        	scanf("%d%*c",&n_start_points);
	        	printf(" - Enter the end npoints: ");
	        	scanf("%d%*c",&n_end_points);
	        }
	        else{
	        	printf(" - Standard value to npoints: %d\n",BEST_NPOINTS);
	        	n_start_points = BEST_NPOINTS;
	        	n_end_points = BEST_NPOINTS;
	        }

	        //f. Defing the weight of the test process
			if(nfiles != N_HIGHWAYS){
				printf(" - Enter the start test weight: ");
				scanf("%lf%*c",&startweight);
				printf(" - Enter the end test weight: ");
				scanf("%lf%*c",&endweight);
			}
			else{
				printf(" - Standard value to weight: %.2lf\n",BEST_WEIGHT);
				startweight = BEST_WEIGHT;
				endweight = BEST_WEIGHT;
			}
				    
			//g. Starting Testing Phase
			for(month = JAN ; month <= MAR ; month++){
				//i. Opening the necessary file
			  	ftesting = monthfile_open(month,link_name);

				//ii. Verifying the file existence
				if(ftesting && (filesize(ftesting) >= 1)){
					start_time = clock();
					for(day = 1 ; day < 31 ; day++){
				    	for(weight = startweight; weight <= endweight && sucess_flag ; weight += 0.05){
							for(dweight = startdweight; dweight <= enddweight && sucess_flag ; dweight += 0.05){
			                    for(npoints = n_start_points ; npoints <= n_end_points && sucess_flag ; npoints++){
									//printf("%s testing %2d/%2d with %d points - weight %.2lf - dweight %.2lf\n",link_name, day,month+1, npoints, weight, dweight);
					                sucess_flag = adaptflow(link_name,day,month,ntrainning_days,NPREDICTIONS,weight,npoints,dweight);
				                }
							}
				        }
				   		sucess_flag = TRUE; 

				   		if(day == 30){
	          				end_time = clock();
					   		double time = (end_time - start_time)/(double)CLOCKS_PER_SEC;
	    					printf("RUN TIME: %lf\n", time);
	    					start_time = clock();
	    				}
			    	}
				}
			}
		}
		else{
		    printf("Invalid link_ref, try another.\n");
		    counter--;
		}
	    //h. Freeing the link name in the memory 
	    free(link_name);
	}

    //2. Returning to main function
    return;
}

int main(int argc, char **argv){
    printf(":: TRAFFIC STUDY AND PREDICTIONS SYSTEM ::\n\n");
    //0. Variables
    int nfiles = 0;
    FILE *link_coordenates_file = NULL;
    t_highway *link_info = NULL;
    
    //1. Choosing the number of files to prediction
    printf(" How many predictions do you want?\nEnter 2510 to run all data set.\n-");
    nfiles = readInt(stdin,0,N_HIGHWAYS);
    
    //2. Collecting the necessary informations to realize any prediction
    link_coordenates_file = fopen_("all_data/link-coordinates","r");
    link_info = createLinkCoordenates(link_coordenates_file,N_HIGHWAYS);
    
    //3. Starting the predictions
    printf("--- Start predictions for %d files ---\n",nfiles);
    adaptFlowMethodTest(nfiles,link_info);

    //4. Closing the opened files and freeing the used memory
    printf("::  Foresight: Complete! Adapt Flow Method                     ::\n");
	printf("::  Thanks for using the Traffic Study and Predictions System! ::\n");
    printf(":: Matheus Aparecido do Carmo Alves - Universidade de Sao Paulo::\n");
    printf(":: Prof. Dr. Robson Leonardo Ferreira Cordeiro                 ::\n");
    destroyHighway(link_info,N_HIGHWAYS);
    fclose(link_coordenates_file);
    return 0;
}
