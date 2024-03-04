#ifndef INCL_BUFFER_H
#define INCL_BUFFER_H

#include <stdbool.h>
#include <stdio.h>

/**
 * char 型可変長配列であるバッファ。
 */
typedef struct buffer Buffer;

/**
 * バッファを作成する。
 * 作成されたバッファは最終的に buffer_delete によって解放されなければいけない。
 * @param [in] initial_cap バッファの初期容量
 * @return 新たに作成されたバッファへのポインタ、または作成に失敗した場合ヌルポインタ
 */
Buffer *buffer_new(size_t initial_cap);

size_t buffer_size(Buffer *buf);

int buffer_get(Buffer *buf, size_t index);

bool buffer_set(Buffer *buf, size_t index, int value);

bool buffer_push(Buffer *buf, int value);

bool buffer_load_file(Buffer *restrict buf, FILE *restrict file);

bool buffer_load_file_from_name(Buffer *restrict buf, const char *restrict filename);

void buffer_delete(Buffer *buf);

#endif
