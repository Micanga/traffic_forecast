#ifndef _WEIGHTMETHOD_H_
#define _WEIGHTMETHOD_H_

#include "coef.h"
#include "foresight_utils.h"

/*
*	t_coef *weightedtraining(char *highway_name, int test_day, int test_month, int *ntrainning_days, double weight)
* This function realizes the training of a highway traffic coefficients.
* The coefficients are defined by the weighted mean of bests daily/hourly coefficients found in 
* training process.
* @return t_coef highway_coef
* @args highway_name, test_day, month_test, number_of_training_days, weight_to_test	
*/
t_coef *weightedtraining(char *, int , int , int *, double);

#endif
