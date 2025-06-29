/**
 * @file main_bellman_ford.c
 *
 * @brief Implementation and test of Bellman-Ford algorithm using an adjacency matrix graph.
 * 
 * This file contains the implementation of the Bellman-Ford algorithm for finding the shortest paths
 * from a single source vertex to all other vertices in a graph. The graph is represented using an adjacency 
 * matrix. The algorithm detects negative weight cycles and stops if such cycles are found.
 * 
 * Key functionalities include:
 * - Initializing the graph's distance and parent arrays for shortest path computation.
 * - Running the Bellman-Ford algorithm to compute the shortest paths.
 * - Detecting and reporting negative weight cycles.
 * - Printing the shortest path from the source to any destination vertex using the parent array.
 * 
 * The main function demonstrates these capabilities by parsing command-line arguments to create and configure 
 * the graph, running the algorithm, and displaying the results.
 *
 * @author Grimaud
 * 
 * @license
 * This code is licensed under the GNU Lesser General Public License (LGPL). 
 * You can redistribute it and/or modify it under the terms of the GNU Lesser General Public License 
 * as published by the Free Software Foundation, either version 3 of the License, or (at your option) 
 * any later version.
 * 
 * This code is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License along with this 
 * code. If not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.
 *
 * @date 2024-11-28
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "graph_matrix.h"

/**
 * @brief Applies the Bellman-Ford algorithm to find shortest paths from a single source vertex.
 * 
 * This function computes the shortest path lengths and parent relationships from a single source vertex to 
 * all other vertices in the graph. It iteratively relaxes edges and updates the distance and parent arrays.
 * If a negative weight cycle is detected, the algorithm reports it and stops further processing.
 * 
 * @param g Pointer to the graph structure containing adjacency matrix, distance array, and parent array.
 * @param src The source vertex index.
 */
void bellman_ford(graph_s *g, int src) {
  assert(g && g->adj_matrix && g->dist && g->parent); 
  
  int nb_vertices = g->nb_vertices;
  double **adj_matrix = g->adj_matrix;
  double *dist = g->dist;
  int *parent = g->parent;

  // Initialisation of the arrays : dist and parent
  for (int w = 0; w < nb_vertices; w++) {
    dist[w] = (w != src) ? INFINITY:0.0;
    parent[w] = (w != src) ? -1: w;
  }
 
  // Bellmon-Ford's algorithm
  for (int k = 1; k < nb_vertices - 1; k++) {
x    for (int w = 0; w < nb_vertices; w++) {
      for (int u = 0; u < nb_vertices; u++) {
        if (adj_matrix[u][w] != INFINITY) {
          double new_distance = dist[u] + adj_matrix[u][w];
          if (new_distance < dist[w]) {
            dist[w] = new_distance;
            parent[w] = u;
          }
        }
      }
    }
  }

  // TODO
  for (int w = 0; w < nb_vertices; w++) {
    printf("w : %d - dist : %f, parent : %d\n", w, dist[w], parent[w]);
  }
  printf("fin bellman-ford\n");
  
  // return; 
  for (int w = 0; w < nb_vertices; w++) {
    for (int u = 0; u < nb_vertices; u++) {
      if (adj_matrix[u][w] != INFINITY) {
        double new_distance = dist[u] + adj_matrix[u][w];
        if (new_distance < dist[w]) {
          // A shorter path found, indicates a negative weight cycle
          g->neg_weight_cycle = true;
          printf("Negative cycle detected at vertex %d.\n", w);
          return; // Stop further processing
        }
      }
    }
  }
}

/**
 * @brief Prints the shortest path from the source vertex to a destination vertex.
 * 
 * This function reconstructs and prints the path from the source vertex to a given destination vertex based 
 * on the parent array. If no path exists, it prints an appropriate message.
 * 
 * @param g Pointer to the graph structure containing the parent array.
 * @param dst The destination vertex index.
 */
void print_path(graph_s *g, int dst) {
  if (!g || dst >= g->nb_vertices || dst < 0) {
    printf("Invalid vertices.\n");
    return;
  }
  if (g->parent[dst] == -1) {
    printf("No path to %d.\n", dst);
    return;
  }

  // Use an array in reverse order (or a stack is possible)
  int path[g->nb_vertices];
  int path_length = 0;
  int current = dst;
  // Trace the path back from the destination to the source
  while (g->parent[current] != current) {
    path[path_length++] = current;
    current = g->parent[current];
  }
  // Add the source vertex
  path[path_length++] = current;

  // Print the path in the correct order
  for (int i = path_length - 1; i >= 0; i--) {
    printf("%d", path[i]);
    if (i > 0) printf(" -> ");
  }
  printf("\n");
  return;
}


/**
 * @brief Prints the help message with usage examples.
 */
void print_help(char *prog_name) {
  printf("Usage: %s [OPTIONS]\n", prog_name);
  printf("Options:\n");
  printf("  -h, --help              Show this help message and exit\n");
  printf("  -d, --directed          Specify that the graph is a directed graph (default: undirected)\n");
  printf("  -v, --vertices <number> Specify the number of vertices\n");
  printf("  -a, --adjacencies       Specify the adjacency list in the format \"src:dst1/weight1,dst2/weight2 ...\"\n");
  printf("\nExamples:\n");
  printf("  %s -v 8 -a \"0:1/1.0,2/2.0 1:2/1.5 2:3/1.0 3:5/8.1,6/5.1 5:7/0.7,4/9.1\"\n", prog_name);
  printf("  %s --vertices 5 --adjacencies \"0:1/1.0,2/2.0 1:2/1.5 2:3/1.0\" --directed\n", prog_name);
  return;
}

