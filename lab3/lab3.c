#include <stdlib.h>
#include <stdio.h>


struct Node {
  int grade;
  struct Node *next;
};

struct Node *head = NULL;      // global node to be reffered to in both main and listAvg methods


int listAvg() {
  struct Node *ptr = head;     // create a struct pointer to point to glocal Node var 'head'
  struct Node *freePtr;        // free ptr is a local var that temp points to head, for the purpose of
  int grade_sum = 0;           // freeing malloc'd space
  int num_of_grades = 0;     

  while(ptr != NULL) {         // while ptr is ptr to a node, grab data and add to sum
    num_of_grades++;           // increment bum of values by 1 for later averaging
    grade_sum += ptr->grade;   
    freePtr = ptr;             // assign address pointed to by ptr to freePtr
    ptr = ptr->next;           // assign pointer value of ptr's next field and assign to itself
    free(freePtr);             // freeing freePtr, frees the malloc'd space that ptr previosuly pointed to
  }
  printf("The avg of the %d integers is %d\n", num_of_grades, grade_sum/num_of_grades);   // print the average of all entered values. 

}


int main(int argc, char *argv[]){
  int user_in = 0;
  
  while(1){
    printf("Enter an integer: ");    
    scanf("%d", &user_in);
    if (user_in == -1){         //check for exit value '-1' and if found, break loop
      break;
    }
    struct Node *new_node = malloc(sizeof(struct Node)); // allocate space for new node
    new_node->grade = user_in;                           // set new node grade to uder input
    new_node->next = head;                               // set new node to point to head 
    head = new_node;                                     // set new_node ot head (itself if first)
  }
  listAvg();                    // function avgs values and frees allocated space in reverse order
  
  return 0;
}
