#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stddef.h>

// flushes stdin to avoid ghost newlines breaking the next prompt
void clear_input_buffer(void);

// safer wrapper around fgets to prevent buffer overflows
bool read_string(char *buffer, size_t size);

// simple digit check
bool is_numeric(const char *str);

// rudimentary email validation, we just verify an '@' is present here
bool is_valid_email(const char *email);

#endif /* UTILS_H */
