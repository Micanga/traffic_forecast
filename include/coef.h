#ifndef _COEF_H_
#define _COEF_H_

typedef struct day t_day;
struct day{
    double *coef;
};

typedef struct coef t_coef;
struct coef{
    t_day *day;
};

/*
*	void destroyCoef( t_coef *coefs)
* This function destroys a coefficient structure.
* @return void
* @args highway_coef
*/
void destroyCoef( t_coef *);

/*
*	t_coef *createCoef()
* This function creates a coefficient structure.
* @return void
* @args void
*/
t_coef *createCoef();

#endif
