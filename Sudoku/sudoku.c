#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sudoku.h>

//Le o stdin e armazena os valores em uma matriz
char **sudoku_get(int dim){
    
	int i,j;
	char aux;
	char **grid = (char**)malloc(dim*sizeof(char*));
	for(i=0;i<dim;i++) grid[i] = (char*)calloc(dim,sizeof(char));

	for(i=0;i<dim;i++){
		for(j=0;j<dim;j++){
			scanf(" %c",&aux);
            if(aux != '.')
            grid[i][j] = aux;
		}
	}
    return grid;
}

//Escreve no stdout uma matriz passada como argumento
void sudoku_print_grid(char **grid,int dim){

    int i,j;

    for(i=0;i<dim;i++){
	    for(j=0;j<dim;j++){
            if(grid[i][j]) printf("%c",grid[i][j]);
		}
		printf("\n");
	}
}

//Avalia para uma dada posicao quais sao os possiveis valores para inserir no campo
//O retorno eh um vetor contendo os valores 
char *_sudoku_possible_values(char **grid,int line,int col,int *size, int dim){
    char *values = NULL, aux;
    int i,j,*hash,gap_col,gap_line,block,counter = 0;
    hash = (int*)calloc(127,sizeof(int));
    block = (int)sqrt(dim);
    gap_line = (line/block)*block;
    gap_col = (col/block)*block;

    
    if(!grid[line][col]){
        for(i=0;i<dim;i++){
            aux = grid[line][i];
            hash[(int)aux]++;
        }
        for(i=0;i<dim;i++){
            aux = grid[i][col];
            hash[(int)aux]++;
        }
        for(i=gap_line;i<(gap_line+block);i++){
            for(j=gap_col;j<(gap_col+block);j++){
                aux = grid[i][j];
                hash[(int)aux]++;
            }
        }
        for(i=48;i<58;i++){
            if(!hash[i]){
                counter++;
                values = (char*)realloc(values,counter*sizeof(char));
                values[counter-1] = i;
            }
        }
        for(i=65;i<71;i++){
            if(!hash[i]){
                counter++;
                values = (char*)realloc(values,counter*sizeof(char));
                values[counter-1] = i;
            }
        }
    }

    free(hash);
    *size = counter;

    return values;
}

//Verifica se a posicao possui um unico valor possivel, realiza essa operacao
//ate o quadro do sudoku estar completo
void sudoku_solver(char **grid, int dim){
    char *values;
    int i,j,not_solved = 1,counter;
    i = j = 0;
    while(i <= dim && j <= dim && not_solved){
       not_solved = i = j = 0;
	    for(i=0;i<dim;i++){
		    for(j=0;j<dim;j++){
                if(!grid[i][j]){
                    values = _sudoku_possible_values(grid,i,j,&counter,dim);
                    if(counter == 1) grid[i][j] = values[0];
                    not_solved++;
                    free(values);
                }
            }
        }
    }
}
