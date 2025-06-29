/**
 * @file main_dijkstra_markov.c
 *
 * @author Grimaud
 * @date 2024-11-27
 *
 * @brief Implementation and test of an adapted Dijkstra's algorithm for Markov chain graphs.
 *
 * This file contains an adapted version of Dijkstra's algorithm, specialized for Markov chains. 
 * In this context, the graph is represented using an adjacency list, and edge weights represent 
 * transition probabilities. The algorithm calculates the most probable paths in the Markov graph, 
 * with probabilities interpreted logarithmically to facilitate calculations. 
 * 
 * The program includes:
 * - A function to verify if a graph satisfies Markov chain properties.
 * - An adapted Dijkstra's algorithm to find the most probable paths.
 * - Utilities to print the resulting path as a sequence of states or transitions.
 * - A main function demonstrating these features.
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

#define EPSILON 1e-6 // Tolerance for floating-point comparisons

/**
 * @brief Verifies whether a graph satisfies Markov chain properties.
 * 
 * This function checks if a directed graph satisfies the Markov chain conditions:
 * - The graph must be directed.
 * - The sum of outgoing edge weights from each vertex must equal 1.
 * 
 * If any vertex does not satisfy these conditions, the function returns `false` and 
 * prints a diagnostic message.
 * 
 * @param g Pointer to the graph structure.
 * @return `true` if the graph satisfies Markov properties, otherwise `false`.
 */
bool check_markov(graph_s *g) {
  // Check if the graph is directed
  if (!g->directed) {
    printf("The graph is not directed.\n");
    return false;
  } 
  // Iterate over all vertices to verify the Markov chain conditions
  for (int i = 0; i < g->nb_vertices; i++) {
    double sum_weights = 0.0;
    // Traverse the adjacency list for the current vertex
    adj_list_s *current = g->adj_lists[i];
    // If the vertex has no outgoing edges, skip verification
    if (current == NULL) {
      continue;
    }
    while (current != NULL) {
      sum_weights += current->vertex.weight;
      current = current->next;
    }
    // Check if the sum of outgoing edge weights equals 1
    if (fabs(sum_weights - 1.0) > EPSILON) {
      printf("Vertex %d does not satisfy the Markov condition (sum of weights = %f).\n", i, sum_weights);
      return false;
    }
  }
  // All vertices satisfy the Markov condition
  return true;
}

/**
 * @brief Performs an adapted Dijkstra's algorithm for Markov chain graphs.
 * 
 * This function computes the most probable paths from a source vertex to all other 
 * vertices in a Markov chain graph. It uses logarithms to handle probabilities 
 * (converting multiplications into additions and avoiding underflow issues).
 * 
 * The results are stored in an array of `vertex_s` structures, containing:
 * - The vertex index.
 * - The computed probability (logarithmic scale for intermediate calculations).
 * - The predecessor vertex for reconstructing the path.
 * 
 * @param g Pointer to the graph structure.
 * @param src The source vertex.
 * @return A dynamically allocated array of `vertex_s` containing the results.
 */
vertex_s *dijkstra_markov(graph_s *g, int src) {
  // Allocate memory for the result array dist
  int nb_vertices = g->nb_vertices;
  vertex_s *dist = malloc(nb_vertices*sizeof(vertex_s));
  assert(dist!=NULL);
  // Create an array of visited vertices and setup dist
  bool visited_vertices[nb_vertices];
  for (int i = 0; i < nb_vertices; i++) {
    visited_vertices[i] = false;
    dist[i].ind = i;
    dist[i].weight = 0.0;
    dist[i].prev = -1;
  }
  // Create a heap to record the next vertex to visit
  heap_s *q = heap_create(nb_vertices);
  // Start with the source vertex
  dist[src].weight=1.0;
  q = heap_add(dist[src], q);
  // While it remains vertices to visit
  while (!heap_empty(q)) {
    // Dequeue the the highest priority vertex
    vertex_s v = heap_peek(q);
    q = heap_remove(q);
    v.weight = exp(-v.weight);
    if (!visited_vertices[v.ind]) {
      // Mark the current vertex as visited and update dist
      visited_vertices[v.ind] = true;
      dist[v.ind].weight = v.weight;
      dist[v.ind].prev = v.prev;
      // Process all adjacent vertices
      adj_list_s *adj = get_adj_list(g, v.ind);
      while (adj != NULL) {
	// Calculate new weight and update if it's smaller
	double new_weight =  v.weight * adj->vertex.weight;
	if (new_weight > dist[adj->vertex.ind].weight) {
	  vertex_s tmp = {.ind = adj->vertex.ind,
			  .weight=-log(new_weight),
			  .prev=v.ind};
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
 * @brief Prints the most probable path as a sentence of states or transitions.
 * 
 * This function reconstructs the most probable path from the source to a given 
 * destination vertex using the predecessor array. It then prints the path as 
 * a sequence of words or transitions, specified by the `words` array.
 * 
 * @param g Pointer to the graph structure.
 * @param v Array of `vertex_s` containing the shortest path information.
 * @param dst The destination vertex index.
 * @param words Array of strings representing the "labels" for the vertices.
 */
void print_sentence(graph_s *g, vertex_s *v, int dst, const char **words) {
  // Check if the destination is valid
  if (dst >= g->nb_vertices || dst < 0) {
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
    printf("%s ", words[path[i]]);
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
 * @brief Main function to test the adapted Dijkstra's algorithm for Markov graphs.
 * 
 * The program parses command-line arguments to configure the graph, including:
 * - Number of vertices.
 * - Edges and weights, given as adjacency lists.
 * - Whether the graph is directed.
 * - The source vertex for the algorithm.
 * 
 * The program verifies if the graph satisfies Markov chain conditions, computes 
 * the most probable paths, and prints the resulting path as a sentence of states 
 * or transitions.
 * 
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status (0 for success, 1 for errors).
 */
int main(int argc, char **argv) {
  int vertices = 0;
  char *edges_list = NULL;
  bool directed = false;
  int start_vertex = 0; // Default start vertex
  
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
  if (vertices == 0 || edges_list == NULL) {
    fprintf(stderr, "Error: --vertices and --adjacencies are required\n\n");
    print_help(argv[0]);
    return 1;
  }
  edge_s edges[vertices * vertices]; // Assume a maximum possible number of edges
  int edge_count = 0;
  // Parsing edges_list without strtok
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
      for(int i=0;i<ptr-edges_list+1;i++) fprintf(stderr,"-"); fprintf(stderr,"v"); fprintf(stderr, "\n");
      fprintf(stderr, "\"%s\"\n",edges_list);
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
        for(int i=0;i<ptr-edges_list+1;i++) fprintf(stderr,"-"); fprintf(stderr,"v"); fprintf(stderr, "\n");
        fprintf(stderr, "\"%s\"\n",edges_list);
        return 1;
      }
      // Read weight
      char *weight_start = (char *)ptr;
      weight = strtod(weight_start, (char **)&ptr);
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
  print(g);
  printf(check_markov(g)?"true\n":"false\n");
  vertex_s *dst = dijkstra_markov(g, start_vertex);
  const char *words[] = {"", "On a étudié", "On a créé", "On a vu", "un exemple", "une vidéo", "d'un graphe", "d'un exo", ""};
  print_sentence(g, dst, 8, words);
  
  delete_graph(g);
  free(dst);
  return 0;
}

