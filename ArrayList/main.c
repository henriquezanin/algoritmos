#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arraylist.h>

int main(int argc, char **argv){

    int param1, param2,param3,aux;
    char *command;
    ARRAY_LIST *list, *new_list;
    ITEM *elem;
    list = ArrayList();

    command = (char*)malloc(10);
    //Le e executa as operacoes. Apos o comando sair toda a lista e os elementos sao liberados
    scanf("%s", command);
    while(strcmp(command,"sair")){
        if(!(strcmp(command,"tamanho"))) printf("%d\n", ArrayList_size(list));
        if(!(strcmp(command,"vazia"))) printf("%d\n", ArrayList_isEmpty(list));
        if(!(strcmp(command,"print"))) ArrayList_print(list);
        if(!(strcmp(command,"add"))){
            scanf("%d %d", &param1, &param2);
            ArrayList_add(list, param1, param2);
        }
        if(!(strcmp(command,"contem"))){
            scanf("%d", &param1);
            printf("%d\n",ArrayList_contain(list, param1));
        }
        if(!(strcmp(command,"indice"))){
            scanf("%d", &param1);
            aux = ArrayList_index(list, param1);
            if(aux > -1) printf("%d\n",aux);
        }
        if(!(strcmp(command,"remover"))){
            scanf("%d", &param1);
           elem = ArrayList_remove(list, param1);
           free(elem);
        }
        if(!(strcmp(command,"get"))){
            scanf("%d", &param1);
            elem = ArrayList_get(list, param1);
            if(elem) printf("%d/%d\n", elem->key, elem->value);
        }
        if(!(strcmp(command,"set"))){
            scanf("%d %d %d", &param1, &param2, &param3);
            ArrayList_set(list, param1, param2, param3);
        }
        if(!(strcmp(command,"sub"))){
            scanf("%d %d", &param1, &param2);
            new_list = ArrayList_sub(list, param1, param2);
            ArrayList_print(new_list);
            ArrayList_OnlyListFree(&new_list);
        }

        scanf("%s",command);
    }
    free(command);
    ArrayList_free(&list);

    return 0;
}