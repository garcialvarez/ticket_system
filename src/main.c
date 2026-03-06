#include "../include/ticket/ticket.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  printf("=== Sistema de Registro de Tickets ===\n");

  // allocating on the heap so we can safely drop it later
  Ticket *my_ticket = create_ticket();
  if (my_ticket == NULL) {
    // malloc failed, bail out before segfaulting
    return EXIT_FAILURE;
  }

  // prompt the user and persist to disk
  if (fill_ticket_data(my_ticket)) {
    if (!save_ticket(my_ticket)) {
      fprintf(stderr, "Failed to persist ticket to disk.\n");
    }
  } else {
    fprintf(stderr, "Aborting ticket creation due to bad input.\n");
  }

  // free up the memory, no leaks allowed
  free_ticket(my_ticket);

  return EXIT_SUCCESS;
}