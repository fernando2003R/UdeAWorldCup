#define _CRT_SECURE_NO_WARNINGS
#include "Jugador.h"

Jugador::Jugador() : numeroCamiseta(0), partidosJugados(0), goles(0),
minutosJugados(0), asistencias(0), tarjetasAmarillas(0),
tarjetasRojas(0), faltas(0) {
}

Jugador::Jugador(const char* nom, const char* ap, int num)
    : nombre(nom), apellido(ap), numeroCamiseta(num), partidosJugados(0),
    goles(0), minutosJugados(0), asistencias(0), tarjetasAmarillas(0),
    tarjetasRojas(0), faltas(0) {
}

Jugador::~Jugador() {}

int Jugador::getNumeroCamiseta() const { return numeroCamiseta; }
int Jugador::getGoles() const { return goles; }
int Jugador::getTarjetasAmarillas() const { return tarjetasAmarillas; }
int Jugador::getTarjetasRojas() const { return tarjetasRojas; }
int Jugador::getFaltas() const { return faltas; }
int Jugador::getMinutosJugados() const { return minutosJugados; }
int Jugador::getPartidosJugados() const { return partidosJugados; }

void Jugador::setGoles(int g) { goles = g; }
void Jugador::agregarGoles(int g) { goles += g; }
void Jugador::agregarTarjetaAmarilla() { tarjetasAmarillas++; }
void Jugador::agregarTarjetaRoja() { tarjetasRojas++; }
void Jugador::agregarFalta() { faltas++; }
void Jugador::agregarMinutos(int minutos) { minutosJugados += minutos; }
void Jugador::agregarPartido() { partidosJugados++; }