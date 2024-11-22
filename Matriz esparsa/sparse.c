#include <stdlib.h>
#include <stdio.h>
#include <sparse.h>

//Cria a matriz esparsa
SPARSE *sparse_create(int nrow, int ncol){
    SPARSE *sparse = (SPARSE*)calloc(1, sizeof(SPARSE));
    sparse->nrow = nrow;
    sparse->ncol = ncol;
    sparse->row = (SPARSE_ELEM**)calloc(nrow,sizeof(SPARSE_ELEM*));
    sparse->col = (SPARSE_ELEM**)calloc(ncol,sizeof(SPARSE_ELEM*));

    return sparse;    
}

//Insere os dados na matriz esparsa
int sparse_put(SPARSE *sparse, int row, int col, int value){
    if(!sparse) return INVALID_SPARSE;
    if(row < 0 || col < 0 || row > sparse->nrow || col > sparse->ncol) return INVALID_POSITION;

    SPARSE_ELEM **rp = &sparse->row[row];
    SPARSE_ELEM **cp = &sparse->col[col];

    while(*rp && (*rp)->col < col) rp = &(*rp)->right;
    while(*cp && (*cp)->row < row) cp = &(*cp)->botton;

    if(*rp && *cp && (*rp)->col == col && (*cp)->row == row) (*rp)->value = value;
    else{
        SPARSE_ELEM *new = (SPARSE_ELEM*)malloc(sizeof(SPARSE_ELEM));
        new->right = *rp;
        new->botton = *cp;
        new->col = col;
        new->row = row;
        new->value = value;
       *cp = *rp = new;
    }

    return SUCCCESS;
}

//Retorna o elemento pra uma dada coluna e linha
int sparse_get(SPARSE *sparse, int row, int col){
    if(!sparse) return INVALID_SPARSE;
    if(row < 0 || col < 0 || row > sparse->nrow || col > sparse->ncol) return INVALID_POSITION;

    SPARSE_ELEM **rp = &sparse->row[row];

    while(*rp && (*rp)->col < col) rp = &(*rp)->right;

    if(*rp && (*rp)->col == col && (*rp)->row == row) return (*rp)->value;
    else return sparse->constant;
}

//Soma um valor em uma dada posicao ou cria o elemento caso o mesmo nao exista
int __sparse_add(SPARSE *sparse, int row, int col, int value){
    if(!sparse) return INVALID_SPARSE;
    if(row < 0 || col < 0 || row > sparse->nrow || col > sparse->ncol) return INVALID_POSITION;

    SPARSE_ELEM *aux;
    SPARSE_ELEM **rp = &sparse->row[row];
    SPARSE_ELEM **cp = &sparse->col[col];

    while(*rp && (*rp)->col < col) rp = &(*rp)->right;
    while(*cp && (*cp)->row < row) cp = &(*cp)->botton;

    if(*rp && *cp && (*rp)->col == col && (*cp)->row == row) (*rp)->value += value;
    else{
        SPARSE_ELEM *new = (SPARSE_ELEM*)malloc(sizeof(SPARSE_ELEM));
        new->right = *rp;
        new->botton = *cp;
        new->col = col;
        new->row = row;
        new->value = value;
       *cp = *rp = new;
    }
    if((*rp)->value == 0){
        aux = *rp;
        *rp = (*rp)->right;
        *cp = (*cp)->botton;
        free(aux);
    }

    return SUCCCESS;
}

//Soma as duas matrizes e retorna um ponteiro para a matriz resultante
SPARSE *sparse_sum(SPARSE *matrix1, SPARSE *matrix2){
    if(!matrix1 || !matrix2) return NULL;

    int i;
    SPARSE *resultant = sparse_create(matrix1->nrow,matrix1->ncol);
    SPARSE_ELEM **rp1, **rp2;

    for(i=0;i<matrix1->nrow;i++){   
        rp1 = &matrix1->row[i];
        rp2 = &matrix2->row[i];
        while(*rp1){
           __sparse_add(resultant,i,(*rp1)->col,(*rp1)->value);
           rp1 = &(*rp1)->right;
        }
        while(*rp2){
            __sparse_add(resultant,i,(*rp2)->col,(*rp2)->value);
            rp2 = &(*rp2)->right;
        }
    }

    return resultant;
}

//Multiplica duas matrizes e retorna a matriz resultante
SPARSE *sparse_mult(SPARSE *matrix1, SPARSE *matrix2){
    if(!matrix1 || !matrix2) return NULL;

    int i,j,acc;
    SPARSE *resultant = sparse_create(matrix1->nrow,matrix2->ncol);
    SPARSE_ELEM *p;

    for(i=0;i<resultant->nrow;i++){
        for(j=0;j<resultant->ncol;j++){
            p = matrix1->row[i];
            acc = 0;
            while(p){
                acc += sparse_get(matrix2,p->col,j)*(p->value);
                p = p->right;
            }
            __sparse_add(resultant,i,j,acc);
        }
    }

    return resultant;
}

//Imprime uma matriz esparsa
void sparse_print(SPARSE *resultant){
    int i;
    SPARSE_ELEM **rp1;
    printf("-1 %d %d\n", resultant->nrow, resultant->ncol);

    for(i=0;i<resultant->nrow;i++){
        rp1 = &resultant->row[i];
        while(*rp1){
            printf("%d %d %d\n",i,(*rp1)->col,(*rp1)->value);
            rp1 = &(*rp1)->right;
        }
    }
}

//Libera a memoria alocada para uma matriz esparsa
int sparse_free(SPARSE *sparse){
	if (!sparse) return INVALID_SPARSE;

	int i;
	SPARSE_ELEM *p;

	for(i=0;i<sparse->nrow;i++) {
		while ((p = sparse->row[i])) {
			sparse->row[i] = p->right;
			free(p);
		}
	}
	free(sparse->row);
	free(sparse->col);
	free(sparse);

    return 0;
}