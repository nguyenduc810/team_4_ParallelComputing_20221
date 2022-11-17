
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <iostream>
#define EPSILON 1e-15

using namespace std;

// time
double Start, Finish, Duration, DurationParallel;

// converts numbers form LongInt type to double type
double LiToDouble(LARGE_INTEGER x)
{
    double result =
        ((double)x.HighPart) * 4.294967296E9 + (double)((x).LowPart);
    return result;
}
//  gets the timestamp in seconds
double GetTime()
{
    LARGE_INTEGER lpFrequency, lpPerfomanceCount;
    QueryPerformanceFrequency(&lpFrequency);
    QueryPerformanceCounter(&lpPerfomanceCount);
    return LiToDouble(lpPerfomanceCount) / LiToDouble(lpFrequency);
}

//cal more exactly
 double mySqrt(int number)
{
    double result = 1.0f;
    while (fabs(result * result - number) / number >= EPSILON)
        result = (number / result - result) / 2 + result;
    return result;
}
const double sqrt5 = mySqrt(5);

// memory allocation
void ProcessInitialization(double *&pFibo, int &Size, int &NumberOfThreads)
{
    // input size
    do
    {
        cout << "\nEnter number n-th of Fibonacci: ";
        cin >> Size;
        if (Size <= 0)
        {
            cout << "\nMust be greater than 0!\n";
        }
    } while (Size <= 0);

    // number of threads
    do
    {
        cout << "\nNumber of threads: ";
        cin >> NumberOfThreads;

        if (NumberOfThreads <= 0)
        {
            cout << "\nNumber of threads must be greater than 0!\n";
        }
    } while (NumberOfThreads <= 0);

    pFibo = new double[Size + 1];
    cout << "===============================================================================================\n";
    for (int i = 0; i < Size + 1; i++)
    {
        pFibo[i] = 0;
    }
}

// print

void PrintFibo(double *&pFibo, int &Size)
{
    cout << "Number no - " << Size << " of Fibonacci: " << pFibo[Size];
}

// serial
void ResultCalculation(double *pFibo, int Size)
{
    int i, j;
    double f1x = 1, f2x = 1;
    for (int i = 1; i < Size + 1; i++)
    {
        f1x = 1;
        f2x = 1;
        for (int j = 1; j <= i; j++)
        {
            f1x = f1x * ((1 + sqrt5) / 2);
            f2x = f2x * ((1 - sqrt5) / 2);
        }
        pFibo[i] = round((f1x - f2x) / sqrt5);
    }
}
// parallel
void ParallelResultCalculation(double *pFibo, int Size)
{
    int i, j;
    double f1x = 1, f2x = 1;
#pragma omp parallel for private(f1x, f2x, i, j)
    for (i = 1; i < Size + 1; i++)
    {
        f1x = 1;
        f2x = 1;
        for (j = 1; j <= i; j++)
        {
            f1x = f1x * (1 + sqrt5) / 2;
            f2x = f2x * (1 - sqrt5) / 2;
        }
        pFibo[i] = round((f1x - f2x) / sqrt5);
    }
}
void TestResult(double *pFibo, int Size)
{
    double *pSerialFibo;
    int equal = 0;
    int i;
    pSerialFibo = new double[Size + 1];
    for (int i = 0; i < Size + 1; i++)

        pSerialFibo[i] = 0;
    Start = GetTime();
    // serial
    ResultCalculation(pSerialFibo, Size);
    Finish = GetTime();
    Duration = Finish - Start;

    for (i = 0; i < Size + 1; i++)
    {
        if (fabs(pFibo[i] - pSerialFibo[i]) > 1e-8)
        {
            equal = 1;
            cout << "Error";
        }
    }
    if (equal == 1)
    {
        cout << "\nThe results of serial and parallel algorithms "
                "are NOT identical. Check your code.\n";
    }
    else
    {
        cout << "\nThe results of serial and parallel algorithms are "
                "identical.\n";
    }
    delete pSerialFibo;
}

int main()
{
    double *pFibo;
    int Size, NumberOfThreads;
    // init
    cout << "***********************************************************************************************************\n";
    ProcessInitialization(pFibo, Size, NumberOfThreads);
    // time execution
    Start = GetTime();
    omp_set_num_threads(NumberOfThreads);
    ParallelResultCalculation(pFibo, Size);
    Finish = GetTime();
    DurationParallel = Finish - Start;

    PrintFibo(pFibo, Size);
    // test case
    TestResult(pFibo, Size);
    // print serial time
    cout << "\nTime of serial execution: "
         << Duration;
    
    for (int i = 1; i <= NumberOfThreads; i++)
    {
        omp_set_num_threads(i);
        Start = GetTime();
        ParallelResultCalculation(pFibo, Size);
        Finish = GetTime();
        DurationParallel = Finish - Start;
        cout << "\nTime of execution parallel with [" << i << " threads] " << DurationParallel;
    }
    delete pFibo;
    getch();
    return 0;
}