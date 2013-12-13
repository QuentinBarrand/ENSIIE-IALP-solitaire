/** 
 * \file Stack.c 
 * \brief Module offering a stack structure for data manipulation.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */

#include <stdlib.h>
#include <strings.h>

#include "Stack.h"

/** The default size of a Stack when the parameter is not specified. */
#define DEFAULT_SIZE 5 


/** Creates a new Stack.
 *
 * \param size the default size of the Stack.
 *
 * \return a pointer to the new Stack.
 */
Stack* Stack_New(int size)
{
    if (size < DEFAULT_SIZE)
        size = DEFAULT_SIZE;

    Stack* stack = malloc(sizeof(Stack));
    stack->container = malloc(sizeof(void**) * size);

    stack->count = 0;
    stack->max = size;

    return stack;
}

/** Frees the memory space taken by the Stack.
 *
 * \param stack pointer to the Stack that should be freed.
 */
void Stack_Free(Stack* stack)
{
    free(stack->container);
    free(stack);
}


/** Returns the current number of elements of a Stack.
 *
 * \param pointer to the Stack of which we want to get the number of elements.
 *
 * \return the current number of elements into the specified Stack.
 */
int Stack_Size(Stack* stack)
{
    return stack->count;
}


/** Returns the capacity of a Stack.
 *
 * \param stack the Stack we want to know the capacity of.
 *
 * \return the capacity of the specified Stack.
 */
int Stack_Max(Stack* stack)
{
    return stack->max;
}


/** Useful to know if the Stack is empty.
 *
 * \param stack the Stack of which we want to now the status.
 *
 * \return an integer:
 *    - 0 if the Stack is empty;
 *    - 1 if not.
 */
int Stack_IsEmpty(Stack* stack)
{
    if(stack->count == 0)
        return 1;

    return 0;
}


/** Doubles the capacity of the Stack.
 *
 * \param stack the Stack of which we want to double the capacity.
 *
 * \return a pointer to the Stack's container with the new capacity.
 */
void** Stack_ResizeContainer(Stack* stack)
{
    void** tmpContainer = malloc(sizeof(void**) * stack->max * 2);
    bcopy(stack->container, tmpContainer, sizeof(void**)*stack->max);
    stack->max *= 2;

    return tmpContainer;
}


/** Searchs for an element in the Stack.
 *
 * \param value a pointer to the element that is looked for.
 * \param stack the Stack in which the search will be performed.
 *
 * \return the index of the element in the Stack.
 */
int Stack_Search(void* value, Stack* stack)
{
    int i;
    for(i = stack->count - 1; i >= 0; i--)
    {
        if(stack->container[i] == value)
            return i;
    }

    return -1;
}


/** Returns the top element of the Stack (last in).
 *
 * \param stack the Stack of which we want the top element.
 *
 * \return a pointer to the element a the top of the Stack.
 */
void* Stack_Peek(Stack* stack)
{
    if (stack->count > 0)
        return stack->container[stack->count - 1];

    return NULL;
}


/** Pushes an element on the top of the Stack.
 *
 * \param item pointer to the element to be pushed.
 * \param stack the Stack in which the element should be pushed.
 */
void Stack_Push(void* item, Stack* stack)
{
    if (stack->count == stack->max)
        stack->container = Stack_ResizeContainer(stack);
 
    stack->container[stack->count] = item;  
    stack->count++;
}


/** Removes and returns the top element of the Stack (last in).
 *
 * \param stack the Stack of which the top element will be removed.
 *
 * \return a pointer to the element that was removed.
 */
void* Stack_Pop(Stack* stack)
{
    if (stack->count > 0)
    {
        stack->count--;
        return stack->container[stack->count];
    }

    return NULL;
}