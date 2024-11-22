#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <graph.h>
#include <stack.h>

//Incinializa a lista de adjacencias
GRAPH_TOPOLOGY *create_graph(){
    return (GRAPH_TOPOLOGY*)calloc(1,sizeof(GRAPH_TOPOLOGY));
}

//Adiciona o vertice a lista de adjacencias
int add_vertice(GRAPH_TOPOLOGY *graph, int id, float x, float y){
    if(!graph) return INVALID_GRAPH;

    graph->vertices = (VERTICES**)realloc(graph->vertices, (graph->nVertices+1)*sizeof(VERTICES*));
    graph->vertices[graph->nVertices] = (VERTICES*)calloc(1,sizeof(VERTICES));
    graph->vertices[graph->nVertices]->id = id;
    graph->vertices[graph->nVertices]->x = x;
    graph->vertices[graph->nVertices]->y = y;
    graph->nVertices++;

    return SUCCESS;
}

//Define o tipo do vertice, se e uma saida 1, se nao 0
int set_vertice_type(GRAPH_TOPOLOGY *graph, int id, int type){
    if(!graph) return INVALID_GRAPH;
    graph->vertices[id-1]->node_type = type;
    return SUCCESS;
}

//Cria as arestas do grafo em cada vertice
int set_edge(GRAPH_TOPOLOGY *graph, int node1, int node2){
    if(!graph) return INVALID_GRAPH;
    if(node1 < 0 || node2 < 0) return INVALID_NODE;

    graph->vertices[node1-1]->nAdjacency++;
    graph->vertices[node2-1]->nAdjacency++;

    ADJACENCY **pNode1 = &(graph->vertices[node1-1]->head);
    ADJACENCY **pNode2 = &(graph->vertices[node2-1]->head);

    while(*pNode1 && (*pNode1)->id != node2 &&(pNode1 = &(*pNode1)->next));
    while(*pNode2 && (*pNode2)->id != node1 &&(pNode2 = &(*pNode2)->next));

    if(*pNode1 && *pNode2 && (*pNode1)->id == node2 && (*pNode2)->id == node1)
        return EDGE_ALREADY_EXIST;

    *pNode1 = (ADJACENCY*)calloc(1,sizeof(ADJACENCY));
    *pNode2 = (ADJACENCY*)calloc(1,sizeof(ADJACENCY));

    (*pNode1)->id = node2;
    (*pNode2)->id = node1;

    graph->nEdges++;

    return SUCCESS;
}

//insere toda a pilha em um vetor de inteiros
void stack_to_result(STACK *stack, RESULTS *results){

    int i;
    results->routes = (ROUTE**)realloc(results->routes,(results->size+1)*sizeof(ROUTE));
    results->routes[results->size] = (ROUTE*)malloc(sizeof(ROUTE));
    results->routes[results->size]->size = stack->size;
    int *array = (int*)malloc(stack->size*sizeof(int));

    STACK_ELEM *elem = stack->top;

    for(i=stack->size-1;i>=0;i--){
        array[i] = elem->value;
        elem = elem->next;
    }

    results->routes[results->size]->array = array;
    results->size++;
}

//Ordena os resultados para a saida ser compativel com a do run.codes
void order_results(RESULTS *results){
    int i,j,k;
    ROUTE *aux;

    for(i=results->size;i>=0;i--){
        for(j=0;j<i-1;j++){
            if(results->routes[j]->array[results->routes[j]->size] > results->routes[j+1]->array[results->routes[j+1]->size]){
                aux = results->routes[j];
                results->routes[j] = results->routes[j+1];
                results->routes[j+1] = aux;
            }
        }
    }

    for(i=results->size-1;i>=0;i--){
        for(j=0;j<i;j++){
            if(results->routes[j]->array[results->routes[j]->size] == results->routes[j+1]->array[results->routes[j+1]->size]){
                if(results->routes[j]->size > results->routes[j+1]->size){
                    aux = results->routes[j];
                    results->routes[j] = results->routes[j+1];
                    results->routes[j+1] = aux;
                }
            }
        }
    }

    for(i=results->size;i>=0;i--){
        for(j=0;j<i-1;j++){
            k = 0;
            if(results->routes[j]->array[results->routes[j]->size] == results->routes[j+1]->array[results->routes[j+1]->size]){
                if(results->routes[j]->size == results->routes[j+1]->size){
                    while(k < results->routes[j]->size && results->routes[j]->array[k] == results->routes[j+1]->array[k]) k++;
                    if(results->routes[j]->array[k] > results->routes[j+1]->array[k]){
                        aux = results->routes[j];
                        results->routes[j] = results->routes[j+1];
                        results->routes[j+1] = aux;
                    }
                }
            }
        }
    }
}

//Escreve os resultados no stdout
void print_results(RESULTS *results,GRAPH_TOPOLOGY *graph){

    int i,j,last,first;
    float Ax,Ay,Bx,By,distance = 0;

    for(i=0;i<results->size;i++){
        distance = 0;
        for(j=0;j<results->routes[i]->size-1;j++){
            first = results->routes[i]->array[j];
            last = results->routes[i]->array[j+1];
            Ax = graph->vertices[last-1]->x;
            Ay = graph->vertices[last-1]->y;
            Bx = graph->vertices[first-1]->x;
            By = graph->vertices[first-1]->y;
            distance += sqrt(pow(Ax-Bx,2)+pow(Ay-By,2));
        }
        results->routes[i]->array = (int*)realloc(results->routes[i]->array,(results->routes[i]->size+1)*sizeof(int));
        results->routes[i]->array[results->routes[i]->size] = distance;
    }

    order_results(results);

    for(i=0;i<results->size;i++){
        printf("%d ",results->routes[i]->size);
        for(j=0;j<results->routes[i]->size+1;j++){
            printf("%d ",results->routes[i]->array[j]);
        }
        printf("\n");
    }

}

//Realiza o percurso no grafo, caso o vertice seja uma saida a pilha inteira e
//convertida para um vetor de inteiros
RESULTS *backtracking(GRAPH_TOPOLOGY *graph, int start_point){

    int id;
    ADJACENCY *p;
    STACK *stack = create_stack();
    RESULTS *results = (RESULTS*)calloc(1,sizeof(RESULTS));
    unsigned char *visited = (unsigned char*)calloc(graph->nVertices,sizeof(unsigned char));

    stack_push(stack,start_point);
    if(graph->vertices[stack_top(stack)-1]->node_type) stack_to_result(stack,results);
    

    while(stack->size){
        id = stack_top(stack);
        p = graph->vertices[id-1]->head;
        visited[id-1] = 1;
        while(p && (p->flag || visited[p->id-1])) p = p->next;
        if(!p){
            visited[id-1] = 0;
            p = graph->vertices[id-1]->head;
            while(p){
                p->flag = 0;
                p = p->next;
            }
            stack_pop(stack);
        }
        else{
            p->flag = 1;
            visited[id-1] = 1;
            stack_push(stack,p->id);
            if(graph->vertices[stack_top(stack)-1]->node_type) stack_to_result(stack,results);
        }

    }

    stack_free(&stack);

    return results;
}

//Libera toda a memoria alocada para o grafo
void free_graph(GRAPH_TOPOLOGY *graph){
    int i;
    ADJACENCY *p,*aux;

    for(i=0;i<graph->nVertices;i++){
        p = graph->vertices[i]->head;
        while(p){
            aux = p;
            p = p->next;
            free(aux);
        }
        free(graph->vertices[i]);
    }
    free(graph);
}