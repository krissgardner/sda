#include "helpers.h"

void shakersort(int *v, int n)
{
    int p, i;
    for (p = 1; p <= n / 2; p++)
    {
        for (i = p - 1; i < n - p; i++)
        {
            if (v[i] > v[i + 1])
            {
                swap(v + i, v + i + 1);
            }
        }
        for (i = n - p - 1; i >= p; i--)
        {
            if (v[i] < v[i - 1])
            {
                swap(v + i, v + i - 1);
            }
        }
    }
}