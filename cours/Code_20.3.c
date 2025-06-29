typedef struct graph {
  int nb_vertices;    
  int nb_edges;       
  bool directed;     
  double **adj_matrix; 
  double **dist;      
  int **parent;        
} graph_s;
