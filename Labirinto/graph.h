#ifndef __GRAPH__
#define __GRAPH__

#define SUCCESS 0
#define INVALID_GRAPH -1
#define INVALID_NODE -2
#define EDGE_ALREADY_EXIST -3

//Adjacencias(arestas) do No
struct adjacency{
    int id;
    unsigned char flag;
    struct adjacency *next;
};

//define a estrutura dos vertices
struct vertices{
    int id;
    int nAdjacency;
    int node_type;
    float x,y;
    struct adjacency *head;
};

//define a estrutura que contera os vertices
struct graph_topology{
    int nVertices;
    int nEdges;
    struct vertices **vertices;
};

//Estrutura para acomodar uma possivel saida
struct route{
    int size;
    int *array;
};

//Estrutura para administrar todos os resultados
struct results{
    int size;
    struct route **routes;
};

typedef struct vertices VERTICES;
typedef struct graph_topology GRAPH_TOPOLOGY;
typedef struct adjacency ADJACENCY;
typedef struct results RESULTS;
typedef struct route ROUTE;


GRAPH_TOPOLOGY *create_graph();
int add_vertice(GRAPH_TOPOLOGY *, int, float, float);
int set_vertice_type(GRAPH_TOPOLOGY *, int, int);
int set_edge(GRAPH_TOPOLOGY *, int, int);
RESULTS *backtracking(GRAPH_TOPOLOGY*,int);
void print_results(RESULTS*,GRAPH_TOPOLOGY *);
void free_graph(GRAPH_TOPOLOGY*);

#endif