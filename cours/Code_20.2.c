vertex_s *dijkstra(graph_s *g, int src) {
  // Allocate memory for the result array dist
  int nb_vertices = g->nb_vertices;
  vertex_s *dist = malloc(nb_vertices*sizeof(vertex_s));
  assert(dist!=NULL);
  // Create an array of visited vertices and setup dist
  bool visited_vertices[nb_vertices];
  for (int i = 0; i < nb_vertices; i++) {
    visited_vertices[i] = false;
    dist[i].ind = i;
    dist[i].weight = INFINITY;
    dist[i].prev = -1;
  }
  // Create a heap to record the next vertex to visit
  heap_s *q = heap_create(nb_vertices);
  // Start with the source vertex
  dist[src].weight=0.0;
  q = heap_add(dist[src], q);
  // While it remains vertices to visit
  while (!heap_empty(q)) {
    // Dequeue the the highest priority vertex
    vertex_s v = heap_peek(q);
    q = heap_remove(q);    
    if (!visited_vertices[v.ind]) {
      // Mark the current vertex as visited and update dist
      visited_vertices[v.ind] = true;
      dist[v.ind].weight = v.weight;
      dist[v.ind].prev = v.prev;
      // Process all adjacent vertices
      adj_list_s *adj = get_adj_list(g, v.ind);
      while (adj != NULL) {
      	// Calculate new weight and update if it's smaller
        double new_weight = v.weight + adj->vertex.weight;
        if (new_weight < dist[adj->vertex.ind].weight) {
          vertex_s tmp = {.ind = adj->vertex.ind,
                          .weight=new_weight,
                          .prev=v.ind
          };
          q = heap_add(tmp,q);
        }
        adj = adj->next;
      }
    }
  }
  // Clean up
  heap_delete(q);
  return dist;
}
