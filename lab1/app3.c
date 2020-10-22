#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "timer.h"
#include "algorithms.h"

void format_print(FILE *fout, char *name, int n, double sec)
{
    fprintf(fout, "%-14s %4d %lf\n", name, n, sec);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Expecting one number\n");
    }
    int limit = atoi(argv[1]);
    if (limit == 0)
    {
        fprintf(stderr, "Argument cannot be 0\n");
    }

    FILE *fout = fopen("stats3.txt", "w");
    if (fout == NULL)
    {
        fprintf(stderr, "File error\n");
        exit(1);
    }
    double exec_time;

    for (int i = 0; i < limit; i += 5)
    {
        starton();
        fibonacciRec(i);
        exec_time = startoff();
        format_print(fout, "Fibonacci rec", i, exec_time);

        starton();
        fibonacci(i);
        exec_time = startoff();
        format_print(fout, "Fibonacci", i, exec_time);
    }

    fclose(fout);
    return 0;
}