/*
 *
 * Algorithms are fun
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <ctype.h>

/* type definitions ----------------------------------------------------------*/
#define NUM_LETTER 26
#define ID(x) ((isupper(x))? (x)-'A': ((x)-'a'+NUM_LETTER))
#define CHAR(x) ((char)((x)>=NUM_LETTER?((x)-NUM_LETTER+'a'):((x)+'A')))
#define MAX(x,y) ((x)>(y)?(x):(y))
#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX_VERTICES 52
#define VERTICES_NOT_EXIST -1
#define VERTICES_EXIST 0
#define OUTPUT_LIMIT_ALL 12
#define OUTPUT_LIMIT 6
/* end  definitions ------------------------------------------------------*/

/* type definitions ----------------------------------------------------------*/
typedef struct {
    int to;
    int val;
    bool *vis;
} data_t;

typedef struct node node_t;

struct node {
    data_t data;
    node_t *next;
};

typedef struct {
    node_t *head;
    node_t *foot;
} list_t;

typedef struct {
    int num_vertices;
    int num_edge;
    list_t **edges;
    int *deg;
} graph_t;

/* end type definitions ------------------------------------------------------*/

/* function prototypes -------------------------------------------------------*/

// list operations from listops.c
list_t* make_empty_list(void);
int    is_empty_list(list_t*);
void   free_list(list_t*);
list_t* insert_at_head(list_t*, data_t);
list_t* insert_at_foot(list_t *list, data_t value);
data_t get_head(list_t *list);
list_t* get_tail(list_t *list);

// graph operations
void create_graph(graph_t*);
void read_data(graph_t*, char);
void free_graph(graph_t*);
void set_edge_unvisited(graph_t*);

// functions to check vertices or edges' status
bool is_finish(graph_t*);
bool has_unvisited_edge(graph_t*,int);

// functions about circuit
list_t* expand(graph_t*, char);
int calc_val(list_t*);

// functions to print
void print_stage_1(graph_t*, char);
void print_stage_2(graph_t*, char);
void print_circuit(list_t*);

/* end function prototypes ---------------------------------------------------*/

