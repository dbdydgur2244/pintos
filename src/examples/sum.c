#include <stdio.h>
#include <syscall.h>
#include <stdlib.h>
/* use pibonacci
 * use sum of four numbers
 * Usage : ./sum [num1] [num2] [num3] [num4]
 * print : pibonacci[num1] sum_of_four_integer(num1~4)*/

int
main(int argc, char *argv[])
{
    int a,b,c,d;
    
    if(argc < 5) return EXIT_FAILURE;
    a = atoi(argv[1]);
    if(a<1) return EXIT_FAILURE;

    b = atoi(argv[2]);
    c = atoi(argv[3]);
    d = atoi(argv[4]);

    printf("%d %d\n",pibonacci(a),sum_of_four_integers(a,b,c,d));
    return EXIT_SUCCESS;
}
