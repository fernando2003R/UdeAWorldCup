#ifndef EQUIPO_H
#define EQUIPO_H

#include "String.h"
#include "Jugador.h"
#include "Confederacion.h"
#include "ArrayList.h"

class Equipo {
private:
    String pais;
    Confederacion confederacion;
    String directorTecnico;
    int rankingFIFA;
    int golesAFavor;
    int golesEnContra;
    int puntos;
    ArrayList<Jugador*> jugadores;

public:
    Equipo();
    Equipo(const char* p, Confederacion conf, const char* dt, int ranking);
    ~Equipo();

    String getPais() const;
    Confederacion getConfederacion() const;
    int getRanking() const;
    int getGolesAFavor() const;
    int getGolesEnContra() const;
    int getPuntos() const;
    int getDiferenciaGoles() const;

    void agregarGolesAFavor(int g);
    void agregarGolesEnContra(int g);
    void agregarVictoria();
    void agregarEmpate();
    void agregarDerrota();
    void reiniciarPuntos();

    ArrayList<Jugador*>& getJugadores();
    void agregarJugador(Jugador* j);
    void distribuirGolesIniciales(int golesEquipo);

    // FUNCION AMIGA (requisito del documento)
    friend void mostrarDatosEquipo(const Equipo& e);

    // SOBRECARGA DE OPERADOR (requisito del documento)
    friend std::ostream& operator<<(std::ostream& os, const Equipo& e);
};

#endif