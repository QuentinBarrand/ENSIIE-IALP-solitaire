/**
 * \file Stack.h
 * \brief Header pour le fichier Stack.c.
 *
 * \author Quentin Barrand <quentin.barrand@ensiie.fr>
 */

#ifndef STACK_H_FILE
#define STACK_H_FILE
 
typedef struct Stack {
    int count;
    int max;
    void** container;
} Stack;

/* Fonctions du module. */
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