#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <omp.h>
#include <time.h>

// Function that converts numbers form LongInt type to double type
double LiToDouble(LARGE_INTEGER x) {
	double result =
		((double)x.HighPart) * 4.294967296E9 + (double)((x).LowPart);
	return result;
}
// Function that gets the timestamp in seconds
double GetTime() {
	LARGE_INTEGER lpFrequency, lpPerfomanceCount;
	QueryPerformanceFrequency(&lpFrequency);
	QueryPerformanceCounter(&lpPerfomanceCount);
	return LiToDouble(lpPerfomanceCount) / LiToDouble(lpFrequency);
}
// Function for simple definition of matrix and vector elements
void DummyDataInitialization(double* pMatrix, double* pMatrix2, int Size) {
	int i, j; // Loop variables
	for (i = 0; i < Size; i++) {
		for (j = 0; j < Size; j++)
			pMatrix[i * Size + j] = i;
	}
    	for (i = 0; i < Size; i++) {
		for (j = 0; j < Size; j++)
            pMatrix2[i * Size +j] = i;
	}
}

// Function for memory allocation and definition of object’s elements
void ProcessInitialization(double*& pMatrix, double*& pMatrix2,
	double*& pResult, int& Size) {
	// Size of initial matrix and vector definition
	do {
		printf("\nEnter size of the initial objects: ");
		scanf("%d", &Size);
		printf("\nChosen objects size = %d\n", Size);
		if (Size <= 0)
			printf("\nSize of objects must be greater than 0!\n");
	} while (Size <= 0);
	// Memory allocation 
	pMatrix = new double[Size * Size];
	pMatrix2 = new double[Size * Size];
	pResult = new double[Size * Size];
	// Definition of matrix and vector elements
	DummyDataInitialization(pMatrix, pMatrix2, Size);
}
// Function for formatted matrix output
void PrintPMatrix(double* pMatrix, int RowCount, int ColCount) {
	int i, j; // Loop variables
    printf("Matrix A:\n");
	for (i = 0; i < RowCount; i++) {
		for (j = 0; j < ColCount; j++)
			printf("%7.4f ", pMatrix[i * RowCount + j]);
		printf("\n");
	}
}
void PrintPMatrix2(double* pMatrix2, int RowCount, int ColCount) {
	int i, j; // Loop variables
	for (i = 0; i < RowCount; i++) {
		for (j = 0; j < ColCount; j++)
			printf("%7.4f ", pMatrix2[i * RowCount + j]);
		printf("\n");
	}
}
// Function for serial matrix-vector multiplication
void SerialResultCalculation(double* pMatrix, double* pMatrix2,
	double* pResult, int Size) {
	int i, j, k ; 
	for (i = 0; i < Size; i++) 
    {
		for (j = 0; j < Size; j++){
            double dot = 0;
			for (k = 0; k < Size; k++) {
					dot += pMatrix[i*Size+k]*pMatrix2[k*Size+j];
				} 
				pResult[i*Size+j ] = dot;
        }
            
	}
}

// Function for parallel matrix-matrix multiplication
void ParallelResultCalculation(double* pMatrix, double* pMatrix2,
	double* pResult, int Size) {
	int i, j, k ; 
omp_set_num_threads(4);
#pragma omp parallel for private (j)
	for (i = 0; i < Size; i++) 
    {
		for (j = 0; j < Size; j++){
            double dot = 0;
			for (k = 0; k < Size; k++) {
					dot += pMatrix[i*Size+k]*pMatrix2[k*Size+j];
				} 
				pResult[i*Size+j ] = dot;
        }
         
	}
}
void PrintResult(double* pResult, int RowCount, int ColCount) {
	int i, j; // Loop variables
	for (i = 0; i < RowCount; i++) {
		for (j = 0; j < ColCount; j++)
			printf("%7.4f ", pResult[i * RowCount + j]);
		printf("\n");
	}
}
// Function for computational process termination
void ProcessTermination(double* pMatrix, double* pMatrix2, double* pResult) {
	delete[] pMatrix;
	delete[] pMatrix2;
	delete[] pResult;
}
int main() {
	double* pMatrix; // The first argument - initial matrix
	double* pMatrix2; // The second argument - initial matrix
	double* pResult; // Result vector for matrix-matrix multiplication 
	int Size; // Sizes of initial matrix and vector
	double Start, Finish, Duration;
	printf("Parallel matrix-vector multiplication program\n");
	// Memory allocation and definition of objects' elements
	ProcessInitialization(pMatrix, pMatrix2, pResult, Size);
	// Matrix and vector output
	// printf("Initial Matrix1 \n");
	// PrintPMatrix(pMatrix, Size, Size);
    // printf("Initial Matrix2 \n");
    // PrintPMatrix2(pMatrix2, Size, Size);
	// Matrix-vector multiplication
	Start = GetTime();
	//ParallelResultCalculation(pMatrix, pMatrix2, pResult, Size);
    SerialResultCalculation( pMatrix, pMatrix2,pResult, Size);
    // printf("The result:\n");
    // PrintResult(pResult,Size,Size);
	Finish = GetTime();
	Duration = Finish - Start;
	// TestResult(pMatrix, pMatrix2, pResult, Size);
	// Printing the time spent by matrix-matrix multiplication
	printf("\n Time of execution pararrel: %f\n", Duration);

	ProcessTermination(pMatrix, pMatrix2, pResult);
}