/**
 * @brief Main function to create and test the Bellman-Ford algorithm on a graph.
 * 
 * The program parses command-line arguments to configure the graph, including the number of vertices, edges, 
 * and the source vertex for the Bellman-Ford algorithm. It then creates the graph, runs the algorithm, and 
 * displays the shortest paths or reports if a negative weight cycle exists.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status.
 */
int main(int argc, char **argv) {
  int vertices = 0;
  char *edges_list = NULL;
  bool directed = false;
  int start_vertex = 0; // Default start vertex
  
  // parse options
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
      print_help(argv[0]);
      return 0;
    } else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--directed") == 0) {
	directed = true;
    } else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--vertices") == 0) {
      if (i + 1 < argc) {
        vertices = atoi(argv[++i]);
      } else {
	fprintf(stderr, "Error: Missing argument for --vertices\n");
	return 1;
      }
    } else if (strcmp(argv[i], "-a") == 0||strcmp(argv[i], "--adjacencies")==0) {
      if (i + 1 < argc) {
        edges_list = argv[++i];
      } else {
        fprintf(stderr, "Error: Missing argument for --adjacencies\n");
        return 1;
      }
    } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--start") == 0) {
      if (i + 1 < argc) {
	start_vertex = atoi(argv[++i]);
      } else {
	fprintf(stderr, "Error: Missing argument for --start\n");
	return 1;
      }
    }
  }
  if (vertices == 0 || edges_list == NULL) {
    fprintf(stderr, "Error: --vertices and --adjacencies are required\n\n");
    print_help(argv[0]);
    return 1;
  }
  
  edge_s edges[vertices * vertices];// Assume a maximum possible number of edges
  int edge_count = 0;
  
  // Parsing edges_list 
  const char *ptr = edges_list;
  while (*ptr != '\0') {
    int start_vertex = 0;
    int end_vertex = 0;
    double weight = 0.0;
    
    // Read start vertex
    while (*ptr >= '0' && *ptr <= '9') {
      start_vertex = start_vertex * 10 + (*ptr - '0');
      ptr++;
    }
    if (*ptr == ':') ptr++;
    else {
      fprintf(stderr, "Error: Invalid edge format (missing ':' ?)\n");
      for (int i = 0; i < ptr - edges_list + 1; i++) fprintf(stderr, "-");
      fprintf(stderr, "v");
      fprintf(stderr, "\n");
      fprintf(stderr, "\"%s\"\n", edges_list);
      return 1;
    }
    
    // Read each connection for the current start vertex
    while (*ptr != ' ' && *ptr != '\0') {
      end_vertex = 0;
      weight = 0.0;
      
      // Read end vertex
      while (*ptr >= '0' && *ptr <= '9') {
	end_vertex = end_vertex * 10 + (*ptr - '0');
	ptr++;
      }
      if (*ptr == '/') ptr++;
      else {
	fprintf(stderr, "Error: Invalid edge format (missing '/' ?)\n");
	for (int i = 0; i < ptr - edges_list + 1; i++) fprintf(stderr, "-");
	fprintf(stderr, "v");
	fprintf(stderr, "\n");
	fprintf(stderr, "\"%s\"\n", edges_list);
	return 1;
      }
      
      // Read weight
      char *weight_start = (char *)ptr;
      weight = strtod(weight_start, (char **)&ptr);
      if (*ptr == ',' || *ptr == ' ' || *ptr == '\0') {
	if(edge_count>vertices*vertices) {
	  fprintf(stderr, "Error: Too much edges, did you define more than one edges per couple of vertex ?\n");
	  return 1;
	}
	edges[edge_count++] = (edge_s){start_vertex, end_vertex, weight};
	if (*ptr == ',') ptr++;
      } else {
	fprintf(stderr, "Error: Invalid edge format here (',' or ' ' expected).\n");
	for (int i = 0; i < ptr - edges_list + 1; i++) fprintf(stderr, "-");
	fprintf(stderr, "v");
	fprintf(stderr, "\n");
	fprintf(stderr, "\"%s\"\n", edges_list);
	return 1;
      }
    }
    while (*ptr == ' ') ptr++;
  }
  
  // create the graph_s 
  graph_s *g = create_graph(vertices, edge_count, directed, edges);
  if (!g) {
    fprintf(stderr, "Error: Failed to create graph\n");
    return 1;
  }
  print(g);

  // Bellman-Ford algorithm process - beginning
  bellman_ford(g, start_vertex);
  printf("Resulting Bellman-Ford shortest paths :\n");

  if (!g->neg_weight_cycle){
  for (int i = 0; i < g->nb_vertices; i++) {
    if (g->dist[i] == INFINITY)
      printf("to vertex %d, length   ∞ : \n", i); 
    else{
      printf("to vertex %d, length %.2f: ", i, g->dist[i]);
      print_path(g, i);
    }
  }
  } else
    printf("Graph contains a negative weight cycle. No shortest paths printed.\n");
  // Bellman-Ford algorithm process - end
  
  // delete the graph_s
  delete_graph(g);
  
  // that's all folk !
  return 0;
}

