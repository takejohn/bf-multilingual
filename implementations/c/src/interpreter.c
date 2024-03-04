#include <stdlib.h>
#include <stdio.h>

#include <interpreter.h>
#include <buffer.h>

struct interpreter {
    Buffer *program;
};

Interpreter *interpreter_new(char *filename) {
    Buffer *const program = buffer_new(0);
    FILE *const stream = fopen(filename, "r");
    if (stream == NULL) {
        return NULL;
    }
    const bool load_result = buffer_load_file(program, stream);
    fclose(stream);
    if (!load_result) {
        return NULL;
    }
    Interpreter *const interpreter = (Interpreter *)malloc(sizeof (Interpreter));
    if (interpreter == NULL) {
        return NULL;
    }
    interpreter->program = program;
    return interpreter;
}

void interpreter_delete(Interpreter *interpreter) {
    if (interpreter == NULL) {
        return;
    }
    buffer_delete(interpreter->program);
    free(interpreter);
}
