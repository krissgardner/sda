#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#include "timer.h"
#include "algorithms.h"

void format_print(FILE *fout, char *name, int n, double sec)
{
    fprintf(fout, "%-10s %10d %lf\n", name, n, sec);
}

void generateIntArray(int *v, int n)
{
    for (int i = 0; i < n; i++)
    {
        v[i] = rand() % n;
    }
}

void copyArray(int* source, int* destination, int length) {
    memcpy(destination, source, sizeof(int) * length);
}

int compInt(const void* a, const void* b) {
    if(*(int*)a < *(int*)b) return -1;
    else if(*(int*)a > *(int*)b) return 1;
    return 0;
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

    FILE *fout = fopen("stats2.txt", "w");
    if (fout == NULL)
    {
        fprintf(stderr, "File error\n");
        exit(1);
    }
    double exec_time;

    srand(time(0));


    int *v, *vGen;
    v = (int*)malloc(limit * sizeof(int));
    if (v == NULL) {
        fclose(fout);
        printf("Failed alloc\n");
        exit(1);
    }
    vGen = (int*)malloc(limit * sizeof(int));
    if (vGen == NULL) {
        fclose(fout);
        free(v);
        printf("Failed alloc\n");
        exit(1);
    }

    generateIntArray(vGen, limit);
    for (int i = 10; i <= limit; i *= 10)
    {
        copyArray(vGen, v, i);
        starton();
        qsort(v, i, sizeof(int), compInt);
        exec_time = startoff();
        format_print(fout, "Quicksort", i, exec_time);

        copyArray(vGen, v, i);
        starton();
        heapSort(v, i);
        exec_time = startoff();
        format_print(fout, "Heapsort", i, exec_time);
    }

    free(v);
    free(vGen);
    fclose(fout);
    return 0;
}