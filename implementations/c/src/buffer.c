#include <stdlib.h>
#include <buffer.h>

#define EXTEND_MIN_CAP 16

struct buffer {
    unsigned char *pointer;
    size_t cap;
    size_t size;
};

Buffer *buffer_new(size_t initial_cap) {
    unsigned char *const pointer = (unsigned char *)malloc(initial_cap);
    if (pointer == NULL) {
        return NULL;
    }
    Buffer *buf = (Buffer *)malloc(sizeof (Buffer));
    if (buf == NULL) {
        free(pointer);
        return NULL;
    }
    buf->pointer = pointer;
    buf->cap = initial_cap;
    buf->size = 0;
    return buf;
}

size_t buffer_size(Buffer *buf) {
    return buf->size;
}

int buffer_get(Buffer *buf, size_t index) {
    if (index > buf->size) {
        return -1;
    }
    return buf->pointer[index];
}

static size_t extend_cap(Buffer *buf) {
    const size_t old_cap = buf->cap;
    if (old_cap < EXTEND_MIN_CAP) {
        return EXTEND_MIN_CAP * 2;
    } else {
        return old_cap * 2;
    }
}

static bool buffer_extend(Buffer *buf) {
    const size_t new_cap = extend_cap(buf);
    unsigned char *const old_pointer = buf->pointer;
    unsigned char *const new_pointer = realloc(old_pointer, new_cap);
    if (new_pointer == NULL) {
        return false;
    }
    buf->pointer = new_pointer;
    buf->cap = new_cap;
    return true;
}

bool buffer_load_file(Buffer *restrict buf, FILE *restrict stream) {
    do {
        const size_t size = buf->size;
        const size_t reading_size = buf->cap - size;
        if (reading_size < EXTEND_MIN_CAP) {
            const bool extend_success = buffer_extend(buf);
            if (!extend_success) {
                return false;
            }
            continue;
        }
        const size_t result = fread(buf->pointer + size, 1, reading_size, stream);
        if (result < reading_size) {
            if (ferror(stream)) {
                clearerr(stream);
                return false;
            }
            if (feof(stream)) {
                buf->size += result;
                return true;
            }
        }
        buf->size += result;
    } while (true);
}

bool buffer_load_file_from_name(Buffer *restrict buf, const char *restrict filename) {
    FILE *const stream = fopen(filename, "r");
    if (stream == NULL) {
        return false;
    }
    const bool load_result = buffer_load_file(buf, stream);
    fclose(stream);
    return load_result;
}

void buffer_delete(Buffer *buf) {
    if (buf == NULL) {
        return;
    }
    free(buf->pointer);
    free(buf);
}
