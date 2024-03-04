#include <stdio.h>
#include <interpreter.h>
#include <buffer.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fputs("Source file name is not specfied\n", stderr);
        return 1;
    }
    Interpreter *interpreter = interpreter_new(argv[1]);
    if (interpreter == NULL) {
        fputs("Could not load the source file\n", stderr);
        return 2;
    }
    StepResult step_result;
    do {
        step_result = interpreter_step(interpreter);
    } while (step_result == STEP_SUCCESS);
    return 0;
}
