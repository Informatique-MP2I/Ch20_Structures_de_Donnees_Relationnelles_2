void floyd_warshall(graph_s *g) {
  assert(g && g->adj_matrix && g->dist && g->parent); 
  // Data recovery
  int nb_vertices = g->nb_vertices;
  double **adj_matrix = g->adj_matrix;
  double **dist = g->dist;
  int **parent = g->parent;
  // Initialisation of the matrices : dist and parent
  for (int v = 0; v < nb_vertices; v++) {
    for (int w = 0; w < nb_vertices; w++) {
      dist[v][w] = adj_matrix[v][w];
      parent[v][w] = (adj_matrix[v][w] != INFINITY) ? v : -1;
    }
  }  
  // Floyd-Warshall's algorithm
  for (int k = 0; k < n; k++) {
    for (int v = 0; v < nb_vertices; v++) {
      for (int w = 0; w < nb_vertices; w++) {
        double new_distance = dist[v][k] + dist[k][w];
        if (new_distance < dist[v][w]) {
          dist[v][w] = new_distance;
          parent[v][w] = parent[k][w];
        }
      }
    }
  } 
  return;
} 
