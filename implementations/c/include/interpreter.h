#ifndef INCL_INTERPRETER_H
#define INCL_INTERPRETER_H

typedef struct interpreter Interpreter;

Interpreter *interpreter_new(char *filename);

void interpreter_delete(Interpreter *interpreter);

#endif
