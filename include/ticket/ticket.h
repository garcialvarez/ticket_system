#ifndef TICKET_H
#define TICKET_H

typedef struct {
  char identificacion[50];
  char correo[100];
  char tipo_reclamacion[100];
  long radicado;
} Ticket;

/* Función que reserva memoria dinámica para la estructura de Ticket */
Ticket *create_ticket(void);

/* Llena los datos del ticket pasado por referencia, solicitándolo al usuario */
int fill_ticket_data(Ticket *ticket);

/* Guarda el ticket en un archivo dentro de la carpeta assets/ */
int save_ticket(const Ticket *ticket);

/* Libera la memoria reservada del ticket */
void free_ticket(Ticket *ticket);

#endif /* TICKET_H */