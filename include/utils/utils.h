#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdbool.h>

/* Limpia el buffer de entrada para evitar saltos de línea basura al leer */
void clear_input_buffer(void);

/* Lee una cadena de caracteres desde stdin de manera segura, sin buffer overflow */
bool read_string(char *buffer, size_t size);

/* Verifica si la cadena contiene únicamente dígitos */
bool is_numeric(const char *str);

/* Verifica de manera elemental si un correo es válido (contiene '@') */
bool is_valid_email(const char *email);

#endif /* UTILS_H */
