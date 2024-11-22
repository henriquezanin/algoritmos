#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <run_lenght.h>

//Le o stdin e armazena a imagem em uma struct
IMAGE *read_image(){
    int i,size;
    char aux;
    IMAGE *output_image;
    output_image = (IMAGE*)malloc(sizeof(IMAGE));
    output_image->ASCII_type = (char*)malloc(3);

    fscanf(stdin,"%s",output_image->ASCII_type);
    fscanf(stdin,"%d %d",&output_image->columns, &output_image->lines);
    fscanf(stdin,"%d ",&output_image->scale);

    output_image->matrix = (char**)calloc(output_image->lines,sizeof(char*));

    for(i=0;i<output_image->lines;i++){
        fscanf(stdin, "%c", &aux);
        size = 0;
        while(aux != '\n'){
            if(aux != '\r'){
                size++;
                output_image->matrix[i] = (char*)realloc(output_image->matrix[i], sizeof(char)*size);
                output_image->matrix[i][size-1] = aux;
            }
            fscanf(stdin, "%c", &aux);
        }
        output_image->matrix[i] = (char*)realloc(output_image->matrix[i], sizeof(char)*(size+2));
        output_image->matrix[i][size] = ' ';
        output_image->matrix[i][size+1] = '\0';
    }

    return output_image;
}

//Comprime a imagem para o formato P8 usando expressao regular.
//O inteiro e a quantidade de caracteres repetidos que deve ser comprimida
IMAGE *encode_image(IMAGE *input_image, int compression){

    int errorCode, i,size,SubStringBytes;
    int MaxGroups = 2;
    char *expression = "([0-9]+\\s*)\\1*";
    char *pString,*NumAsString;
    IMAGE *output_image;
    
    NumAsString = (char*)malloc(8);
    output_image =  (IMAGE*)malloc(sizeof(IMAGE));
    output_image->matrix = (char**)calloc(input_image->lines,sizeof(char*));
    output_image->ASCII_type = (char*)malloc(3);
    strcpy(output_image->ASCII_type, "P8");
    output_image->scale = input_image->scale;
    output_image->lines = input_image->lines;
    output_image->columns = input_image->columns;

    regex_t regexCompiled;
    regmatch_t *groupArray =(regmatch_t*)malloc(MaxGroups*sizeof(regmatch_t));

    if ((errorCode = regcomp(&regexCompiled, expression, REG_EXTENDED))){
            printf("Error in regcomp: %d\n", errorCode);
            return NULL;
    }

    //Para cada linha da imagem o regexec e aplicado
    for(i=0;i<input_image->lines;i++){
        size = 1;
        pString = input_image->matrix[i];
        output_image->matrix[i] = (char*)realloc(output_image->matrix[i], size*sizeof(char));
        memset(output_image->matrix[i],'\0',1);
        do{
            regexec(&regexCompiled,pString,MaxGroups,groupArray,0);
            SubStringBytes = groupArray[0].rm_eo;

            if((groupArray[0].rm_eo)/(groupArray[1].rm_eo) > compression){
                sprintf(NumAsString, "%d", (groupArray[0].rm_eo/groupArray[1].rm_eo));
                size += 3+strlen(NumAsString)+groupArray[1].rm_eo;
                output_image->matrix[i] = (char*)realloc(output_image->matrix[i], size*sizeof(char));
                strcat(output_image->matrix[i],"@ ");
                strncat(output_image->matrix[i],pString,groupArray[1].rm_eo);
                strcat(output_image->matrix[i],NumAsString);
                strcat(output_image->matrix[i]," ");
            }
            else{
                size += 1+groupArray[0].rm_eo;
                output_image->matrix[i] = (char*)realloc(output_image->matrix[i], size*sizeof(char));
                strncat(output_image->matrix[i],pString,groupArray[0].rm_eo);
            }

            pString += SubStringBytes;

        }while(strlen(pString));
        //Remove o espaco do ultimo caractere
        output_image->matrix[i][size-1] = '\0'; 
    }
    free(NumAsString);
    free(groupArray);

    return output_image;
}

