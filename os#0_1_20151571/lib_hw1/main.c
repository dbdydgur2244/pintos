#include <stdio.h>
#include "main.h"

int main(void){
    char input[256];
    char *token;
    char *argu[8];
    int arnum;
    while(1){
        argnum = 0;
        fgets(input,sizeof(input), stdin);
        while( (token = strtok(input) ) != NULL ){
            argu[argnum++] = token;
        }
    } 
    return 0;
}
