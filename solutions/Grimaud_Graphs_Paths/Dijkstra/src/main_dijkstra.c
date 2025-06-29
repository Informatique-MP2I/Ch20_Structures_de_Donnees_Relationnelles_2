/**
 * @file main_dijkstra.c
 *
 * @author Grimaud
 * @date 2024-06-05
 *
 * @brief Implementation and test of Dijkstra's algorithm using an adjacency list graph and a priority queue.
 *
 * This file contains the implementation of Dijkstra's algorithm for finding the shortest path in a graph.
 * The graph is represented using an adjacency list and the priority queue is implemented as a binary heap.
 * The program defines the graph structure with vertices and edges, initializes the graph, and performs
 * Dijkstra's algorithm starting from a source vertex. It also includes functionality to print the shortest
 * path from the source to a target vertex. The main function demonstrates these capabilities by creating a
 * sample graph, running the algorithm, and displaying the results.
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
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <getopt.h>
#include <math.h>
#include <assert.h>
#include "graph_list.h"
#include "heap.h"

/**
 * @brief Performs Dijkstra's algorithm to find the shortest paths from the source vertex.
 * 
 * @param g The graph.
 * @param src The source vertex.
 * @return An array of vertices with the shortest path information.
 */
vertex_s *dijkstra(graph_s *g, int src) {
  // Allocate memory for the result array dist
  int nb_vertices = g->nb_vertices;
  vertex_s *dist = (vertex_s *)malloc(nb_vertices*sizeof(vertex_s));
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
  printf("Initial heap:\n");
  // While it remains vertices to visit
  while (!heap_empty(q)) {
    heap_print(q); printf("\n");
    // Dequeue the the highest priority vertex
    vertex_s v = heap_peek(q);
    q = heap_remove(q);    
    printf("Processing vertex %d from the queue:\n", v.ind);
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

/**
 * @brief Prints the shortest path from the source to the target vertex.
 * 
 * @param g The graph.
 * @param v The array of vertices with the shortest path information.
 * @param dst The destination vertex.
 */
void print_path(graph_s *g, vertex_s *v, int dst) {
  // Check if the destination is valid
  if (!g || dst >= g->nb_vertices || dst < 0) {
    printf("Invalid destination vertex.\n");
    return;
  }
  // Use an array in reverse order (or a stack is possible)
  int path[g->nb_vertices];
  int path_length = 0;
  int current = dst;
  // Trace the path back from the destination to the source
  while (v[current].prev != -1) {
    path[path_length++] = current;
    current = v[current].prev;
  }
  // Add the source vertex
  path[path_length++] = current;
  // Print the path in the correct order
  for (int i = path_length - 1; i >= 0; i--) {
    printf("%d", path[i]);
    if (i > 0) printf(" → ");
  }
  printf("\n");
  return;
}

/**
 * @brief Prints the help message with usage examples.
 */
void print_help(char *prog_name) {
  printf("Usage: %s [OPTIONS]\n",prog_name);
  printf("Options:\n");
  printf("  -h, --help              Show this help message and exit\n");
  printf("  -d, --directed          Specify that the graph is a directed graph (default: undirected)\n");
  printf("  -v, --vertices <number> Specify the number of vertices\n");
  printf("  -a, --adjacencies       Specify the adjacency list in the format \"src:dst1,dst2 ...\"\n");
  printf("  -s, --start             Specify the start vertex for Dijkstra (default: 0)\n");
  printf("\nExamples:\n");
  printf("  %s -v 8 -a \"0:1/1.0,2/2.0 1:2/1.5 2:3/1.0 3:5/8.1,6/5.1 5:7/0.7,4/9.1\" -s 3\n",prog_name);
  printf("  %s --vertices 5 --adjancencies \"0:1/1.0,2/2.0 1:2/1.5 2:3/1.0\" --directed\n",prog_name);
}

/**
 * @brief Main function to test Dijkstra's algorithm implementation with command line options.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Exit status.
 */
int main(int argc, char **argv) {
  int vertices = 0;
  char *edges_list = NULL;
  bool directed = false;
  int initial_vertex = 0;
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
    } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--adjacencies") == 0) {
      if (i + 1 < argc) {
        edges_list = argv[++i];
      } else {
        fprintf(stderr, "Error: Missing argument for --adjacencies\n");
        return 1;
      }
    } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--start") == 0) {
      if (i + 1 < argc)
	      initial_vertex = atoi(argv[++i]);
      else {
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
  edge_s edges[vertices * vertices]; // Assume a maximum possible number of edges
  int edge_count = 0;
  // Parsing edges_list
  const char *ptr = edges_list;
  while (*ptr != '\0') {
    int start_vertex = (int)strtol(ptr,(char **)&ptr,10);
    int end_vertex = 0;
    double weight = 0.0;
    if (*ptr++ != ':') {
      fprintf(stderr, "Error: Invalid edge format (missing ':' ?)\n");
      for(int i=0;i<ptr-edges_list+1;i++) fprintf(stderr,"-"); fprintf(stderr,"v"); fprintf(stderr, "\n");
      fprintf(stderr, "\"%s\"\n",edges_list);
      return 1;
    }
    // Read each connection for the current start vertex
    while (*ptr != ' ' && *ptr != '\0') {
      end_vertex = (int)strtol(ptr,(char **)&ptr,10);
      weight = 0.0;
      if (*ptr++ != '/') {
        fprintf(stderr, "Error: Invalid edge format (missing '/' ?)\n");
        for(int i=0;i<ptr-edges_list+1;i++) fprintf(stderr,"-"); fprintf(stderr,"v"); fprintf(stderr, "\n");
        fprintf(stderr, "\"%s\"\n",edges_list);
        return 1;
      }
      // Read weight
      weight = strtod(ptr, (char **)&ptr);
      if (*ptr == ',' || *ptr == ' ' || *ptr == '\0') {
        edges[edge_count++] = (edge_s){start_vertex, end_vertex, weight};
        if (*ptr == ',') ptr++;
      } else {
        fprintf(stderr, "Error: Invalid edge format here (',' or ' ' expected).\n");
        for(int i=0;i<ptr-edges_list+1;i++) fprintf(stderr,"-"); fprintf(stderr,"v"); fprintf(stderr, "\n");
        fprintf(stderr, "\"%s\"\n",edges_list);
        return 1;
      }
    }
    while (*ptr == ' ') ptr++;
  }
  graph_s *g = create_graph(vertices, edge_count, directed, edges);
  if (!g) {
    fprintf(stderr, "Error: Failed to create graph\n");
    return 1;
  }
  printf("The initial Graph:\n");
  print(g);

  // Dijkstra algorithm process - beginning
  vertex_s *dst = dijkstra(g, initial_vertex);
  printf("\nResulting Dijkstra shortest path array:\n");
  for (int i = 0; i < g->nb_vertices; i++)
    printf("[% 2d, %02.2f, % 2d]\n", dst[i].ind, dst[i].weight, dst[i].prev);
  printf("\nResulting Dijkstra shortest paths from vertex %d:\n", initial_vertex);
  for (int i = 0; i < g->nb_vertices; i++) {
    if (dst[i].weight == INFINITY)
      printf("to vertex %d, length   ∞ : \n", i); 
    else{
      printf("to vertex %d, length %.2f: ", i, dst[i].weight);
      print_path(g, dst, i);
    }
  }  
  free(dst);
  // Dijkstra algorithm process - end

  // delete the graph_s
  delete_graph(g);
  
  // that's all folk !
  return 0;
}
