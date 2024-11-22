#include <stdlib.h>
#include <stdio.h>
#include <captcha_solver.h>
/*
#####################################################################################################################
# O programa pode ser dividido em tres etapas pricipais: Eliminacao de ruido; segmentacao da imagem e identificacao #
# dos caracteres. A eliminacao de ruido utiliza a matriz de convolucao median filter e para a identificacao dos     #
# caracteres aplicou-se o erro quadratico medio entre os vetores das mascaras e os vetores da imagem segmentada.    #
# As funcoes disponiveis para os usuarios estao definidas em captcha_solver.h. Apenas a funcao write_image nao esta #
# em utilizacao, foi implementada para fins didaticos. Caso nenhum argumento seja passado na execucao do programa   #
# o mesmo aguarda o usuario digitar o nome da imagem a ser transcrita.                                              #
#####################################################################################################################*/

int main(int argc, char **argv){

    char *file_name = NULL;
    unsigned char *result,size,i;
    IMAGE *original_image,*mod_image;

    if(argc > 1){
         original_image = captcha_load_image(argv[1]);
    }
    else{
        file_name = (char*)malloc(18*sizeof(char));
        fscanf(stdin,"%s",file_name);
        original_image = captcha_load_image(file_name);
    }

    mod_image = median_filter(original_image);

    result = solve_captcha(mod_image,&size);

    for(i=0;i<size;i++){
        printf("%d", result[i]);
    }
    printf("\n");

    //Exporta a imagem filtrada
    //write_image(original_image, argv[2]);

    free(file_name);
    free(result);
    free_image(mod_image);
    free_image(original_image);

    return 0;
}