/**
 * @file heap.h
 *
 * @author Grimaud
 * @date 2024-06-05
 *
 * @brief Structures and functions for managing a heap data structure.
 * 
 * This file contains the definitions and function declarations for implementing a heap (priority queue). 
 * The heap is used to efficiently manage a collection of elements with associated priorities, where the element 
 * with the highest priority (smallest value) can be quickly accessed and removed. The provided functions allow 
 * for creating a heap, adding elements, checking if the heap is empty, peeking at the top element, removing 
 * the top element, printing the heap's contents, and deleting the heap.
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

#ifndef HEAP_H
#define HEAP_H

#include "graph_list.h"

/** 
 * @struct heap_s
 * @brief Structure of the heap_s.
 */
typedef struct heap heap_s;
 
/** 
 * @brief Creates a new heap and initializes it.
 * @return A pointer to the newly created empty heap.
 */
heap_s *heap_create(int nb_ind);

/** 
 * @brief Adds a given value to the heap.
 * @param vertex A new vertex to add.
 * @param heap The address of the current heap.
 * @return The address of the updated heap.
 * @note Asserts that the heap is already created and has enough space.
 */
heap_s *heap_add(vertex_s vertex, heap_s *heap);

/** 
 * @brief Tests if the heap is empty.
 * @param heap The address of the current heap.
 * @return true if the heap is empty, false otherwise.
 * @note Asserts that the heap is created.
 */
bool heap_empty(heap_s *heap);

/** 
 * @brief Reads the head element without removing it.
 * @param heap The address of the current heap.
 * @return The value of the head element.
 * @note Asserts that the heap is not empty.
 */
vertex_s heap_peek(heap_s *heap);

/** 
 * @brief Removes the head element.
 * @param heap The address of the current heap.
 * @return The address of the updated heap.
 * @note Asserts that the heap is not empty.
 */
heap_s *heap_remove(heap_s *heap);

/** 
 * @brief Prints the heap elements from the head to the last element.
 * @param heap The address of the current heap.
 * @note Asserts that the heap is already created.
 */
void heap_print(heap_s *heap);

/** 
 * @brief Erases the heap.
 * @param heap The address of the current heap.
 * @note Asserts that the heap is already created.
 */
void heap_delete(heap_s *heap);

#endif // HEAP_H
