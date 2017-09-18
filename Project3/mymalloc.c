#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "mymalloc.h"

struct Node {
  int req_size;
  int in_use;
  struct Node *next_node;
  struct Node *prev_node;
};

struct Node *first;
struct Node *last;
struct Node *current;
struct Node *search;

// Prototypes
void *insert_fit(int);
void *add_heap(int);
void *best_fit(int);



void *my_bestfit_malloc(int size){
  // first check for valid size request
  if(size == 0){
    printf("no allocation made. Invalid request");
  }

// if first is null then no allocation made.
// initialize by creating first allocation node
// return a poiter to address of node's data segment
  if(first == NULL){
    first = sbrk(size + sizeof(struct Node));
    first->in_use = 1;
    first->req_size = size;
    first->next_node = NULL;
    first->prev_node = NULL;
    last = first;
    return ((void *)first + sizeof(struct Node));
  }

// if not first allocation, call best_fit() and locate the, you guessed it, best_fit
// if current if NULL then no fit was located, and we must create add to the heap
// if a PERFECT fit was found, return a pointer to the located node's data segemnt
// if not a Perfect fit, call function to insert into list and return pointer
  current = best_fit(size);
  if(current == NULL){
    return(add_heap(size)); 
  } else if (current->req_size == size){
    return ((void *)current + sizeof(struct Node));
  } else {
    return insert_fit(size);
  }
}

// best_fit algorithm
// traverse our linked list in its current state and locate the best_fit for our requested space
// if not fit is found, then current returns as NULL
// if a fit is found current points to that location we return current. 
void *best_fit(int size){
  search = first->next_node;

  while(search != NULL){
    if(search->in_use == 0){
      if(search->req_size == size){
	search->in_use = 1;
	current = search; 
	return (char*)current;
      } else if (search->req_size > size && current == NULL){
	current = search;
      } else if (search->req_size > size && current->req_size > search->req_size){
	current = search;
      }
    }
    search = search->next_node;
  } 
  
  return (char*)current;
}


// if no fit was found then we must add to the end of the heap
// create a new node that points to the end of the heap and extend the heap
// to hold the requested size and the size of our header
// set the enw node to last and return a pointer to the node's data segment
void *add_heap(int size){
  struct Node *add_node = sbrk(size + sizeof(struct Node));

  add_node->req_size = size;
  add_node->in_use = 1;
  add_node->prev_node = last;
  add_node->next_node = NULL;
  last->next_node = add_node;
  last = add_node;

  return ((void *)last + sizeof(struct Node));
}

// if a best_fit was found, we must insert the allocation into the list and create a 
// new node out of the remaining adjacent free space
// point the new node to the end of the requested spaces data segment, and set it to free
// then change the previous and next node pointers to include the new smaller free node
// update the size of the new node to the difference in requested size and best_fit size
// return a pointer the best fit nodes new data segement
void *insert_fit(int size){
  
  struct Node *insert_node = current + (size + sizeof(struct Node));
		
  insert_node->in_use = 0;
  insert_node->req_size = current->req_size - size;
  insert_node->next_node = current->next_node;
  insert_node->prev_node = current;
  current->next_node->prev_node = insert_node;
  current->next_node = insert_node;
  current->req_size = size;
  current->in_use = 1;
  
  return ((void *)current + sizeof(struct Node));
}



// my_free function frees specified portion of memory and coalesces adjacent free nodes
void my_free(void *ptr) {
      
  // make sure prt passed is valid
  if (ptr == NULL){
    return;
  }
  //create a node that points to ptr, minus the size of the header
  struct Node *free_node = (struct Node*)(ptr - sizeof(struct Node));
  // set the node to NOT in use 
  free_node->in_use = 0;
  
  // if the adjacent previous node is free, we must coalesce
  // if free's next node is null, we are freeing end of list
  if(free_node->prev_node != NULL){
    if(free_node->prev_node->in_use == 0){
      // coallesce size
      free_node->prev_node->req_size = (free_node->req_size + sizeof(struct Node));
      // coalesce nodes
      free_node->prev_node->next_node = free_node->next_node;
      if(free_node->next_node != NULL) {
	free_node->next_node->prev_node = free_node->prev_node;
      } else {
	last = free_node->prev_node;
      }
    }
  }
  // do the same thing if the adjacent next node is free
  // if free's next is NULL then we are freeing the end of the list
  if(free_node->next_node != NULL){
    if(free_node->next_node->in_use == 0){
      // coallesce size
      free_node->req_size += (free_node->next_node->req_size + sizeof(struct Node));
      // coallesce nodes
      free_node->next_node = free_node->next_node->next_node;
      if(free_node->next_node != NULL){
	free_node->next_node->prev_node = free_node;
      } else {
	last = free_node;
      }
    }
  }

  // if last has been set to FREE, then we must truncate the heap
  // if it is the last and first node, then the list is empty and we set all nodes to NULL
  // if list is not empty, then we update last
  // after either case, brk is set to the end of the new last node
  if(free_node->next_node == NULL){
    if(free_node->prev_node == NULL){
      brk((void *)free_node);
      first = NULL;
      current = NULL;
      last = NULL;
    } else {
      last = last->prev_node;
      last->next_node = NULL;
      brk((void *)free_node);
    }

  }
  
}




