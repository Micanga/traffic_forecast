#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

#define TRUE 1
#define FALSE 0

int match(char *str, char *pattern){
    int status;
    regex_t re;
    if(regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0)
        return FALSE;

    status = regexec(&re, str, (size_t) 0, NULL, 0);
    regfree(&re);

    if(status != FALSE)
        return FALSE;

    return TRUE;
}