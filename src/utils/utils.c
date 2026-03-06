#include "../../include/utils/utils.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void clear_input_buffer(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) {
    // throw away the rest of the line
  }
}

bool read_string(char *buffer, size_t size) {
  if (fgets(buffer, size, stdin) == NULL) {
    return false;
  }

  size_t len = strlen(buffer);
  if (len > 0 && buffer[len - 1] == '\n') {
    // chop off the trailing newline
    buffer[len - 1] = '\0';
  } else {
    // the user typed more than the buffer sizes, so flush the rest to avoid
    // weird bugs
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

  // rudimentary check just making sure there's an '@' - could be much stricter
  return strchr(email, '@') != NULL;
}
