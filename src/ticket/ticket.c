#include "../../include/ticket/ticket.h"
#include "../../include/utils/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#if defined(_WIN32)
#include <direct.h>
#define MKDIR(dir) _mkdir(dir)
#else
#define MKDIR(dir) mkdir(dir, 0777)
#endif

Ticket *create_ticket(void) {
  // hand out a brand new ticket on the heap
  Ticket *new_ticket = (Ticket *)malloc(sizeof(Ticket));
  if (new_ticket == NULL) {
    fprintf(stderr, "Critical: malloc failed to allocate a ticket.\n");
    return NULL;
  }

  // zero everything out so we don't end up with garbage in the arrays
  memset(new_ticket->identificacion, 0, sizeof(new_ticket->identificacion));
  memset(new_ticket->correo, 0, sizeof(new_ticket->correo));
  memset(new_ticket->tipo_reclamacion, 0, sizeof(new_ticket->tipo_reclamacion));
  new_ticket->radicado = 0;

  return new_ticket;
}

int fill_ticket_data(Ticket *ticket) {
  if (ticket == NULL) {
    return 0;
  }

  printf("Ingrese su identificación (numérica): ");
  if (!read_string(ticket->identificacion, sizeof(ticket->identificacion)) ||
      !is_numeric(ticket->identificacion) ||
      strlen(ticket->identificacion) == 0) {
    fprintf(stderr, "Error: La identificación es inválida, está vacía o no es "
                    "puramente numérica.\n");
    return 0;
  }

  printf("Ingrese su correo electrónico: ");
  if (!read_string(ticket->correo, sizeof(ticket->correo)) ||
      !is_valid_email(ticket->correo) || strlen(ticket->correo) == 0) {
    fprintf(stderr, "Error: Correo inválido. Es obligatorio que contenga el "
                    "carácter '@'.\n");
    return 0;
  }

  printf("Ingrese el tipo de reclamación: ");
  if (!read_string(ticket->tipo_reclamacion,
                   sizeof(ticket->tipo_reclamacion)) ||
      strlen(ticket->tipo_reclamacion) == 0) {
    fprintf(stderr,
            "Error: El tipo de reclamación no puede ser una cadena vacía.\n");
    return 0;
  }

  // combining unix timestamp with a slight random offset to keep ids fairly
  // unique. Using time_t and unsigned long avoids the Y2038 overflow bug on
  // 32-bit builds.
  srand((unsigned int)time(NULL));
  time_t timestamp = time(NULL);
  unsigned long random_offset = (unsigned long)rand() % 10000;

  ticket->radicado = (unsigned long)timestamp + random_offset;

  printf("\nTicket registrado correctamente.\n");
  printf("Radicado generado: %lu\n", ticket->radicado);

  return 1;
}

int save_ticket(const Ticket *ticket) {
  if (ticket == NULL) {
    fprintf(stderr,
            "Error: Intento de guardar con referencia nula del Ticket.\n");
    return 0;
  }

  // forcefully create the assets folder, ignore errors if it already exists
  MKDIR("assets");

  // setup formatting for the filename
  char filepath[256];
  struct tm *tm_info;
  time_t rawtime;

  time(&rawtime);
  tm_info = localtime(&rawtime);
  if (tm_info == NULL) {
    fprintf(stderr,
            "Error: No se ha podido obtener la hora local del sistema.\n");
    return 0;
  }

  char time_str[20];
  strftime(time_str, sizeof(time_str), "%Y%m%d%H%M", tm_info);

  // generate an easy to read path based on the current minute
  snprintf(filepath, sizeof(filepath), "assets/ticket_%s.txt", time_str);

  // attempt to grab a write lock on the new file
  FILE *file = fopen(filepath, "w");
  if (file == NULL) {
    fprintf(stderr, "Error: No se pudo abrir el archivo '%s' para escritura.\n",
            filepath);
    return 0;
  }

  // dump struct payload into plain text file
  fprintf(file, "Radicado: %lu\n", ticket->radicado);
  fprintf(file, "Identificación: %s\n", ticket->identificacion);
  fprintf(file, "Correo: %s\n", ticket->correo);
  fprintf(file, "Tipo de reclamación: %s\n", ticket->tipo_reclamacion);

  fclose(file);
  return 1;
}

void free_ticket(Ticket *ticket) {
  if (ticket != NULL) {
    // safely return the ticket pointer to the OS
    free(ticket);
  }
}
