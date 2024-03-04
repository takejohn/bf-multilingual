#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <interpreter.h>
#include <buffer.h>
#include <stack.h>

struct interpreter {
    const char *filename;
    Buffer *program;
    size_t pc;
    Buffer *memory;
    size_t ptr;
    Stack *stack;
};

Interpreter *interpreter_new(const char *filename) {
    Buffer *const program = buffer_new(0);
    if (program == NULL) {
        return NULL;
    }
    const bool load_result = buffer_load_file_from_name(program, filename);
    if (load_result) {
        Buffer *const memory = buffer_new(1);
        if (memory != NULL) {
            Stack *const stack = stack_new();
            if (stack != NULL) {
                Interpreter *const interpreter = (Interpreter *)malloc(sizeof (Interpreter));
                if (interpreter != NULL) {
                    buffer_push(memory, 0);
                    interpreter->filename = filename;
                    interpreter->program = program;
                    interpreter->pc = 0;
                    interpreter->memory = memory;
                    interpreter->ptr = 0;
                    interpreter->stack = stack;
                    return interpreter;
                }
                stack_delete(stack);
            }
            buffer_delete(memory);
        }
    }
    buffer_delete(program);
    return NULL;
}

static StepResult step_plus(Interpreter *interpreter) {
    Buffer *const memory = interpreter->memory;
    const size_t ptr = interpreter->ptr;
    const int value = buffer_get(memory, ptr);
    buffer_set(memory, ptr, value + 1);
    interpreter->pc++;
    return STEP_SUCCESS;
}

static StepResult step_minus(Interpreter *interpreter) {
    Buffer *const memory = interpreter->memory;
    const size_t ptr = interpreter->ptr;
    const int value = buffer_get(memory, ptr);
    buffer_set(memory, ptr, value - 1);
    interpreter->pc++;
    return STEP_SUCCESS;
}

static StepResult step_write(Interpreter *interpreter) {
    Buffer *const memory = interpreter->memory;
    const size_t ptr = interpreter->ptr;
    putchar(buffer_get(memory, ptr));
    interpreter->pc++;
    return STEP_SUCCESS;
}

static StepResult step_read(Interpreter *interpreter) {
    Buffer *const memory = interpreter->memory;
    const size_t ptr = interpreter->ptr;
    buffer_set(memory, ptr, getchar());
    interpreter->pc++;
    return STEP_SUCCESS;
}

static StepResult step_right(Interpreter *interpreter) {
    Buffer *const memory = interpreter->memory;
    const size_t ptr = interpreter->ptr + 1;
    const size_t memory_size = buffer_size(memory);
    assert(ptr <= memory_size);
    if (ptr == memory_size) {
        const bool success = buffer_push(memory, 0);
        if (!success) {
            return STEP_OUT_OF_MEMORY;
        }
    }
    interpreter->ptr = ptr;
    interpreter->pc++;
    return STEP_SUCCESS;
}

static StepResult step_left(Interpreter *interpreter) {
    const size_t ptr = interpreter->ptr - 1;
    if (ptr < 0) {
        return STEP_OUT_OF_BOUND;
    }
    interpreter->ptr = ptr;
    interpreter->pc++;
    return STEP_SUCCESS;
}

static StepResult step_while(Interpreter *interpreter) {
    if (buffer_get(interpreter->memory, interpreter->ptr) != 0) {
        stack_push(interpreter->stack, interpreter->pc);
        interpreter->pc++;
        return STEP_SUCCESS;
    }
    unsigned int depth = 0; 
    Buffer *const program = interpreter->program;
    const size_t program_size = buffer_size(program);
    size_t i = interpreter->pc;
    while (i < program_size) {
        const unsigned char inst = buffer_get(program, i);
        if (inst == '[') {
            depth++;
        } else if (inst == ']') {
            depth--;
            if (depth == 0) {
                interpreter->pc = i + 1;
                return STEP_SUCCESS;
            }
        }
        i++;
    }
    return STEP_CLOSING_BRACKET_EXPECTED;
}

static StepResult step_endwhile(Interpreter *interpreter) {
    Stack *const stack = interpreter->stack;
    if (stack_size(stack) == 0) {
        return STEP_UNEXPECTED_CLOSING_BRACKET;
    }
    if (buffer_get(interpreter->memory, interpreter->ptr) == 0) {
        stack_pop(stack);
        interpreter->pc++;
        return STEP_SUCCESS;
    }
    size_t begin_pc = stack_peek(interpreter->stack);
    interpreter->pc = begin_pc + 1;
    return STEP_SUCCESS;
}

StepResult interpreter_step(Interpreter *interpreter) {
    const int inst = buffer_get(interpreter->program, interpreter->pc);
    if (inst < 0) {
        return STEP_END;
    }
    switch (inst) {
        case '+': 
            return step_plus(interpreter);
        case '-': 
            return step_minus(interpreter);
        case '.': 
            return step_write(interpreter);
        case ',': 
            return step_read(interpreter);
        case '>':
            return step_right(interpreter);
        case '<':
            return step_left(interpreter);
        case '[':
            return step_while(interpreter);
        case ']':
            return step_endwhile(interpreter);
        default: {
            interpreter->pc++;
            return STEP_SUCCESS;
        }
    }
}

void interpreter_delete(Interpreter *interpreter) {
    if (interpreter == NULL) {
        return;
    }
    buffer_delete(interpreter->program);
    buffer_delete(interpreter->memory);
    stack_delete(interpreter->stack);
    free(interpreter);
}
