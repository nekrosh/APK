#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <locale.h>
#include <windows.h>
#include <time.h>


#define number_of_repetitions 1000000
#define colums 16
#define rows 16
using namespace std;


void output_matrix(int* matrix) {
	for (int i = 0; i < rows; i++) {
		printf("\n");
		for (int j = 0; j < colums; j++) {
			printf("%6d ", matrix[i+j]);
		}
	}

}




int main()
{
	unsigned int i, j;
	clock_t timer;
	int matrix_main[rows][colums], matrix_C[rows][colums], matrix_asm[rows][colums], matrix_asm_with_MMX[rows][colums];
	srand(time(NULL));
	for (i = 0; i < rows; i++){
		for (j = 0; j < colums; j++){
			matrix_main[i][j] =  rand();
		}
	}


	// C
	timer = clock();
	for (unsigned long int k = 0; k < number_of_repetitions; k++){
	for (i = 0; i < rows; i++){
		for (j = 0; j < colums; j++){
			matrix_C[i][j] = matrix_main[i][j];
		}
	}
	}
	timer = clock() - timer;
	printf("C time = %.30lf \n", ((double)timer) / CLOCKS_PER_SEC);


//assembly
	timer = clock();
	for (unsigned long int k = 0; k < number_of_repetitions; k++){
		i = colums * rows;
		_asm{
			pushad
				xor esi, esi
			loop1 :
				mov eax, matrix_main[esi*4]
				mov matrix_asm[esi*4], eax
				inc esi
				dec i
				jnz loop1
				popad		
		}
	}
	timer = clock() - timer;
	printf("Assembly time = %.30lf \n", ((double)timer) / CLOCKS_PER_SEC);

// assembly with MMX
	timer = clock();
	for (unsigned long int k = 0; k < number_of_repetitions; k++) {
		i = colums * rows/2;
		_asm {
			pushad
				xor esi, esi
				pxor MM0,MM0
			loop2:
				movq MM0, matrix_main[esi*8]
				movq matrix_asm_with_MMX[esi * 8],MM0 
				inc esi
				dec i
				jnz loop2
				emms
				popad
		}
	}
	timer = clock() - timer;
	printf("Assembly with MMX time = %.30lf \n", ((double)timer) / CLOCKS_PER_SEC);
	
	
	
	/*output_matrix(*matrix_C);
	printf("\n"); 
	printf("\n");
	printf("\n");
	output_matrix(*matrix_asm);
	printf("\n");
	printf("\n");
	printf("\n");
	output_matrix(*matrix_asm_with_MMX);*/

	getchar();


	
	return 0;
}