int main(int argc, char *argv[]){
#ifdef DEBUG
    freopen("test5.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
    graph_t *G = (graph_t *) malloc(sizeof(G));
    create_graph(G);
    char start = argv[1][0];
    read_data(G, start);
    print_stage_1(G, start);
    print_stage_2(G, start);
    free_graph(G);
    return 0;
}

void create_graph(graph_t *G){
    G->num_edge = G->num_vertices = 0;
    G->deg = (int *) malloc(MAX_VERTICES * sizeof(int));
    G->edges = (list_t**) malloc(MAX_VERTICES * sizeof(list_t*));
    int i;
    for(i=0; i<MAX_VERTICES; i++){
        G->edges[i] = make_empty_list();
        G->deg[i] = VERTICES_NOT_EXIST;
    }
}

void read_data(graph_t *G, char route_start){
    char u, v;
    int val;
    int max_edge_val = INT_MIN;
    int min_edge_val = INT_MAX;
    int total_edge_val = 0;
    int odd_deg_num = 0;
    int even_deg_num = 0;
    int i;
    while(~scanf("%c %c %d\n", &u, &v, &val)){
        if(G->deg[ID(u)] == VERTICES_NOT_EXIST)
            G->deg[ID(u)] = VERTICES_EXIST;
        if(G->deg[ID(v)] == VERTICES_NOT_EXIST)
            G->deg[ID(v)] = VERTICES_EXIST;
        G->deg[ID(u)] ++;
        G->deg[ID(v)] ++;
        G->num_edge ++;
        max_edge_val = MAX(max_edge_val, val);
        min_edge_val = MIN(min_edge_val, val);
        total_edge_val += val;
        bool *edge_vis = (bool*) malloc(sizeof(bool));
        *edge_vis = false;
        insert_at_foot(G->edges[ID(u)], (data_t){ID(v), val, edge_vis});
        if(u != v)
            insert_at_foot(G->edges[ID(v)], (data_t){ID(u), val, edge_vis});
    }
    for(i=0; i<MAX_VERTICES; i++){
        if(G->deg[i] == VERTICES_NOT_EXIST)
            continue;
        G->num_vertices ++;
        if(G->deg[i] & 1)
            odd_deg_num ++;
        else
            even_deg_num ++;
    }
    puts("Stage 0 Output");
    puts("--------------");
    printf("S0: Map is composed of %d vertices ", G->num_vertices);
    printf("and %d edges\n", G->num_edge);
    printf("S0: Min. edge value: %d\n", min_edge_val);
    printf("S0: Max. edge value: %d\n", max_edge_val);
    printf("S0: Total value of edges: %d\n", total_edge_val);
    printf("S0: Route starts at \"%c\"\n", route_start);
    printf("S0: Number of vertices with odd degree: %d\n", odd_deg_num);
    printf("S0: Number of vertices with even degree: %d\n", even_deg_num);
    if(odd_deg_num == 0|| odd_deg_num == 2){
        if(even_deg_num == 2){
            printf("S0: Multigraph is traversable\n");
            exit(EXIT_FAILURE);
        }
        else
            printf("S0: Multigraph is Eulerian\n");
    } else
        exit(EXIT_FAILURE);

}

void free_graph(graph_t *G){
    int i;
    for(i=0; i < MAX_VERTICES; i++){
        if(G->deg[i] == VERTICES_NOT_EXIST)
            continue;
        node_t *p;
        for(p = G->edges[i]->head; p != NULL; p = p->next){
            if(i <= p->data.to)
                free(p->data.vis);
        }
        free_list(G->edges[i]);
    }
    free(G->deg);
    free(G);
}

void print_stage_1(graph_t *G, char start){
    puts("Stage 1 Output");
    puts("--------------");
    list_t *circuit = expand(G, start);
    insert_at_head(circuit, (data_t){ID(start), 0, NULL});
    printf("S1: ");
    print_circuit(circuit);

    while(!is_finish(G)){
        node_t *p;
        for(p = circuit->head; p != NULL; p = p->next){
            if(has_unvisited_edge(G, p->data.to)){
                list_t *new_circuit = expand(G, CHAR(p->data.to));
                new_circuit->foot->next = p->next;
                p->next = new_circuit->head;

                printf("S1: ");
                print_circuit(circuit);

                break;
            }
        }
    }
    int weight = 0, route_val = 0;
    node_t *p;
    for(p = circuit->head; p != NULL; p = p->next, weight++){
        route_val += weight * p->data.val;
    }
    printf("S1: Scenic route value is %d\n", route_val);
}

void print_stage_2(graph_t *G, char start){
    set_edge_unvisited(G);
    puts("Stage 2 Output");
    puts("--------------");
    list_t *circuit = expand(G, start);
    insert_at_head(circuit, (data_t){ID(start), 0, NULL});
    printf("S2: ");
    print_circuit(circuit);

    while(!is_finish(G)){
        node_t *target = NULL;
        int max_val = INT_MIN;
        list_t *new_circuit;
        node_t *p;
        for(p= circuit->head; p != NULL; p = p->next) {
            if (has_unvisited_edge(G, p->data.to)) {
                target = p;
                new_circuit = expand(G, CHAR(target->data.to));
                break;
            }
        }
        for(p = circuit->head; p != NULL; p = p->next) {
            if (p->data.to == target->data.to) {
                node_t *tmp = new_circuit->foot->next;
                new_circuit->foot->next = p->next;
                p->next = new_circuit->head;
                int cur_val = calc_val(circuit);
                if (cur_val > max_val) {
                    target = p;
                    max_val = cur_val;
                }
                p->next = new_circuit->foot->next;
                new_circuit->foot->next = tmp;
            }
        }

        new_circuit->foot->next = target->next;
        target->next = new_circuit->head;

        printf("S2: ");
        print_circuit(circuit);
    }

    printf("S2: Scenic route value is %d\n", calc_val(circuit));
}


void print_circuit(list_t *circuit){
    int length = 0;
    node_t *p;
    for(p = circuit->head->next; p != NULL; p = p->next)
        length++;
    printf("%c", CHAR(circuit->head->data.to));
    if(length <= OUTPUT_LIMIT_ALL){
        for(p = circuit->head->next; p != NULL; p = p->next)
            printf("-%d->%c", p->data.val, CHAR(p->data.to));
    } else {
        p = circuit->head->next;
        int i = 0;
        for(; i < OUTPUT_LIMIT; i++, p = p->next){
            printf("-%d->%c", p->data.val, CHAR(p->data.to));
        }
        printf("...");
        for(;i + OUTPUT_LIMIT + 1 < length; i++, p = p->next);
        printf("%c", CHAR(p->data.to));
        i++, p = p->next;
        for(; i < length; i++, p = p->next)
            printf("-%d->%c", p->data.val, CHAR(p->data.to));
    }
    printf("\n");
}

list_t* expand(graph_t *G, char start){
    list_t *ans = make_empty_list();
    int now_id = ID(start);
    while(true){
        data_t *target = NULL;
        node_t *p;
        for(p = G->edges[now_id]->head; p!=NULL; p = p->next){
            if(*(p->data.vis) == true)
                continue;
            if(target == NULL)
                target = &p->data;
            else if(p->data.val < target->val)
                target = &p->data;
            else if(p->data.val == target->val && p->data.to < target->to)
                target = &p->data;

        }
        if(target == NULL)
            break;
        insert_at_foot(ans, *target);
        *(target->vis) = true;
        if(target->to == ID(start))
            break;
        now_id = target->to;
    }
#ifdef DEBUG
    puts("");
    printf("[%c]",start);
    for(node_t *p = ans->head; p!=NULL; p=p->next)
        printf("-%d->[%c]", p->data.val, CHAR(p->data.to));
    puts("");
#endif
    return ans;
}


bool has_unvisited_edge(graph_t *G,int id){
    node_t *p;
    for(p = G->edges[id]->head; p != NULL; p = p->next)
        if(*(p->data.vis) == false)
            return true;
    return false;
}

bool is_finish(graph_t *G){
    int i;
    for(i = 0; i < MAX_VERTICES; i++){
        if(G->deg[i] == VERTICES_NOT_EXIST)
            continue;
        if(has_unvisited_edge(G, i))
            return false;
    }
    return true;
}

int calc_val(list_t *circuit){
    int weight = 0, route_val = 0;
    node_t *p;
    for(p = circuit->head; p != NULL; p = p->next, weight++){
        route_val += weight * p->data.val;
    }
    return route_val;
}

void set_edge_unvisited(graph_t *G){
    int i;
    for(i=0; i < MAX_VERTICES; i++){
        if(G->deg[i] == VERTICES_NOT_EXIST)
            continue;
        node_t *p;
        for(p = G->edges[i]->head; p != NULL; p = p->next)
            *(p->data.vis) = false;
    }
}

/*------------------------------------------------------------------------------
   Code that follows is written by Alistair Moffat, as an example for the book
   "Programming, Problem Solving, and Abstraction with C", Pearson
   Custom Books, Sydney, Australia, 2002; revised edition 2012,
   ISBN 9781486010974.

   See http://people.eng.unimelb.edu.au/ammoffat/ppsaa/ for further
   information.

   Prepared December 2012 for the Revised Edition.
------------------------------------------------------------------------------*/

list_t* make_empty_list(void){
    list_t *list;
    list = (list_t*)malloc(sizeof(*list));
    assert(list!=NULL);
    list->head = list->foot = NULL;
    return list;
}

int is_empty_list(list_t *list){
    assert(list!=NULL);
    return list->head==NULL;
}

void free_list(list_t *list){
    node_t *curr, *prev;
    assert(list!=NULL);
    curr = list->head;
    while (curr) {
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    free(list);
}

list_t* insert_at_head(list_t *list, data_t value){
    node_t *new;
    new = (node_t*)malloc(sizeof(*new));
    assert(list!=NULL && new!=NULL);
    new->data = value;
    new->next = list->head;
    list->head = new;
    if (list->foot==NULL) {
        /* this is the first insertion into the list */
        list->foot = new;
    }
    return list;
}

list_t* insert_at_foot(list_t *list, data_t value){
    node_t *new;
    new = (node_t*)malloc(sizeof(*new));
    assert(list!=NULL && new!=NULL);
    new->data = value;
    new->next = NULL;
    if (list->foot==NULL) {
        /* this is the first insertion into the list */
        list->head = list->foot = new;
    } else {
        list->foot->next = new;
        list->foot = new;
    }
    return list;
}

data_t get_head(list_t *list){
    assert(list!=NULL && list->head!=NULL);
    return list->head->data;
}

list_t *get_tail(list_t *list){
    node_t *oldhead;
    assert(list!=NULL && list->head!=NULL);
    oldhead = list->head;
    list->head = list->head->next;
    if (list->head==NULL) {
        /* the only list node just got deleted */
        list->foot = NULL;
    }
    free(oldhead);
    return list;
}

