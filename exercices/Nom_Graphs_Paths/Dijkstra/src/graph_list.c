/**
 * @file graph_list.c
 *
 * @author Grimaud
 * @date 2024-06-05
 *
 * @brief Header file for graph representation using adjacency lists.
 * 
 * This file contains the data structures and function declarations necessary for 
 * representing and manipulating graphs using adjacency lists. It defines the structures 
 * for vertices, edges, and graphs, and provides functions for creating, deleting, 
 * and accessing graph elements. The adjacency list representation is efficient for 
 * sparse graphs and allows for easy traversal and manipulation of graph data.
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
 */

#include "graph_list.h"
#include <stdio.h>
#include <stdlib.h>

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
  g->adj_lists = (adj_list_s **)malloc(nb_vertices * sizeof(adj_list_s *));
  if (!g->adj_lists) {
    free(g);
    return NULL; // Memory allocation failed
  }
  for (int i = 0; i < nb_vertices; i++) 
    g->adj_lists[i] = NULL;
  for (int i = 0; i < nb_edges; i++) {
    adj_list_s *new_node = (adj_list_s *)malloc(sizeof(adj_list_s));
    if (!new_node) {
      delete_graph(g);
      return NULL; // Memory allocation failed
    }
    new_node->vertex.ind = edges[i].dst;
    new_node->vertex.weight = edges[i].weight;
    new_node->vertex.prev = edges[i].src;
    new_node->next = g->adj_lists[edges[i].src];
    g->adj_lists[edges[i].src] = new_node;
    if (!directed) {
      adj_list_s *new_node_undirected = (adj_list_s *)malloc(sizeof(adj_list_s));
      if (!new_node_undirected) {
        delete_graph(g);
        return NULL; // Memory allocation failed
      }
      new_node_undirected->vertex.ind = edges[i].src;
      new_node_undirected->vertex.weight = edges[i].weight;
      new_node_undirected->vertex.prev = edges[i].dst;
      new_node_undirected->next = g->adj_lists[edges[i].dst];
      g->adj_lists[edges[i].dst] = new_node_undirected;
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
  if (!g) return;
  for (int i = 0; i < g->nb_vertices; i++) {
    adj_list_s *current = g->adj_lists[i];
    while (current) {
      adj_list_s *next = current->next;
      free(current);
      current = next;
    }
  }
  free(g->adj_lists);
  free(g);
  return;
}

/**
 * @brief Gets the adjacency list of a vertex in the graph.
 * 
 * @param g Pointer to the graph
 * @param ind Index of the vertex
 * 
 * @return Pointer to the adjacency list of the vertex
 */
adj_list_s *get_adj_list(graph_s *g, int ind) {
  if (!g || ind >= g->nb_vertices) return NULL;
  else return g->adj_lists[ind];
}

/**
 * @brief Prints the details of the graph to the standard output.
 * 
 * @param g Pointer to the graph to be printed
 */
void print(graph_s *g) {
  if (!g) return;
  printf("%s graph of %d vertices:\n",(g->directed?"Directed":"Undirected"),g->nb_vertices);
  printf("┌─────┐\n");
  for (int i = 0; i < g->nb_vertices; i++) {
    adj_list_s *current = g->adj_lists[i];
    printf("│ %3d │", i);
    while (current) {
      printf(" →%d(%2.1f)", current->vertex.ind, current->vertex.weight);
      current = current->next;
    }
    printf("\n");
  }
  printf("└─────┘\n");
  return;
}

