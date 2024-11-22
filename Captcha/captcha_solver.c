#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <captcha_solver.h>


/*Funcao responsavel pela leitura da imagem passada pelo seu respectivo nome no argumento da funcao.
Retorna um ponteiro para o tipo IMAGE alocado dinamicamente no corpo da funcao*/
IMAGE *captcha_load_image(char *FileName){

    unsigned int i,j;
    char aux;
    FILE *pgm_file;
    IMAGE *pgm_image;

    if(!(pgm_file = fopen(FileName, "r"))) printf("File could not be opened");

    pgm_image = (IMAGE*)calloc(1,sizeof(IMAGE));
    
    i=0;
    while(fscanf(pgm_file,"%c",&aux) & (aux != '\n')){
        pgm_image->ASCII_type = (char*)realloc(pgm_image->ASCII_type,sizeof(char)*(i+1));
        pgm_image->ASCII_type[i] = aux;
        i++;
    }
    pgm_image->ASCII_type = (char*)realloc(pgm_image->ASCII_type,sizeof(char)*(i));
    pgm_image->ASCII_type[i+1] = '\0';

    fscanf(pgm_file,"%d %d",&pgm_image->columns, &pgm_image->lines);
    fscanf(pgm_file,"%d",&pgm_image->scale);

    pgm_image->matrix = (unsigned char**)calloc(pgm_image->lines, sizeof(unsigned char*));

    for(i=0;i<pgm_image->lines;i++){
        pgm_image->matrix[i] = (unsigned char*)calloc(pgm_image->columns, sizeof(unsigned char));
        for(j=0;j<pgm_image->columns;j++){
            fscanf(pgm_file,"%hhd",&pgm_image->matrix[i][j]);
        }
    }
    fclose(pgm_file);

    return pgm_image;
}

//Funcao de ordenacao para operacionalizar o algoritimo median filter
int bubble_sort(unsigned char *vector, unsigned char size){
    unsigned int i, j;
    unsigned char aux;

    if(!vector) return -1;

    for(i=0;i<size;i++){
        aux = 0;
        for(j=0;j<size-1-i;j++){
            if(vector[j] > vector[j+1]){
                aux = vector[j];
                vector[j] = vector[j+1];
                vector[j+1] = aux;
            }
        }
    }
    return 0;
}

/*Alogoritimo median filter para remocao do ruido na imagem.
Definido para uma matriz de convolucao 3X3*/
IMAGE *median_filter(IMAGE *pgm_image){

    IMAGE *filtered_image;
    unsigned char *AUXvector;
    unsigned int i,j,k,l,p;

    filtered_image = (IMAGE*)calloc(1,sizeof(IMAGE));
    filtered_image->ASCII_type = (char*)malloc(strlen(pgm_image->ASCII_type)*sizeof(char*));
    strcpy(filtered_image->ASCII_type,pgm_image->ASCII_type);
    filtered_image->scale = pgm_image->scale;
    filtered_image->lines = pgm_image->lines;
    filtered_image->columns = pgm_image->columns;

    filtered_image->matrix = (unsigned char**)calloc(filtered_image->lines, sizeof(unsigned char*));
    for(i=0;i<filtered_image->lines;i++){
        filtered_image->matrix[i] = (unsigned char*)calloc(filtered_image->columns, sizeof(unsigned char));
    }

    AUXvector = (unsigned char*)malloc(9*sizeof(unsigned char));
    for(i=0;i<filtered_image->lines-2;i++){
        for(j=0;j<filtered_image->columns-2;j++){
            p = 0;
            for(k=0;k<3;k++){
                for(l=0;l<3;l++){
                    AUXvector[p] = pgm_image->matrix[i+k][j+l];
                    p++;
                }
            }
            bubble_sort(AUXvector,9);
            filtered_image->matrix[i+1][j+1] = AUXvector[4];
        }
    }

    free(AUXvector);

    return filtered_image;
}

