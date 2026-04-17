#define _CRT_SECURE_NO_WARNINGS
#include "Equipo.h"
#include <iostream>
using namespace std;

Equipo::Equipo() : rankingFIFA(100), golesAFavor(0), golesEnContra(0), puntos(0) {}

Equipo::Equipo(const char* p, Confederacion conf, const char* dt, int ranking)
    : pais(p), confederacion(conf), directorTecnico(dt), rankingFIFA(ranking),
    golesAFavor(0), golesEnContra(0), puntos(0) {
}

Equipo::~Equipo() {}

String Equipo::getPais() const { return pais; }
Confederacion Equipo::getConfederacion() const { return confederacion; }
int Equipo::getRanking() const { return rankingFIFA; }
int Equipo::getGolesAFavor() const { return golesAFavor; }
int Equipo::getGolesEnContra() const { return golesEnContra; }
int Equipo::getPuntos() const { return puntos; }
int Equipo::getDiferenciaGoles() const { return golesAFavor - golesEnContra; }

void Equipo::agregarGolesAFavor(int g) { golesAFavor += g; }
void Equipo::agregarGolesEnContra(int g) { golesEnContra += g; }
void Equipo::agregarVictoria() { puntos += 3; }
void Equipo::agregarEmpate() { puntos += 1; }
void Equipo::agregarDerrota() {}
void Equipo::reiniciarPuntos() { puntos = 0; }

ArrayList<Jugador*>& Equipo::getJugadores() {
    return jugadores;
}

void Equipo::agregarJugador(Jugador* j) {
    jugadores.add(j);
}

void Equipo::distribuirGolesIniciales(int golesEquipo) {
    int numJugadores = jugadores.size();
    if (numJugadores == 0) return;

    int golesPorJugador = golesEquipo / numJugadores;
    int resto = golesEquipo % numJugadores;

    for (int i = 0; i < numJugadores; i++) {
        int g = golesPorJugador;
        if (i < resto) g++;
        jugadores.get(i)->setGoles(g);
    }
}

// FUNCION AMIGA: Muestra los datos de un equipo
void mostrarDatosEquipo(const Equipo& e) {
    cout << "=== DATOS DEL EQUIPO ===" << endl;
    cout << "Pais: " << e.pais.c_str() << endl;
    cout << "Director Tecnico: " << e.directorTecnico.c_str() << endl;
    cout << "Ranking FIFA: " << e.rankingFIFA << endl;
    cout << "Goles a favor: " << e.golesAFavor << endl;
    cout << "Goles en contra: " << e.golesEnContra << endl;
    cout << "Puntos: " << e.puntos << endl;
}

// SOBRECARGA DEL OPERADOR <<
ostream& operator<<(ostream& os, const Equipo& e) {
    os << e.pais.c_str() << " (Ranking: " << e.rankingFIFA << ", Pts: " << e.puntos << ")";
    return os;
}