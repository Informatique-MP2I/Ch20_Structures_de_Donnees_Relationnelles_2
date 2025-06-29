typedef struct {
  int ind;       
  double weight; 
  int prev;     
} vertex_s;
typedef struct adj_list {
  vertex_s vertex;       
  struct adj_list *next; 
} adj_list_s;
typedef struct {
  int nb_vertices;     
  int nb_edges;         
  bool directed;         
  adj_list_s **adj_lists;
} graph_s;
