#include "../../include/utils/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void clear_input_buffer(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
    /* Descartar caracteres hasta nueva línea o EOF */
  }
}

bool read_string(char *buffer, size_t size) {
  if (fgets(buffer, size, stdin) == NULL) {
    return false;
  }

  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') {
    /* Reemplazar salto de línea por caracter nulo */
    buffer[len - 1] = '\0';
  } else {
    /* Si no se leyó un salto de línea, la entrada era más grande que el buffer.
       Limpiamos el buffer de entrada. */
    clear_input_buffer();
  }

  return true;
}

bool is_numeric(const char *str) {
  if (str == NULL || *str == '\0') {
    return false;
  }

  for (size_t i = 0; str[i] != '\0'; i++) {
    if (!isdigit((unsigned char)str[i])) {
      return false;
    }
  }
  return true;
}

bool is_valid_email(const char *email) {
  if (email == NULL || *email == '\0') {
    return false;
  }

  /* Válida con presencia del símbolo @ como pidió el requerimiento */
  return strchr(email, '@') != NULL;
}
