#ifndef TORNEO_H
#define TORNEO_H

#include "Equipo.h"
#include "Grupo.h"
#include "Partido.h"
#include "ArrayList.h"
#include "Date.h"

class Torneo {
private:
    ArrayList<Equipo*> equipos;
    ArrayList<Grupo*> grupos;
    ArrayList<Partido*> partidosEliminatorios;

    long long totalIteraciones;
    size_t memoriaActual;

    ArrayList<Equipo*> bombos[4];

    void contarIteracion() { totalIteraciones++; }
    void actualizarMemoria();

public:
    Torneo();
    ~Torneo();

    void cargarEquiposDesdeCSV(const char* filename);
    void generarJugadoresArtificiales();
    void crearBombosYSortearGrupos();
    void imprimirGrupos() const;
    void simularFaseGrupos();
    void imprimirPartidosGrupos() const;
    void construirEliminatorias();
    void imprimirFixtureEliminatorias() const;
    void simularEliminatorias();
    void generarInformeFinal() const;
    void imprimirMetricas(const char* etapa) const;

    // Funciones de estadísticas
    void imprimirRankingFinal();
    void imprimirMaximoGoleadorCampeon();
    void imprimirTopGoleadores();
    void imprimirEquipoMasGolesHistoricos();
    void imprimirConfederacionesDominantes();
};

#endif