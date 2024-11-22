#include <stdlib.h>
#include <stdio.h>
#include <graph.h>
#include <stack.h>

int main(int argc, char **argv){

    GRAPH_TOPOLOGY *graph = create_graph();
    RESULTS *results;

    int input_size,i,start_point;
    float x,y;

    //Le o stdin para coletar os dados de entrada
    scanf("%d", &input_size);
    for(i=1;i<=input_size;i++){
        scanf("%f %f",&x,&y);
        add_vertice(graph,i,x,y);
    }
    scanf("%d", &input_size);
    for(i=1;i<=input_size;i++){
        scanf("%f %f",&x,&y);
        set_vertice_type(graph,(int)x,(int)y);
    }
    scanf("%d", &input_size);
    for(i=1;i<=input_size;i++){
        scanf("%f %f",&x,&y);
        set_edge(graph,(int)x,(int)y);
    }
    scanf("%d", &start_point);    

    results = backtracking(graph,start_point);
    print_results(results, graph);
    free_graph(graph);

    return SUCCESS;
}