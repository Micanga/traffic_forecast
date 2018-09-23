#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

double mean(double *x, int start, int end){
	int i = 0;
	double sum = 0;
	for(i = start ; i < end ; i++) 
	    sum += x[i];
	return sum / (end - start);
}

double stdDeviation(double *vec, double mean, int n){
    int i;
    double sum = 0, deviation = 0;

    for(i = 0 ; i < n ; i++){
        sum += pow(vec[i] - mean,2);
    }
    deviation = sqrt(sum/n);
    return deviation;
}

double leastSquaresNextPoint(double *x, double *y, int start, int end){
	//0. Variables
	int i = 0;
	double a = 0, b = 0;
	double xma = 0, yma = 0;
	double sumA = 0, sumB = 0;
    double result = 0;
	

	//1. Verifying some necessary properties to apply the least squares method
	if(end - start == 1) 
        return y[start];
	if(end - start == 2){
	    double m = (y[end-1] - y[start]) / (x[end-1] - x[start]);
	    return m*(x[end] - x[start]) + y[start];
    } 

	//2. Calculating the arithmetic mean of axis x and y
	xma = mean(x,start,end);
	yma = mean(y,start,end);

	//3. Calculating the fitting coeficients
	for( i = start ; i < end ; i++ ){
	    sumA += ((x[i] - xma)*y[i]);
	    sumB += pow((x[i] - xma),2);
	}

	a = (sumA/sumB);
	b = (yma - (a*xma));
	
	//4. Defining the next point and returning it
    result = a*x[end] + b; 
	return result;
}











char *itos(int n){
    int i = 0, counter = 0;
    int len = (n != 0)?((int)log10(n)):(0);
    char *string = (char *)malloc(sizeof(char)*((len+2)));

    for(counter = len; counter >= 0 ; counter--){
        string[i++] = (n / (int) pow(10,counter)) + '0';
        n -= (n / (int) pow(10,counter)) * (int) pow(10,counter);
    }
    string[i] = '\0';
    return string;
}











void free_matrix(void **matrix, int row){
    int i;
    for(i = 0 ; i < row ; i++)
        free(matrix[i]);
    free(matrix);
    return;
}

void print_dmatrix(double **matrix, int row, int col){
    int i, j;
    for( i = 0 ; i < row ; i++ ){
        for( j = 0 ; j < col ; j++ ){
	    printf("%.3lf ",matrix[i][j]);
	}printf("\n");
    }
    return;
}

void initialize_dmatrix( double ***matrix, int row, int col, double num ){
    int i, j; 
    for(i = 0 ; i < row ; i++)
        for(j = 0 ; j < col ; j++)
            matrix[0][i][j] = num;
    return;
}

double **create_dmatrix( int row, int col){
    int i;
    double **matrix = (double **)malloc(sizeof(double *)*row);
    for(i = 0 ; i < row ; i++)
        matrix[i] = (double *)malloc(sizeof(double)*col);
    return matrix;
}










char *strcopy(char *source){
    int i = 0;
    char *string = (char *)malloc(sizeof(char)*(strlen(source)+1));
    
    do{
        string[i] = source[i];
    }while(source[i++] != '\0');

    return string;
}

char *tokenize ( char *string, int n, char delim){
    char c = string[0], *token = NULL;
    int counter = 0, i = 0;
    
    if(string[i] == ';' || string[i] == ',' || string[i] == delim)
        while((string[i] == ';' || string[i] == ',' || string[i] == delim) && (i < strlen(string))) i++;
        
    while(counter < n && (i < strlen(string))){
        while(string[i] != ';' && string[i] != ',' && string[i] != delim) i++;
        if(string[i] == ';' || string[i] == ',' || string[i] == delim) i++;
        counter++;
    }
    
    if(i >= strlen(string)) return NULL;
    
    counter = 0;
    do{
        c = string[i++];
        if(c != ';' && c != ',' && c != delim){
            token = (char *)realloc(token,sizeof(char)*counter+1);
            token[counter++] = c;
        }
    }while(c != ';' && c != ',' && c != delim && c != '\0' && c != EOF);
                
    token = (char *)realloc(token,sizeof(char)*counter+1);
    token[counter++] = '\0';
    
    return token;
}

char *readLine(FILE *fp, char delimiter) {
    char c = 0, *string = NULL;
    int counter = 0;
    
    do{
        c = fgetc(fp);
        string = (char *)realloc(string,sizeof(char)*(counter+1));
        string[counter++] = c;
    } while(c != 10 && c != 11 && c != 12 && c != 13 && c != delimiter && c != EOF);
    string[counter-1] = '\0';
    
    return string;
}

int readInt(FILE *fp, int min, int max){
    int num = INVALID;
    char *string = NULL;

    while(num == INVALID){
        string = readLine(fp, 10);

        if(string){
            if(match(string,"^\\s*[a-zA-Z ]*[0-9]+\\s*[a-zA-Z ]*$")){
                num = atoi(string);
            }
            if(num < min || num > max) 
		num = INVALID;
            free(string);
        }

        if(num == INVALID) 
	    printf("Invalid value. Try: %d < input < %d.\n", min, max);
    }
    return num;    
}











int filesize(FILE *fp){
    int size, curPos = ftell(fp);

    fseek(fp,0,SEEK_END);
    	size = ftell(fp);
    fseek(fp,curPos,SEEK_SET);

    return size;
}

FILE *fopen_(char *filename, char *openmode){
    FILE *fp = fopen(filename,openmode);
    if(!fp){
        printf(":: invalid file %s ::\n",filename);
        exit(1);
    }
    return fp;
}