//Decodifica a imagem P8 para P2 usando expressao regular
IMAGE *decode_image(IMAGE *input_image){
    
    int errorCode,i,j,k,size;
    int MaxGroups = 3;
    char *expression = "@(\\s[0-9]+)(\\s[0-9]+\\s*)";
    char *pString,*NumAsStringCounter = NULL,*Group1 = NULL;
    IMAGE *output_image;
    
    output_image =  (IMAGE*)malloc(sizeof(IMAGE));
    output_image->matrix = (char**)calloc(input_image->lines,sizeof(char*));
    output_image->ASCII_type = (char*)malloc(3);
    strcpy(output_image->ASCII_type, "P2");
    output_image->scale = input_image->scale;
    output_image->lines = input_image->lines;
    output_image->columns = input_image->columns;

    regex_t regexCompiled;
    regmatch_t *groupArray =(regmatch_t*)malloc(MaxGroups*sizeof(regmatch_t));

    if ((errorCode = regcomp(&regexCompiled, expression, REG_EXTENDED))){
            printf("Error in regcomp: %d\n", errorCode);
            return NULL;
    }

    //Para cada linha da imagem o regexec e aplicado
    for(i=0;i<input_image->lines;i++){
        size = 1;
        pString = input_image->matrix[i];
        output_image->matrix[i] = (char*)realloc(output_image->matrix[i], size*sizeof(char));
        memset(output_image->matrix[i],'\0',1);

        while(!(regexec(&regexCompiled,pString,MaxGroups,groupArray,0))){
            if(groupArray[0].rm_so > 0){
                size += groupArray[0].rm_so;
                output_image->matrix[i] = (char*)realloc(output_image->matrix[i], size*sizeof(char));
                strncat(output_image->matrix[i],pString, groupArray[0].rm_so);
            }

            NumAsStringCounter = (char*)realloc(NumAsStringCounter,(groupArray[2].rm_eo - groupArray[2].rm_so)+1);
            for(j=groupArray[2].rm_so,k=0;j<=groupArray[2].rm_eo;j++,k++) NumAsStringCounter[k] = pString[j];
            NumAsStringCounter[k] = '\0';

            Group1 = (char*)realloc(Group1,(groupArray[1].rm_eo - groupArray[1].rm_so)+1);          
            for(j=groupArray[1].rm_so+1,k=0;j<groupArray[1].rm_eo;j++,k++) Group1[k] = pString[j];
            Group1[k] = ' ';
            Group1[k+1] = '\0';

            size += atoi(NumAsStringCounter)*((groupArray[1].rm_eo - groupArray[1].rm_so));
            output_image->matrix[i] = (char*)realloc(output_image->matrix[i], size*sizeof(char));
            for(j=0;j<atoi(NumAsStringCounter);j++) strcat(output_image->matrix[i], Group1);

            pString += groupArray[0].rm_eo;
            }
            if(strlen(pString) > 0){
                size += strlen(pString);
                output_image->matrix[i] = (char*)realloc(output_image->matrix[i], size*sizeof(char));
                strcat(output_image->matrix[i],pString);
        }
        output_image->matrix[i][size-2] = '\0';
    }

    free(NumAsStringCounter);
    free(Group1);
    free(groupArray);
    return output_image;
}
//Imprime a imagem no stdout
void print_image(IMAGE *input_image){
    int i;

    printf("%s\n",input_image->ASCII_type);
    printf("%d %d\n",input_image->columns, input_image->lines);
    printf("%d\n",input_image->scale);
    
    for(i=0;i<input_image->lines;i++){
        printf("%s\n",input_image->matrix[i]);
    }
}

//Libera a memoria alocada dinamicamente
int free_image(IMAGE *input_image){
    int i;

    free(input_image->ASCII_type);
    for(i = 0;i<input_image->lines;i++){
        free(input_image->matrix[i]);
    }
    free(input_image->matrix);
    free(input_image);

    return 0;
}