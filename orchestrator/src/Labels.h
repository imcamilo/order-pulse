#ifndef LABELS_H
#define LABELS_H

// All user-visible Spanish strings on the orchestrator panel. Centralized
// so changing the language or copy is a single-file edit.
//
// Note: MQTT protocol strings ("pending"/"preparing"/"ready") live in
// TouchLogic::STATUS_LABELS — those are wire-format values, not labels.
namespace Labels
{
    // Status labels shown inside the status buttons / overlay
    inline const char *STATUS_PENDING   = "PENDIENTE";
    inline const char *STATUS_PREPARING = "PREPARANDO";
    inline const char *STATUS_READY     = "LISTO";

    // Fixed UI text
    inline const char *PANEL_TITLE      = "ORDER PULSE - MESAS";
    inline const char *BACK_BUTTON      = "< VOLVER";
    inline const char *SENT_OVERLAY     = "ENVIADO";

    // printf-style format string
    inline const char *FMT_TABLE_DETAIL = "MESA %d";
}

#endif
