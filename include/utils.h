#ifndef _UTILS_H_
#define _UTILS_H_

//----------------------------- libs ----------------------------
#include <math.h>
#include <string.h>
#include "match.h"

//----------------------------- enums ----------------------------
enum boolean{
    INVALID = -1,
    FALSE,
    TRUE 
}bool;

enum months{
  JAN = 0,
  FEB,
  MAR,
  APR,
  MAY,
  JUN,
  JUL,
  AUG,
  SEP,
  OCT,
  NOV,
  DEC
}month;










//----------------------------- statistical/mathematical functions ----------------------------
/*
*	double mean(double *vec, int start, int end);
* This function calculates the arithmetic mean of a real number vector
* @return mean
* @args number_vector, start_vector_index, end_vector_index
*/
double mean(double *, int , int );

/*
* double stdDeviation(double *vec, double mean, int n);
* This function calculates the standard deviation of a real number vector
* @return std_deviation
* @args number_vector, mean, number_of_elements
*/
double stdDeviation(double *, double , int );

/*
*	double leastSquaresNextPoint(double *x, double *y, int start, int end );
* This function calculates the value of the next point in a straight fitting by the 
* least squares method.
* @return point_value
* @args x_variables,y_variables,start_index,end_index
*/
double leastSquaresNextPoint(double *, double *, int , int );










//----------------------------- type manipulation functions ----------------------------
/*
*	char *itos(int n);
* This function converts an integer number to string
* @return string
* @args integer
*/
char *itos(int );










//----------------------------- matrix manipulation functions ----------------------------
/*
*	void free_matrix(void **matrix, int nrow);
* This functions frees a matrix from the memory
* @return void
* @args matrix, nrow
*/
void free_matrix(void **, int);

/*
*	void print_dmatrix(double **matrix, int row, int col);
* This function prints a double matrix
* @return void
* @args matrix, nrow, ncol
*/
void print_dmatrix(double **, int , int );

/*
*	void initialize_dmatrix( double ***matrix, int row, int col, double num);
* This function initializes a double matrix
* @return void
* @args: matrix, nrow, ncol, number_to_initialize
*/
void initialize_dmatrix( double ***, int , int , double );

/*
* double **create_dmatrix( int row, int col);
* This function create a double matrix in the heap memory
* @return matrix
* @args nrow, ncol
*/
double **create_dmatrix( int , int );










//----------------------------- string manipulation functions ----------------------------
char *strcopy(char *source);

/*
*    char *tokenize (char *string, int n, char delimiter)
* This function tokenize a string and return the selected token.
* @return token
* @args string, token_pos, delimiter
*/
char *tokenize (char *, int , char );

/*
*    char *readLine(FILE *input_file, char delimiter)
* This function reads a line/string from an input file until finding a delimiter, a 
* lineskip or the end of the file (EOF).
* @return string
* @args inputfile, delimiter
*/
char *readLine(FILE *, char );

/*
*    int readInt(FILE *input_file, int min, int max);
* This function reads a integer from a file
* @return integer
* @args inputfile, min_bound, max_bound
*/
int readInt(FILE *, int, int);










//----------------------------- file manipulation functions ----------------------------
/*
*    int filesize(FILE *file);
* This functions returns the size of a file
* @return file_size
* @args opened_file
*/
int filesize(FILE *);

/*
*    FILE *fopen_(char *filename, char *openmode);
* This function opens a file.
* @return sucess: opened_file; fail: exit(1)
* @args void
*/
FILE *fopen_(char *, char *);

#endif
