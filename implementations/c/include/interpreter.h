#ifndef INCL_INTERPRETER_H
#define INCL_INTERPRETER_H

typedef struct interpreter Interpreter;

typedef enum step_result {
    STEP_SUCCESS,
    STEP_END,
    STEP_ERROR
} StepResult;

Interpreter *interpreter_new(const char *filename);

StepResult interpreter_step(Interpreter *interpreter);

void interpreter_delete(Interpreter *interpreter);

#endif
