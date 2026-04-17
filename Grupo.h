#ifndef GRUPO_H
#define GRUPO_H

#include "Equipo.h"
#include "Partido.h"
#include "ArrayList.h"
#include "Date.h"

class Grupo {
private:
    char letra;
    ArrayList<Equipo*> equipos;
    ArrayList<Partido*> partidos;

public:
    Grupo(char l);
    ~Grupo();

    void agregarEquipo(Equipo* e);
    void configurarPartidos(const Date& fechaInicio);
    void simularPartidos();
    void ordenarEquiposPorPuntos();
    void imprimirPartidos() const;
    void imprimirTabla() const;

    ArrayList<Equipo*>& getEquipos();
    char getLetra() const;
    Equipo* getPrimero();
    Equipo* getSegundo();
    Equipo* getTercero();
};

#endif