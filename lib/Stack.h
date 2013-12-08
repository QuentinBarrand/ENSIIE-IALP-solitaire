/**
 * \file Stack.h
 * \brief Header for the Stack.c file.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */

#ifndef STACK_H_FILE
#define STACK_H_FILE

/** The Stack data structure. */
typedef struct Stack {
    /** Number of elements actually present in the stack. */
    int count;

    /** Maximum size of the stack. */
    int max;

    /** Double array of pointers that will link to elements you push in the
     * stack.
     */
    void** container;
} Stack;

/* Module functions */
extern Stack* Stack_New(int size);
extern void   Stack_Free(Stack* stack);
extern int    Stack_Size(Stack* stack);
extern int    Stack_Max(Stack* stack);
extern int    Stack_IsEmpty(Stack* stack);
extern void** Stack_ResizeContainer(Stack* stack);
extern int    Stack_Search(void* value, Stack* stack);
extern void*  Stack_Peek(Stack* stack);
extern void   Stack_Push(void* item, Stack* stack);
extern void*  Stack_Pop(Stack* stack);

#endif