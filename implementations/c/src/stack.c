#include <assert.h>
#include <stdlib.h>
#include <stack.h>

#define INITIAL_CAP 8

struct stack {
    StackItem *pointer;
    size_t cap;
    size_t size;
};

Stack *stack_new(void) {
    StackItem *const pointer = (StackItem *)malloc(sizeof(StackItem) * INITIAL_CAP);
    if (pointer == NULL) {
        return NULL;
    }
    Stack *stack = (Stack *)malloc(sizeof (Stack));
    if (stack == NULL) {
        free(pointer);
        return NULL;
    }
    stack->pointer = pointer;
    stack->cap = INITIAL_CAP;
    stack->size = 0;
    return stack;
}

size_t stack_size(Stack *stack) {
    return stack->size;
}

bool stack_push(Stack *stack, StackItem value) {
    const size_t cap = stack->cap;
    const size_t size = stack->size;
    assert(size <= cap);
    if (size == cap) {
        const size_t new_cap = cap * 2;
        StackItem *new_pointer = (StackItem *)malloc(sizeof(StackItem) * new_cap);
        if (new_pointer == NULL) {
            return false;
        }
        stack->pointer = new_pointer;
        stack->cap = new_cap;
        new_pointer[size] = value;
    } else {
        stack->pointer[size] = value;
    }
    stack->size = size + 1;
    return true;
}

StackItem stack_peek(Stack *stack) {
    const size_t size = stack->size;
    if (size == 0) {
        return STACK_NULL_ITEM;
    }
    return stack->pointer[size - 1];
}

StackItem stack_pop(Stack *stack) {
    const size_t size = stack->size;
    if (size == 0) {
        return STACK_NULL_ITEM;
    }
    const size_t new_size = size - 1;
    const StackItem result = stack->pointer[new_size];
    stack->size = new_size;
    return result;
}

void stack_delete(Stack *stack) {
    free(stack->pointer);
    free(stack);
}
