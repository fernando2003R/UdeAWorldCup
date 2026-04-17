#ifndef CONFEDERACION_H
#define CONFEDERACION_H

enum Confederacion {
    UEFA,
    CONMEBOL,
    CONCACAF,
    CAF,
    AFC,
    OFC
};

inline const char* confederacionToString(Confederacion c) {
    switch (c) {
    case UEFA: return "UEFA";
    case CONMEBOL: return "CONMEBOL";
    case CONCACAF: return "CONCACAF";
    case CAF: return "CAF";
    case AFC: return "AFC";
    case OFC: return "OFC";
    default: return "DESCONOCIDO";
    }
}

#endif