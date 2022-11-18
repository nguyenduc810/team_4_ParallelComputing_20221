#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <omp.h>
#include <time.h>
using namespace std;

double LiToDouble (LARGE_INTEGER x) {
double result = 
 ((double)x.HighPart) * 4.294967296E9 + (double)((x).LowPart);
 return result;
}
// Function that gets the timestamp in seconds
double GetTime() {
 LARGE_INTEGER lpFrequency, lpPerfomanceCount;
 QueryPerformanceFrequency (&lpFrequency);
 QueryPerformanceCounter (&lpPerfomanceCount);
 return LiToDouble(lpPerfomanceCount)/LiToDouble(lpFrequency);
}

void PrintMatrix (double* result, int RowCount, int ColCount) {
    int i, j; 
    for (i=0; i<RowCount; i++) {
    for (j=0; j<ColCount; j++)
    printf("%7.4f ", result[i*ColCount+j]);
    printf("\n");
    }
}

void RandomDataInitialization(double* pMatrix, int Size, int threads) {
    int i, j; 
    time_t t;
    srand((unsigned) time(&t) );
    omp_set_num_threads(threads);
    #pragma omp parallel for
    for (i=0; i<Size; i++) {
        for (j=0; j<Size; j++)
            pMatrix[i*Size+j] = rand()/ double(100);
    }
    //PrintMatrix(pMatrix, Size, Size);
}
void pickNumThreads(int threads)
{
    printf("Choose the number of threads: ");
    scanf("%d", &threads);
}

void ProcessInitialization (double* &pMatrix, int &Size, int threads) {
    pMatrix = new double [Size*Size];
    RandomDataInitialization(pMatrix, Size, threads);
}

double ParallelMinMax(double *pMatrix, int size, int threads)
{
    double max = -10e4, min = -10e4;
    omp_set_num_threads(threads);
    int i,j;
    #pragma omp parallel for private (i,j)
    for (i = 0; i < size; i++)
    {
        //min = pMatrix[i *size + 0];
        for (int j = 0; j < size; j++)
        {
            if (min > pMatrix[i*size+ j]) 
                min = pMatrix[i*size+ j];
        }
        if (max < min)
            max = min;
    }
    return max;
}

void test(int* arrayTestSize, int *arrayNumThreads, double * &result,  int numTest, int numThreads)
{
    double Start, Finish;
    result = new double[numTest * numThreads];

    for(int i = 0; i < numTest; i++)
    {
        double* pMatrix;
        for (int j = 0; j < numThreads; j++)
        {
            int size = arrayTestSize[i];
            int threads = arrayNumThreads[j];
            ProcessInitialization(pMatrix, size, threads);
            Start = GetTime();
            ParallelMinMax(pMatrix, size, threads);
            Finish = GetTime();
            result[i*numThreads +j] = Finish - Start;
        }
        delete [] pMatrix;
    } 
}

int main()
{
    int arrayTestSize[] = { 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };
    int arrayNumThreads[] = {1, 4, 6, 8};
    int arrSize = sizeof(arrayTestSize)/4, numThreads = sizeof(arrayNumThreads)/4;
    double * result;
    test(arrayTestSize, arrayNumThreads, result, arrSize, numThreads);
    PrintMatrix(result, arrSize, numThreads);
    delete [] result;
}