/**
 * @file graph_matrix.c
 *
 * @brief Implementation of functions for graph representation and manipulation using adjacency matrices.
 * 
 * This source file contains the implementations of functions for handling graphs
 * represented by adjacency matrices. It includes functions for creating and deleting graphs, as well as 
 * printing the adjacency matrix and the shortest path matrices computed by the Floyd-Warshall algorithm.
 * 
 * Structures:
 * - `graph_s`: Represents a graph with its properties and matrices.
 * - `edge_s`: Represents an edge in the graph with source, destination, and weight.
 * 
 * Functions:
 * - `create_graph`: Creates a graph with specified vertices, edges, and directed property.
 * - `delete_graph`: Frees the resources associated with a graph.
 * - `print`: Prints the adjacency matrix of the graph.
 * - `print_matrix`: Prints a given matrix in a formatted way.
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
#include <math.h>
#include "graph_matrix.h"

/**
 * @brief Creates a graph.
 * 
 * @param nb_vertices Number of vertices in the graph
 * @param nb_edges Number of edges in the graph
 * @param directed Boolean indicating if the graph is directed
 * @param edges Array of edges in the graph
 * 
 * @return Pointer to the created graph
 */
graph_s *create_graph(int nb_vertices, int nb_edges, bool directed, edge_s *edges) {
  graph_s *g = (graph_s *)malloc(sizeof(graph_s));
  if (!g) return NULL; // Memory allocation failed
  
  g->nb_vertices = nb_vertices;
  g->nb_edges = nb_edges;
  g->directed = directed;

  g->adj_matrix = (double **)malloc(nb_vertices * sizeof(double *));
  g->dist = (double **)malloc(nb_vertices * sizeof(double *));
  g->parent = (int **)malloc(nb_vertices * sizeof(int *));
  if (!g->adj_matrix || !g->dist || !g->parent) {
    delete_graph(g);
    return NULL;
  }

  for (int i = 0; i < nb_vertices; i++) {
    g->adj_matrix[i] = (double *)malloc(nb_vertices * sizeof(double));
    g->parent[i] = (int *)malloc(nb_vertices * sizeof(int));
    g->dist[i] = (double *)malloc(nb_vertices * sizeof(double));
    if (!g->adj_matrix[i] || !g->dist[i] || !g->parent[i]) {
      delete_graph(g);
      return NULL;
    }
    for (int j = 0; j < nb_vertices; j++) {
      g->adj_matrix[i][j] = (i == j) ? 0 : INFINITY;
    }
  }
  
  // Initialize weights
  for (int i = 0; i < nb_edges; i++) {
    g->adj_matrix[edges[i].src][edges[i].dst] = edges[i].weight;
    if (!directed) {
      g->adj_matrix[edges[i].dst][edges[i].src] = edges[i].weight;
    }
  }
 
  return g;
}

/**
 * @brief Deletes a graph and frees its memory.
 * 
 * @param g Pointer to the graph to be deleted
 */
void delete_graph(graph_s *g) {
  if (g) {
    for (int i = 0; i < g->nb_vertices; i++) {
      if(g->adj_matrix && g->adj_matrix[i]) free(g->adj_matrix[i]);
      if(g->dist       && g->dist[i])       free(g->dist[i]);
      if(g->parent     && g->parent[i])     free(g->parent[i]);
    }
    if(g->adj_matrix) free(g->adj_matrix);
    if(g->dist)       free(g->dist);
    if(g->parent)     free(g->parent);
    free(g);
  }
  return;
}

/**
 * @brief Prints the adjacency matrix of the graph.
 * 
 * This function prints the given adjacency matrix in a formatted table. Each
 * cell contains the weight of the edge between vertices, 
 * with infinity represented by the ∞ symbol for INFINITY values. The matrix is
 * labeled with source and destination vertex indices.
 * 
 * @param m The adjacency matrix to be printed.
 * @param nb_vertices The number of vertices in the graph.
 */
void print_matrix(double **m, int nb_vertices) {
  // Print top border
  printf("┌───────┬");
  for (int i = 0; i < nb_vertices; i++) {
    printf("──────");
  }
  printf("┐\n");
  
  // Print header row with column indices
  printf("│src\\dst│");
  for (int i = 0; i < nb_vertices; i++) {
    printf("%4d  ", i);
  }
  printf("│\n");
  
  // Print separator row
  printf("├───────┼");
  for (int i = 0; i < nb_vertices; i++) {
    printf("──────");
  }
  printf("┤\n");
  
  // Print matrix rows
  for (int i = 0; i < nb_vertices; i++) {
    printf("│   %3d │", i);
    for (int j = 0; j < nb_vertices; j++) {
      if (m[i][j] == INFINITY) {
	printf("   ∞  "); // Use infinity symbol for INFINITY
      } else {
	printf("%5.1f ", m[i][j]); // Print weights
      }
    }
    printf("│\n");
  }
  
  // Print bottom border
  printf("└───────┴");
  for (int i = 0; i < nb_vertices; i++) {
    printf("──────");
  }
  printf("┘\n");
  return;
}


/**
 * @brief Prints the details of the graph to the standard output.
 * 
 * @param g Pointer to the graph to be printed
 */
void print(graph_s *g) {
  if (!g || !g->adj_matrix) return;
  printf("%s graph of %d vertices:\n",(g->directed?"Directed":"Undirected"),g->nb_vertices);
  
  print_matrix(g->adj_matrix, g->nb_vertices);
  return;
}

