#include <stdlib.h>
#include <stdio.h>

#include <interpreter.h>
#include <buffer.h>

struct interpreter {
    const char *filename;
    Buffer *program;
    size_t pc;
    Buffer *memory;
    size_t ptr;
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
            Interpreter *const interpreter = (Interpreter *)malloc(sizeof (Interpreter));
            if (interpreter != NULL) {
                buffer_push(memory, 0);
                interpreter->filename = filename;
                interpreter->program = program;
                interpreter->pc = 0;
                interpreter->memory = memory;
                interpreter->ptr = 0;
                return interpreter;
            }
            buffer_delete(memory);
        }
    }
    buffer_delete(program);
    return NULL;
}

StepResult interpreter_step(Interpreter *interpreter) {
    const int inst = buffer_get(interpreter->program, interpreter->pc);
    if (inst < 0) {
        return STEP_END;
    }
    Buffer *const memory = interpreter->memory;
    const size_t ptr = interpreter->ptr;
    switch (inst) {
        case '+': {
            const int value = buffer_get(memory, ptr);
            buffer_set(memory, ptr, value + 1);
            interpreter->pc++;
            return STEP_SUCCESS;
        }
        case '-': {
            const int value = buffer_get(memory, ptr);
            buffer_set(memory, ptr, value - 1);
            interpreter->pc++;
            return STEP_SUCCESS;
        }
        case '.': {
            putchar(buffer_get(memory, ptr));
            interpreter->pc++;
            return STEP_SUCCESS;
        }
        case ',': {
            buffer_set(memory, ptr, getchar());
            interpreter->pc++;
            return STEP_SUCCESS;
        }
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
    free(interpreter);
}
