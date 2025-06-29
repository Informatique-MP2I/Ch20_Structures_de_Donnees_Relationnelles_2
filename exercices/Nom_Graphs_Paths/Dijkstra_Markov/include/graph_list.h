/**
 * @file graph_list.h
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

#ifndef GRAPH_LIST_H
#define GRAPH_LIST_H

#include <stdbool.h>

/**
 * @brief Structure representing a vertex in the graph.
 */
typedef struct {
    int ind;          /**< Index of the vertex */
    double weight;    /**< Distance of the vertex to the source */
    int prev;         /**< Index of the previous vertex between the current vertex and the source */
} vertex_s;

/**
 * @brief Structure representing the adjacency list of vertices.
 */
typedef struct adj_list {
    vertex_s vertex;      /**< Vertex in the adjacency list */
    struct adj_list *next; /**< Pointer to the next vertex in the adjacency list */
} adj_list_s;

/**
 * @brief Structure representing a graph.
 */
typedef struct {
  int nb_vertices;        /**< Number of vertices in the graph */
  int nb_edges;           /**< Number of edges in the graph */
  bool directed;          /**< Indicates if the graph is directed */
  adj_list_s **adj_lists; /**< Array of adjacency lists for each vertex */
} graph_s;

/**
 * @brief Structure representing an edge in the graph.
 */
typedef struct {
    int src;       /**< Source vertex of the edge */
    int dst;       /**< Destination vertex of the edge */
    double weight; /**< Weight of the edge */
} edge_s;

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
graph_s *create_graph(int nb_vertices, int nb_edges, bool directed, edge_s *edges);

/**
 * @brief Deletes a graph and frees its memory.
 * 
 * @param g Pointer to the graph to be deleted
 */
void delete_graph(graph_s *g);

/**
 * @brief Gets the adjacency list of a vertex in the graph.
 * 
 * @param g Pointer to the graph
 * @param ind Index of the vertex
 * 
 * @return Pointer to the adjacency list of the vertex
 */
adj_list_s *get_adj_list(graph_s *g, int ind);

/**
 * @brief Prints the details of the graph to the standard output.
 * 
 * @param g Pointer to the graph to be printed
 */
void print(graph_s *g);

#endif // GRAPH_LIST_H

