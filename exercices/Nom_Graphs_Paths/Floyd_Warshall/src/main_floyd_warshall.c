/**
 * @file main_floyd_warshall.c
 *
 * @brief Implementation and testing of the Floyd-Warshall algorithm using an adjacency matrix representation.
 * 
 * This file implements the Floyd-Warshall algorithm for finding shortest paths between all pairs of vertices 
 * in a graph. The graph is represented as an adjacency matrix, and the program maintains additional matrices 
 * for distances and parents to construct the shortest paths.
 * 
 * Key functionalities include:
 * - Initializing and representing a graph with vertices and weighted edges.
 * - Running the Floyd-Warshall algorithm to compute shortest paths and detect negative weight cycles.
 * - Printing the shortest path between any pair of vertices using the parent matrix.
 * - Parsing user input to create and configure the graph structure dynamically.
 * 
 * The main function demonstrates these features by parsing command-line arguments to create a graph, 
 * executing the algorithm, and displaying the resulting shortest paths or reporting the presence of negative 
 * weight cycles.
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
 * @date 2024-06-12
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
 * @brief Applies the Floyd-Warshall algorithm to find shortest paths between all pairs of vertices.
 * 
 * This function computes the shortest path lengths and parent relationships for all pairs of vertices in a graph.
 * It iteratively considers each vertex as an intermediate step in potential paths, updating the distance and 
 * parent matrices accordingly. Negative weight cycles are detected by examining the diagonal of the distance 
 * matrix after the algorithm completes.
 * 
 * @param g Pointer to the graph structure containing adjacency matrix, distance matrix, and parent matrix.
 * @return `true` if the algorithm completes successfully without detecting a negative weight cycle, 
 *         `false` otherwise.
 */
bool floyd_warshall(graph_s *g) {
  assert(g && g->adj_matrix && g->dist && g->parent); 
  
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
  for (int k = 0; k < nb_vertices; k++) {
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

  /* TODO */
  
  return true;
}

/**
 * @brief Prints the shortest path from a source vertex to a destination vertex using the parent matrix.
 * 
 * This function reconstructs and prints the path between two vertices based on the parent matrix. If no 
 * path exists, it prints an appropriate message. The path is displayed in order, from source to destination.
 * 
 * @param g Pointer to the graph structure containing the parent matrix.
 * @param src The source vertex index.
 * @param dst The destination vertex index.
 */
void print_path(graph_s *g, int src, int dst) {
  /* TODO */
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
 * @brief Main function to create and demonstrate the Floyd-Warshall algorithm on a graph.
 * 
 * The program parses command-line arguments to configure the graph structure, including the number of 
 * vertices and edges. It then creates the graph, applies the Floyd-Warshall algorithm to compute shortest 
 * paths, and prints the results. If negative weight cycles are detected, a warning is displayed.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status.
 */
int main(int argc, char **argv) {
  int vertices = 0;
  char *edges_list = NULL;
  bool directed = false;
  
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

  // Floyd-Warshall algorithm process - beginning
  bool has_neg_weight_cycle = !floyd_warshall(g);
  printf("Resulting Floyd-Warshall shortest path matrix :\n");
  print_matrix(g->dist, vertices);

  if (!has_neg_weight_cycle){
    printf("\nResulting Floyd-Warshall shortest paths :\n");
    for (int v = 0; v < vertices; v++) {
      printf("from vertex %d\n", v);
      for (int w = 0; w < vertices; w++) {
	if (g->dist[v][w] == INFINITY) {
	  printf("\t to vertex %d, length   ∞    : ", w); // Use infinity symbol for INFINITY
	} else {
	  printf("\t to vertex %d, length %6.2f : ", w, g->dist[v][w]);
	}
	print_path(g, v, w);
      }
    }
  } else
    printf("Graph contains a negative weight cycle. No shortest paths printed.\n");
  // Floyd-Warshall algorithm process - end
  
  // delete the graph_s
  delete_graph(g);
  
  // that's all folk !
  return 0;
}

