#include <stdlib.h>
#include <stdio.h>
#include <sudoku.h>

#define DIMENSION 16

int main(int argc,char **argv){

	//Coleta o quadro do sudoku
	char **grid = sudoku_get(DIMENSION);
	//Resolve o sudoku
	sudoku_solver(grid,DIMENSION);
	//Retorna o jogo resolvido no stdout
	sudoku_print_grid(grid, DIMENSION);

	return 0;
}