#include <time.h>
#include "timer.h"

clock_t aux;
void starton(void)
{
    aux = clock();
}

float startoff(void)
{
    return ((float)clock() - (float)aux) / CLOCKS_PER_SEC;
}