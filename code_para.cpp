#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<omp.h>
#include<sys/time.h>
#include<time.h>
using namespace std;

double GetTime() 
{
	struct timeval now;
	gettimeofday(&now,NULL);
	return now.tv_sec+now.tv_usec/1000000.0;
}

void Input(int &choose)
{
    printf("Please Choose Parallel or Serial:\n");
    printf("1. Serial\n");
    printf("2. Parallel\n");
    printf("Input = ");
    scanf("%d", &choose);

}
void PrintMatrix (double* result, int RowCount, int ColCount) {
    int i, j; 
    for (i=0; i<RowCount; i++) {
    for (j=0; j<ColCount; j++)
    printf("%7.4f ", result[i*ColCount+j]);
    printf("\n");
    }
}

void RandomDataInitialization(double* pMatrix, int Size) {
    int i, j; 
    time_t t;
    srand((unsigned) time(&t) );
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

void ProcessInitialization (double* &pMatrix, int &Size) {
    pMatrix = new double [Size*Size];
    RandomDataInitialization(pMatrix, Size);
}

double SerialMinMax(double *pMatrix, int size)
{
    int i,j;
    double max = -10e4, min = -10e4;
    for (i = 0; i < size; i++)
    {
        min = pMatrix[i *size + 0];
        for (j = 1; j < size; j++)
        {
            if (min > pMatrix[i*size+ j])
                min = pMatrix[i*size+ j];
        }
        if (max < min)
            max = min;
    }
    return max;
}

double ParallelMinMax(double *pMatrix, int size, int threads)
{
    int i,j;
    double max = -10e4, min = -10e4;
    omp_set_num_threads(threads);
    #pragma omp parallel for private(i, j)
    for (i = 0; i < size; i++)
    {
        min = pMatrix[i *size + 0];
        for (j = 1; j < size; j++)
        {
            if (min > pMatrix[i*size+ j]) 
                min = pMatrix[i*size+ j];
        }
        if (max < min)
            max = min;
    }
    return max;
}

void TimeParallel(int* arrayTestSize, int *arrayNumThreads, double * &result,  int numTest, int numThreads)
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
            ProcessInitialization(pMatrix, size);
            Start = GetTime();
            ParallelMinMax(pMatrix, size, threads);
            Finish = GetTime();
            result[i*numThreads +j] = Finish - Start;
        }
        delete [] pMatrix;
    } 
}

void TimeSerial(int* arrayTestSize, double * &result,  int numTest)
{
    double Start, Finish;
    result = new double[numTest];

    for(int i = 0; i < numTest; i++)
    {
        double* pMatrix;
        int size = arrayTestSize[i];
        ProcessInitialization(pMatrix, size);
        Start = GetTime();
        SerialMinMax(pMatrix, size);
        Finish = GetTime();
        result[i] = Finish - Start;
        delete [] pMatrix;
    }
}

int main()
{
    int arrayTestSize[] = { 1000, 2000, 3000, 4000, 5000,6000,7000,8000,9000,10000 };
    int arrayNumThreads[] = {2, 4, 6 , 8};
    int arrSize = sizeof(arrayTestSize)/4, numThreads = sizeof(arrayNumThreads)/4;
    double *resultPara;
    double *resultSerial;
    int choose;
    Input(choose);
    if (choose ==1)
    {
        TimeSerial(arrayTestSize,resultSerial,arrSize);
        PrintMatrix(resultSerial,arrSize,1);
    }
    else
    {
        TimeParallel(arrayTestSize, arrayNumThreads, resultPara, arrSize, numThreads);
        PrintMatrix(resultPara, arrSize, numThreads);
    }
    delete [] resultPara;
    delete [] resultSerial;
}
