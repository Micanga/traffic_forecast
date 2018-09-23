#ifndef _FORESIGHT_UTILS_H_
#define _FORESIGHT_UTILS_H_

int startmonthweekday(int );

int endmonthday(int );

int msdtrainning( int , int , int , int );

FILE *monthfile_open(int , char *);

double *vetorizeDayflow(FILE *file);

#endif
