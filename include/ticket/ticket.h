#ifndef TICKET_H
#define TICKET_H

typedef struct {
    long radicado;
    int identificacion;
    char correo[100];
    char tipo[100];
} Ticket;

#endif