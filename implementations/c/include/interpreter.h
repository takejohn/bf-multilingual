#ifndef INCL_INTERPRETER_H
#define INCL_INTERPRETER_H

typedef struct interpreter Interpreter;

Interpreter *interpreter_new(const char *filename);

int interpreter_step(Interpreter *interpreter);

void interpreter_delete(Interpreter *interpreter);

#endif
