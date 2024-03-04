#ifndef INCL_STACK_H
#define INCL_STACK_H

#include <stdbool.h>
#include <stddef.h>

/**
 * size_t 型のスタック。
 */
typedef struct stack Stack;

typedef size_t StackItem;

#define STACK_NULL_ITEM ((size_t)-1)

Stack *stack_new(void);

size_t stack_size(Stack *stack);

bool stack_push(Stack *stack, StackItem value);

StackItem stack_peek(Stack *stack);

StackItem stack_pop(Stack *stack);

void stack_delete(Stack *stack);

#endif
