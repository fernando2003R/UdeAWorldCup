#ifndef JUGADOR_H
#define JUGADOR_H

#include "String.h"

class Jugador {
private:
    String nombre;
    String apellido;
    int numeroCamiseta;
    int partidosJugados;
    int goles;
    int minutosJugados;
    int asistencias;
    int tarjetasAmarillas;
    int tarjetasRojas;
    int faltas;

public:
    Jugador();
    Jugador(const char* nom, const char* ap, int num);
    ~Jugador();

    // Getters
    int getNumeroCamiseta() const;
    int getGoles() const;
    int getTarjetasAmarillas() const;
    int getTarjetasRojas() const;
    int getFaltas() const;
    int getMinutosJugados() const;
    int getPartidosJugados() const;

    // Setters
    void setGoles(int g);
    void agregarGoles(int g);
    void agregarTarjetaAmarilla();
    void agregarTarjetaRoja();
    void agregarFalta();
    void agregarMinutos(int minutos);
    void agregarPartido();
};

#endif