/**
 * @file heap.c
 *
 * @author Grimaud
 * @date 2024-06-05
 *
 * @brief Structures and functions for managing a heap-based priority queue.
 * 
 * This file defines and implements a heap data structure, which serves as a priority queue. 
 * The heap efficiently supports insertion and deletion operations with a time complexity 
 * of O(log(n)), where n is the number of elements in the heap. It maintains the heap property, 
 * allowing quick access to the element with the highest priority (the smallest value).
 * 
 * This implementation only allows the insertion of `vertex_s` elements with a specific index, 
 * ensuring that each vertex is uniquely identified by its index within the heap. The functions 
 * provided include:
 * - Creating a heap with a specified maximum capacity.
 * - Adding a new element while maintaining the heap property.
 * - Checking if the heap is empty.
 * - Accessing the top element without removing it.
 * - Removing the top element and rebalancing the heap.
 * - Printing the contents of the heap.
 * - Deleting the heap and releasing its allocated memory.
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

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "heap.h"

/**
 * @struct heap
 * @brief Structure of the heap.
 * 
 * This structure represents a heap (priority queue) which stores elements of type vertex_s.
 * It includes an array for storing the elements, the current number of elements, 
 * and the maximum number of elements the heap can hold.
 */
typedef struct heap {
  vertex_s *array;     /**< Array of vertices in the heap */
  int *inds;           /**< Array of index of the vertice ind in the heap array */
  int nb_elements;     /**< Current number of elements in the heap */
  int max_elements;    /**< Maximum number of elements the heap can hold */
} heap_s;

/**
 * @brief Swaps two elements in the heap.
 * 
 * Exchanges the positions of two elements in the heap array and updates their corresponding indices.
 * 
 * @param h A pointer to the heap structure.
 * @param v The index of the first element to be swapped.
 * @param w The index of the second element to be swapped.
 * @note Assumes that the provided indices are within the bounds of the heap array.
 */
void swap(heap_s *h, int v, int w) {
  vertex_s tmp_v = h->array[v];
  h->array[v] = h->array[w];
  h->inds[h->array[v].ind] = v;
  h->array[w] = tmp_v;
  h->inds[h->array[w].ind] = w;
}

/**
 * @brief Creates a new heap structure.
 * 
 * Allocates memory for a heap structure capable of storing a specified number of elements.
 * 
 * @param nb_inds The maximum number of elements the heap can hold.
 * @return A pointer to the newly created heap structure.
 * @note Asserts that memory allocation for the heap and its internal arrays is successful.
 */
heap_s *heap_create(int nb_inds) {
  heap_s *res=malloc(sizeof(heap_s));
  assert(res!=NULL);
  res->max_elements=nb_inds;
  res->nb_elements=0;
  res->array = malloc(sizeof(vertex_s)*res->max_elements);
  assert(res->array!=NULL);
  res->inds = malloc(sizeof(int)*res->max_elements);
  assert(res->inds!=NULL);
  for(int i=0;i<res->max_elements;i++)
    res->inds[i]=-1;
  return res;
}

bool check_heap(heap_s *h) {
  if(h->nb_elements>=h->max_elements) return false;
  for(int i=0;i<h->nb_elements;i++)
    if(h->inds[h->array[i].ind]!=i) return false;
  return true;
}
/**
 * @brief Adds a new vertex to the heap.
 * 
 * Inserts a vertex into the heap while maintaining the heap property.
 * 
 * @param vertex The vertex to be added to the heap.
 * @param heap A pointer to the heap structure.
 * @return A pointer to the updated heap structure.
 * @note 
 * - Asserts that the heap is not NULL.
 * - Asserts that the heap does not exceed its maximum capacity after insertion.
 */
heap_s *heap_add(vertex_s vertex, heap_s *heap) {
  assert(heap!=NULL); // if the vertex does not exist, add it
  assert(check_heap(heap));
  if (heap->inds[vertex.ind]==-1) {
    int i=heap->nb_elements;
    heap->nb_elements++;
    assert(heap->nb_elements<=heap->max_elements);
    heap->array[i]=vertex;
    heap->inds[vertex.ind]=i;
    while(i>0 && heap->array[i].weight < heap->array[(i-1)/2].weight) {
      assert(heap->inds[heap->array[i].ind]==i);
      swap(heap,i,(i-1)/2); // restore the heap property
      i=(i-1)/2;            // go forward to the parent
    } 
  } else {
    int i=heap->inds[vertex.ind];
    if(vertex.weight < heap->array[i].weight)
      heap->array[i]=vertex;
  } 
  assert(check_heap(heap));
  return heap;
}

/** 
 * @brief Tests if the heap is empty.
 * @param heap The address of the current heap.
 * @return true if the heap is empty, false otherwise.
 * @note Asserts that the heap is created.
 */
bool heap_empty(heap_s *heap) {
  assert(heap!=NULL);
  return heap->nb_elements==0;
}

/** 
 * @brief Reads the head element without removing it.
 * @param heap The address of the current heap.
 * @return The vertex of the head element.
 * @note Asserts that the heap is not empty.
 */
vertex_s heap_peek(heap_s *heap) {
  assert(!heap_empty(heap));
  return heap->array[0];
}

/**
 * @brief Removes the root element from the heap.
 * 
 * Removes the element with the highest priority (root) from the heap and restores the heap property.
 * 
 * @param heap A pointer to the heap structure.
 * @return A pointer to the updated heap structure.
 * @note 
 * - Asserts that the heap is not NULL.
 * - Asserts that the heap is not empty before attempting to remove the root element.
 */
heap_s *heap_remove(heap_s *heap) {
  assert(heap!=NULL);
  assert(check_heap(heap));
  heap->array[0]=heap->array[heap->nb_elements-1];
  heap->inds[heap->array[0].ind]=0;
  heap->nb_elements--;
  int i=0; // index of the actual tree node
  while (i<heap->nb_elements) {
    int left_index = i*2+1;
    int right_index = i*2+2;
    int largest_index = i;
    if(left_index<heap->nb_elements && heap->array[left_index].weight < heap->array[largest_index].weight) 
      largest_index = left_index;
    if(right_index<heap->nb_elements && heap->array[right_index].weight < heap->array[largest_index].weight) 
      largest_index = right_index;
    if(largest_index == i) 
      break;
    swap(heap,i,largest_index); // restore the heap property
    i=largest_index; // go forward to the child
  }
  assert(check_heap(heap));
  return heap;
}

/** 
 * @brief Prints the heap elements from the head to the last element.
 * @param heap The address of the current heap.
 * @note Asserts that the heap is already created.
 */
void heap_print(heap_s *heap) {
  assert(heap!=NULL);
  printf("┌──────────%s",(heap->nb_elements-1>0)?"┬":"┐");
  for(int i=1;i<heap->nb_elements;i++)
    printf("──────────%s",(i<heap->nb_elements-1)?"┬":"┐");
  printf("\n"); 
  printf("│");
  for(int i=0;i<heap->nb_elements;i++) 
    printf("% 2d,% 3.1f,% 2d|",heap->array[i].ind,heap->array[i].weight,heap->array[i].prev);
  printf("\n");
  printf("└──────────%s",(heap->nb_elements-1>0)?"┴":"┘");
  for(int i=1;i<heap->nb_elements;i++)
    printf("──────────%s",(i<heap->nb_elements-1)?"┴":"┘");
}

/** 
 * @brief Erases the heap.
 * @param heap The address of the current heap.
 * @note Asserts that the heap is already created.
 */
void heap_delete(heap_s *heap) {
  assert(heap!=NULL);
  free(heap->array);
  free(heap->inds);
  free(heap);
}

