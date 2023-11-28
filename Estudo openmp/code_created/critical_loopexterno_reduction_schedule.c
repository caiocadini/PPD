/*
** PPD / DC/UFSCar - Helio
** Programa : multiplicacao de matrizes
** Objetivo: paralelizacao om OpenMP
*/

#include <math.h>
#include <stdlib.h> 
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>

float *A, *B, *C;

int
main(int argc, char *argv[])
{
	int lin_a,col_a,lin_b,col_b,lin_c,col_c;
	int i,j,k;



	printf("Linhas A: ");   scanf("%d",&lin_a);
	printf("Colunas A / Linhas B: "); scanf("%d",&col_a);
	lin_b = col_a;
	printf("Colunas B: ");  scanf("%d",&col_b);
	printf("\n");
	lin_c = lin_a;
	col_c = col_b;

	// Alocacao dinâmica das matrizes, com linhas em sequência 
	A = (float *) malloc ( lin_a * col_a * sizeof(float) );
	B = (float *) malloc ( lin_b * col_b * sizeof(float) );
	C = (float *) malloc ( lin_c * col_c * sizeof(float) );

 	// Atribucao de valores iniciais às matrizes 
 	srand(time(NULL));

	for(i=0; i < lin_a * col_a; i++) 
		A[i] = (float)rand() / (float)RAND_MAX;

	for(i=0; i < lin_b * col_b; i++) 
		B[i] = (float)rand() / (float)RAND_MAX;
	

	// calculo da multiplicacao
 	//Aplicação da otimização do código
	#pragma omp parallel for private(j, k) collapse (2)
	for(i=0; i < lin_c; i++) 
		
		for(j=0; j < col_c; j++) {
			float aux = 0;
			#pragma omp parallel for reduction (+: aux) schedule(static, 8)
			for(k=0; k < col_a; k++){
				
				aux += A[i*col_a+k] * B[k*col_b+j];
			}
			#pragma omp critical
			C[i*col_c+j] = aux;

		}

	return(0);
}