/*Escreve a imagem em um arquivo.Seu nome e passado como argumento na funcao.
Implementado para fins didaticos*/
void write_image(IMAGE *pgm_image, char *output_name){

    FILE *output_file;
    unsigned int i,j;

    if(!(output_file = fopen(output_name, "w"))) printf("File could not be opened");

    fprintf(output_file, "%s\n", pgm_image->ASCII_type);
    fprintf(output_file, "%d %d\n", pgm_image->columns, pgm_image->lines);
    fprintf(output_file, "%d\n", pgm_image->scale);

    for(i=0;i<pgm_image->lines;i++){
        for(j=0;j<pgm_image->columns;j++){
            fprintf(output_file,"%d ", pgm_image->matrix[i][j]);
        }
        fprintf(output_file,"\n");
    }

    fclose(output_file);
}

//Cria uma lista para as imagens segmentadas
SEGMENT_LIST *create_list(unsigned int size){

    SEGMENT_LIST *list = NULL;
    list = (SEGMENT_LIST*)calloc(1,sizeof(SEGMENT_LIST));
    
    list->elem = (IMAGE*)calloc(size,sizeof(IMAGE));
    list->size = size;

    return list;
}

//Libera a memoria heap alocada para a imagem
void free_image(IMAGE *pgm_image){
    unsigned int i;
    free(pgm_image->ASCII_type);
    for(i=0;i<pgm_image->lines;i++){
        free(pgm_image->matrix[i]);
    }
    free(pgm_image->matrix);
}

//Libera a memoria heap alocada para a lista e todos os seus elementos
void free_list(SEGMENT_LIST *list){
    unsigned int i;
    for(i=0;i<list->size;i++){
        free_image(&list->elem[i]);
    }
    free(list->elem);
    free(list);
}

//Segunda segmentacao para que a imagem fique do tamanho da mascara.Tambem insere o elemento na lista
void append_value(SEGMENT_LIST *list,unsigned char **temp_img,int position){
    unsigned int i,j,k,l,m, column, line, gap,sum = 0;
    int diference;

    //Um ruido elevado no comeco da imagem pode prejudicar a segmentacao
    i = -1;
    diference = 0;
    while(diference < 9){
        i += 1;
        j = 0;
        k = MAX_COLUMNS-1;
        while((j<MAX_COLUMNS/2) & (temp_img[i][j] != 1)) j++;
        while((k>MAX_COLUMNS/2) & (temp_img[i][k] != 1)) k--;
        //Desconsidera ruidos remanescentes na imagem
        for(m=0;m<MAX_COLUMNS;m++) sum += temp_img[i][m];
        if(sum > 4) diference = k-j;
    }

    //Adiciona as colunas restantes para que a imagem possua o mesmo tamanho da mascara
    gap = (DEFAULT_MASK_COLUMNS-1)-diference;
    if(gap%2 == 1){
        j -= gap/2;
        k += (gap/2)+1;
    }
    else{
        j -= gap/2;
        k += gap/2;
    }
    column = j;
    line = i;
    l = 0;
    while(i<line+DEFAULT_MASK_LINES){
        k = 0;
        j = column;
        while(j<column+DEFAULT_MASK_COLUMNS){
            list->elem[position].matrix[l][k] = temp_img[i][j];
            j++;
            k++;
        }
        i++;
        l++;
    }
}

/*Retorna uma lista com os elementos da imagem original segmentados de acordo com o tamanho da mascara
 e sem ruido*/
SEGMENT_LIST *segment_image(IMAGE *pgm_image){
    
    SEGMENT_LIST *list = NULL;
    unsigned int i,j,k,l,nElem;
    unsigned char **temp_img;

    nElem = pgm_image->columns/MAX_COLUMNS;
    list = create_list(nElem);

    for(i=0;i<nElem;i++){
        list->elem[i].matrix = (unsigned char**)malloc(DEFAULT_MASK_LINES*sizeof(unsigned char*));
        for(j=0;j<DEFAULT_MASK_LINES;j++){
            list->elem[i].matrix[j] = (unsigned char*)malloc(DEFAULT_MASK_COLUMNS*sizeof(unsigned char));
        }
    }

    temp_img = (unsigned char**)malloc(pgm_image->lines*sizeof(unsigned char*));
    for(j=0;j<pgm_image->lines;j++){
        temp_img[j] = (unsigned char*)malloc(MAX_COLUMNS*sizeof(unsigned char));
    }

    //PARTE CRÍTICA - DUPLA SEGMENTAÇÂO PARA INSERIR NA LISTA
    for(j=0;j<nElem;j++){
        for(i=0;i<pgm_image->lines;i++){
            for(k=j*MAX_COLUMNS,l=0;k<(j*MAX_COLUMNS)+MAX_COLUMNS;k++,l++){
                temp_img[i][l] = pgm_image->matrix[i][k];
            }
        }
        append_value(list,temp_img,j);
    }


free(temp_img);
return list;
}

