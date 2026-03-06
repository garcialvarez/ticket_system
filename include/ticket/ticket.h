#ifndef TICKET_H
#define TICKET_H

typedef struct {
  char identificacion[50];
  char correo[100];
  char tipo_reclamacion[100];
  unsigned long radicado;
} Ticket;

// allocate ticket on the heap
Ticket *create_ticket(void);

// populates the ticket in-place via user prompts
int fill_ticket_data(Ticket *ticket);

// dumps ticket struct to a file in assets dir
int save_ticket(const Ticket *ticket);

// clean up heap allocation
void free_ticket(Ticket *ticket);

#endif /* TICKET_H */