long double fibonacciRec(int n)
{
    if (n <= 1)
        return n;
    return fibonacciRec(n - 2) + fibonacciRec(n - 1);
}

long double fibonacci(int n)
{
    long double i,
        t1 = 0, t2 = 1, next;

    for (i = 0; i < n; i++)
    {
        next = t1 + t2;
        t1 = t2;
        t2 = next;
    }

    return t1;
}