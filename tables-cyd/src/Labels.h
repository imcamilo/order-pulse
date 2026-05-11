#ifndef LABELS_H
#define LABELS_H

// All user-visible Spanish strings on the table-cyd display. Centralized
// so changing the language or copy is a single-file edit.
//
// Note: MQTT protocol strings ("pending"/"preparing"/"ready") are NOT here.
// Those are wire-format values, not labels.
namespace Labels
{
    // Status labels shown inside the status box
    inline const char *STATUS_PENDING   = "PENDIENTE";
    inline const char *STATUS_PREPARING = "PREPARANDO";
    inline const char *STATUS_READY     = "LISTO";
    inline const char *STATUS_UNKNOWN   = "DESCONOCIDO";

    // Idle state when no order has arrived
    inline const char *WAITING_ORDER    = "ESPERANDO PEDIDO";

    // printf-style format strings (one substitution each)
    inline const char *FMT_TABLE        = "MESA %d";
    inline const char *FMT_ORDER        = "Orden #%d";
    inline const char *FMT_ETA          = "Tiempo estimado: %s";
}

#endif
