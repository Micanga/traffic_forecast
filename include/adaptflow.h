#ifndef _HYBRIDMETHOD_H_
#define _HYBRIDMETHOD_H_

#include "coef.h"
#include "foresight_utils.h"

/*
*	int adaptflow(char *highway_name, int test_day, int test_month, 
    				int ntrainning_days, int nprediction, double weight, int npoints, double dweight);	
* This function realizes the training of a highway traffic coefficients.
* The coefficients are defined by an hybrid trainning with the weighted method working together with
* the least square method (near x far past).
* @return boolean result_flag
* @args highway_name, test_day, test_month, number_of_training_days, weight_to_test, n_points_of_lsqrmethod and dynamic_weight.
*/
int adaptflow( char *, int , int , int , int, double , int  , double);

#endif