//Retorna uma lista com todas as mascaras. As imagens devem estar no diretório do executavel e no formato *.pgm
SEGMENT_LIST *load_masks(){
    
    unsigned char i,j,k, char_size;
    char *mask_file_name, aux;
    SEGMENT_LIST *mask_list = NULL;
    FILE *mask_file_pointer;

    mask_list = create_list(10);
    mask_file_name = (char*)malloc(6*sizeof(char));
    strcpy(mask_file_name, "*.pgm");
    
    for(i=0;i<mask_list->size;i++){
        mask_file_name[0] = i+48;
        mask_file_pointer = fopen(mask_file_name,"r");
        if(!mask_file_pointer) printf("Mask file could not be opened");

        char_size = 0;
        while(fscanf(mask_file_pointer,"%c",&aux) & (aux != '\n')){
            mask_list->elem[i].ASCII_type = (char*)realloc(mask_list->elem[i].ASCII_type,sizeof(char)*(char_size+1));
            mask_list->elem[i].ASCII_type[char_size] = aux;
            char_size++;
        }
        mask_list->elem[i].ASCII_type = (char*)realloc(mask_list->elem[i].ASCII_type,sizeof(char)*(char_size));
        mask_list->elem[i+1].ASCII_type = '\0';

        fscanf(mask_file_pointer,"%d %d",&mask_list->elem[i].columns, &mask_list->elem[i].lines);
        fscanf(mask_file_pointer,"%d",&mask_list->elem[i].scale);
        mask_list->elem[i].matrix = (unsigned char**)malloc(mask_list->elem[i].lines*sizeof(unsigned char*));
        for(j=0;j<mask_list->elem[i].lines;j++){
            mask_list->elem[i].matrix[j] = (unsigned char*)malloc(mask_list->elem[i].columns*sizeof(unsigned char));
            for(k=0;k<mask_list->elem[i].columns;k++){
                fscanf(mask_file_pointer,"%hhd ",&mask_list->elem[i].matrix[j][k]);
            }
        }
        fclose(mask_file_pointer);
    }
    free(mask_file_name);

    return mask_list;
}
//Implementacao do algoritimo pra calcular o erro quadratico medio entre as mascaras e as imagens segmentadas
unsigned char *solve_captcha(IMAGE *pgm_image, unsigned char *size){
    SEGMENT_LIST *images_list, *mask_list;
    unsigned char *result;
    unsigned int i,j,k,l;
    float MSE, Min_MSE;

    images_list = segment_image(pgm_image);
    mask_list = load_masks();
    result = (unsigned char *)malloc(images_list->size*sizeof(unsigned char));
    *size =  images_list->size;

    for(i=0;i<images_list->size;i++){
        Min_MSE = 10000;
        for(j=0;j<mask_list->size;j++){
            MSE = 0;
            for(k=0;k<mask_list->elem[j].lines;k++){
                for(l=0;l<mask_list->elem[j].columns;l++){
                    MSE += pow((mask_list->elem[j].matrix[k][l]-images_list->elem[i].matrix[k][l]),2);
                }
            }
            MSE = MSE/(float)mask_list->elem[j].lines;
            if(MSE < Min_MSE){
                Min_MSE = MSE;
                result[i] = j;
            }
        }
    }

    free_list(images_list);
    free_list(mask_list);

    return result;
}