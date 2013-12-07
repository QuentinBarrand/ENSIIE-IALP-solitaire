/** 
 * \file Stack.c 
 * \brief Module offering a stack structure for data manipulation.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */

#include <stdlib.h>
#include <strings.h>

#include "Stack.h"

#define DEFAULT_SIZE 5 

Stack* Stack_New(int size){
    if (size < DEFAULT_SIZE){
        size = DEFAULT_SIZE;
    }

    Stack* stack = malloc(sizeof(Stack));
    stack->container = malloc(sizeof(void**) * size);

    stack->count = 0;
    stack->max = size;

    return stack;
}
 
void Stack_Free(Stack* stack){
    free(stack->container);
    free(stack);
}


int Stack_Size(Stack* stack){
  return stack->count;
}


int Stack_Max(Stack* stack){
  return stack->max;
}


int Stack_IsEmpty(Stack* stack){
  if(stack->count == 0)
    return 1;

  return 0;
}


void** Stack_ResizeContainer(Stack* stack){
  void** tmpContainer = malloc(sizeof(void**) * stack->max * 2);
  bcopy(stack->container, tmpContainer, sizeof(void**)*stack->max);
  stack->max *= 2;

  return tmpContainer;
}


int Stack_Search(void* value, Stack* stack){
  int i;
  for(i = stack->count - 1; i >= 0; i--){
    if(stack->container[i] == value)
      return i;
  }
  return -1;
}


void* Stack_Peek(Stack* stack){
  if (stack->count > 0){
    return stack->container[stack->count - 1];
  }

  return NULL;
}


void Stack_Push(void* item, Stack* stack){
  if (stack->count == stack->max)
    stack->container = Stack_ResizeContainer(stack);
 
  stack->container[stack->count] = item;
  stack->count++;
}


void* Stack_Pop(Stack* stack){
  if (stack->count > 0){
    stack->count--;
    return stack->container[stack->count];
  }

  return NULL;
}