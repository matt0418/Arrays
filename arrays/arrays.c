#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Array {
  int capacity;  // How many elements can this array hold?
  int count;  // How many states does the array currently hold?
  char **elements;  // The string elements contained in the array
} Array;


/************************************
 *
 *   CREATE, DESTROY, RESIZE FUNCTIONS
 *
 ************************************/

/*****
 * Allocate memory for a new array
 *****/
Array *create_array (int capacity) {
  // Allocate memory for the Array struct
  Array *arr = malloc(sizeof(Array));

  // Set initial values for capacity and count
  arr->capacity = capacity;
  arr->count = 0;

  // Allocate memory for elements
  arr->elements = malloc(capacity * sizeof(char *));

  return arr;

}


/*****
 * Free memory for an array and all of its stored elements
 *****/
void destroy_array(Array *arr) {

  // Free all elements
  for (int i = 0; i < arr->count; i++) {
    free(arr->elements[i]);
  }

  // Free array
  free(arr->elements);
  free(arr);

}

/*****
 * Create a new elements array with double capacity and copy elements
 * from old to new
 *****/
void resize_array(Array *arr) {

  // Create a new element storage with double capacity
  char **new_elements= malloc(2 * arr->capacity * sizeof(char*));

  // Copy elements into the new storage

  for (int i = 0; i < arr->capacity; i++) {
    new_elements[i] = arr->elements[i];
  }


  // Free the old elements array (but NOT the strings they point to)

  free(arr->elements);

  // Update the elements and capacity to new values
  arr->elements = new_elements;
  arr->capacity = arr->capacity * 2;

}



/************************************
 *
 *   ARRAY FUNCTIONS
 *
 ************************************/

/*****
 * Return the element in the array at the given index.
 *
 * Throw an error if the index is out of range.
 *****/
char *arr_read(Array *arr, int index) {

  // Throw an error if the index is greater or equal to than the current count
  if (arr->count <= index) {
    perror("Index is out of range");
    return NULL;
  }

  // Otherwise, return the element at the given index
  return arr->elements[index];
}


/*****
 * Insert an element to the array at the given index
 *
 * Store the VALUE of the given string, not the REFERENCE
 *****/
void arr_insert(Array *arr, char *element, int index) {

  // Throw an error if the index is greater than the current count
  if (arr->count < index) {
    perror("Index is out of range");
  } else {
    // Resize the array if the number of elements is over capacity

    if (arr->count == arr->capacity) {
      resize_array(arr);
    }

    // Move every element after the insert index to the right one position
    for (int i = arr->count; i > index; i--) {
      arr->elements[i] = arr->elements[i-1]; 
    }

    // Copy the element (hint: use `strdup()`) and add it to the array
    char *copied_elem = strdup(element);
    arr->elements[index] = copied_elem;

    // Increment count by 1
    arr->count++;

    }

  
}

/*****
 * Append an element to the end of the array
 *****/
void arr_append(Array *arr, char *element) {

  // Resize the array if the number of elements is over capacity
  if (arr->count == arr->capacity) {
    resize_array(arr);
  }

  // Copy the element and add it to the end of the array
  char *copied_elem = strdup(element);

  arr->elements[arr->count] = copied_elem;

  // Increment count by 1
  arr->count++;

}

/*****
 * Remove the first occurence of the given element from the array,
 * then shift every element after that occurence to the left one slot.
 *
 * Throw an error if the value is not found.
 *****/
void arr_remove(Array *arr, char *element) {

  // Search for the first occurence of the element and remove it.
  int did_i_remove = 0;
  for (int i = 0; i < arr->count; i++) {
    if (did_i_remove) {
      // Shift over every element after the removed element to the left one position
      arr->elements[i-1] = arr->elements[i];
    } else if (strcmp(element, arr->elements[i]) == 0) {
      // Don't forget to free its memory!
      free(arr->elements[i]);
      did_i_remove = 1;
    }
  }

  if (did_i_remove) {
    arr->count--;
  } else {
    fprintf(stderr, "Element not in the array");
  }
}


/*****
 * Utility function to print an array.
 *****/
void arr_print(Array *arr) {
  printf("[");
  for (int i = 0 ; i < arr->count ; i++) {
    printf("%s", arr->elements[i]);
    if (i != arr->count - 1) {
      printf(",");
    }
  }
  printf("]\n");
}


#ifndef TESTING
int main(void)
{

  Array *arr = create_array(1);

  arr_insert(arr, "STRING1", 0);
  arr_append(arr, "STRING4");
  arr_insert(arr, "STRING2", 0);
  arr_insert(arr, "STRING3", 1);
  arr_print(arr);
  arr_remove(arr, "STRING3");
  arr_print(arr);

  destroy_array(arr);

  return 0;
}
#endif
