#include <stdlib.h>
#include <stdio.h>

#include <interpreter.h>
#include <buffer.h>

struct interpreter {
    const char *filename;
    Buffer *program;
    size_t pc;
};

Interpreter *interpreter_new(const char *filename) {
    Buffer *const program = buffer_new(0);
    const bool load_result = buffer_load_file_from_name(program, filename);
    if (!load_result) {
        return NULL;
    }
    Interpreter *const interpreter = (Interpreter *)malloc(sizeof (Interpreter));
    if (interpreter == NULL) {
        return NULL;
    }
    interpreter->filename = filename;
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
