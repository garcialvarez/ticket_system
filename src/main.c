#include "../include/ticket/ticket.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  printf("=== Sistema de Registro de Tickets ===\n");

  /* Puntero a estructura Ticket, se inicia pidiendo memoria dinámica */
  Ticket *my_ticket = create_ticket();
  if (my_ticket == NULL) {
    /* Falla en reservar memoria con malloc, error controlado */
    return EXIT_FAILURE;
  }

  /* Flujo principal del programa */
  if (fill_ticket_data(my_ticket)) {
    if (!save_ticket(my_ticket)) {
      fprintf(stderr, "Ocurrió un error general persistiendo la información "
                      "del ticket en el sistema de archivos.\n");
    }
  } else {
    fprintf(stderr, "El proceso de captura de ticket falló debido a entradas "
                    "con errores lógicos.\n");
  }

  /* Libera la memoria reservada para proteger que no haya memory leaks */
  free_ticket(my_ticket);

  return EXIT_SUCCESS;
}