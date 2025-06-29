/**
 * @file graph_matrix.h
 *
 * @brief Definitions and functions for graph representation and manipulation using adjacency matrices.
 * 
 * This header file contains the definitions of structures and declarations of functions for handling graphs
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

#ifndef GRAPH_MATRIX_H
#define GRAPH_MATRIX_H

#include <stdbool.h>

/**
 * @brief Structure representing a graph.
 */
typedef struct graph {
  int nb_vertices;     /**< Number of vertices in the graph */
  int nb_edges;        /**< Number of edges in the graph */
  bool directed;       /**< Indicates if the graph is directed */
  double **adj_matrix; /**< Adjacency matrix representing the graph */
  double *dist;        /**< Distance array used to compute shortest paths lengths via Bellman-Ford algorithm */
  int *parent;         /**< Parent array used to reconstruct shortest paths */
  bool neg_weight_cycle;
} graph_s;


/**
 * @brief Structure representing an edge in the graph.
 */
typedef struct edge {
    int src;       /**< Source vertex of the edge */
    int dst;       /**< Destination vertex of the edge */
    double weight; /**< Weight of the edge */
} edge_s;


/**
 * @brief Create a graph with a given number of vertices and edges.
 *
 * @param nb_vertices Number of vertices in the graph.
 * @param nb_edges Number of edges in the graph.
 * @param directed Indicates if the graph is directed.
 * @param edges Array of edges to initialize the graph.
 * @return Pointer to the created graph.
 */
graph_s *create_graph(int nb_vertices, int nb_edges, bool directed, edge_s *edges);

/**
 * @brief Delete a graph and free its resources.
 *
 * @param g Pointer to the graph to delete.
 */
void delete_graph(graph_s *g);

/**
 * @brief Print the adjacency matrix of the graph.
 *
 * @param g Pointer to the graph to print.
 */
void print(graph_s *g);

/**
 * @brief Prints the adjacency matrix of the graph.
 *
 * This function prints the given adjacency matrix in a formatted table. Each cell contains the weight of the edge between vertices,
 * with infinity represented by the ∞ symbol for DBL_MAX values. The matrix is labeled with source and destination vertex indices.
 *
 * @param m The adjacency matrix to be printed.
 * @param nb_vertices The number of vertices in the graph.
 */
void print_matrix(double **m, int nb_vertices);

#endif // GRAPH_MATRIX_H

