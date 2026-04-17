#ifndef PARTIDO_H
#define PARTIDO_H

#include "Equipo.h"
#include "Date.h"

class Partido {
private:
    Equipo* equipo1;
    Equipo* equipo2;
    Equipo* ganador;
    Date fecha;
    int goles1;
    int goles2;
    int posesion1;
    int posesion2;
    bool huboProrroga;

    double calcularGolesEsperados(Equipo* A, Equipo* B);
    int obtenerGolesPoisson(double lambda);
    void calcularPosesion();
    void simularEventos();

public:
    Partido();
    Partido(Equipo* e1, Equipo* e2, const Date& f);
    ~Partido();

    void simular(bool esEliminatoria = false);
    void imprimirResultado() const;
    Equipo* getGanador() const;
    Date getFecha() const { return fecha; }
};

#endif