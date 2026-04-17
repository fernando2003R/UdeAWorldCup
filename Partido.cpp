#define _CRT_SECURE_NO_WARNINGS
#include "Partido.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <iomanip>
using namespace std;

const double MU = 1.35;
const double ALPHA = 0.6;
const double BETA = 0.4;

Partido::Partido() : equipo1(nullptr), equipo2(nullptr), ganador(nullptr),
goles1(0), goles2(0), posesion1(50), posesion2(50), huboProrroga(false) {
}

Partido::Partido(Equipo* e1, Equipo* e2, const Date& f)
    : equipo1(e1), equipo2(e2), fecha(f), ganador(nullptr),
    goles1(0), goles2(0), posesion1(50), posesion2(50), huboProrroga(false) {
}

Partido::~Partido() {}

double Partido::calcularGolesEsperados(Equipo* A, Equipo* B) {
    double gfA = (double)A->getGolesAFavor() / 10.0;
    double gcB = (double)B->getGolesEnContra() / 10.0;

    if (gfA < 0.5) gfA = 0.8;
    if (gcB < 0.5) gcB = 0.8;

    double lambda = MU * pow(gfA / MU, ALPHA) * pow(gcB / MU, BETA);

    if (lambda < 0.5) lambda = 0.5;
    if (lambda > 3.5) lambda = 3.5;

    return lambda;
}

int Partido::obtenerGolesPoisson(double lambda) {
    if (lambda <= 0) return 0;

    double r = (double)rand() / RAND_MAX;

    if (lambda < 1.0) {
        if (r < 0.5) return 0;
        if (r < 0.8) return 1;
        if (r < 0.95) return 2;
        return 3;
    }
    else if (lambda < 2.0) {
        if (r < 0.3) return 0;
        if (r < 0.6) return 1;
        if (r < 0.8) return 2;
        if (r < 0.95) return 3;
        return 4;
    }
    else {
        if (r < 0.2) return 0;
        if (r < 0.45) return 1;
        if (r < 0.7) return 2;
        if (r < 0.85) return 3;
        if (r < 0.95) return 4;
        return 5;
    }
}

void Partido::calcularPosesion() {
    int r1 = equipo1->getRanking();
    int r2 = equipo2->getRanking();
    int total = r1 + r2;
    posesion1 = (100 * r2) / total;
    posesion2 = 100 - posesion1;
    if (posesion1 < 35) posesion1 = 35;
    if (posesion2 < 35) posesion2 = 35;
}

void Partido::simularEventos() {
    double lambda1 = calcularGolesEsperados(equipo1, equipo2);
    double lambda2 = calcularGolesEsperados(equipo2, equipo1);

    goles1 = obtenerGolesPoisson(lambda1);
    goles2 = obtenerGolesPoisson(lambda2);

    ArrayList<Jugador*>& jugadores1 = equipo1->getJugadores();
    ArrayList<Jugador*>& jugadores2 = equipo2->getJugadores();

    int golesRestantes1 = goles1;
    int golesRestantes2 = goles2;

    for (int i = 0; i < jugadores1.size() && golesRestantes1 > 0; i++) {
        if ((rand() % 100) < 4) {
            jugadores1.get(i)->agregarGoles(1);
            golesRestantes1--;
        }
    }

    for (int i = 0; i < jugadores2.size() && golesRestantes2 > 0; i++) {
        if ((rand() % 100) < 4) {
            jugadores2.get(i)->agregarGoles(1);
            golesRestantes2--;
        }
    }

    for (int i = 0; i < jugadores1.size(); i++) {
        int amarillasActual = jugadores1.get(i)->getTarjetasAmarillas();
        if (amarillasActual == 0 && (rand() % 100) < 6) {
            jugadores1.get(i)->agregarTarjetaAmarilla();
        }
        else if (amarillasActual == 1 && (rand() % 100) < 1) {
            jugadores1.get(i)->agregarTarjetaAmarilla();
            jugadores1.get(i)->agregarTarjetaRoja();
        }
    }

    for (int i = 0; i < jugadores2.size(); i++) {
        int amarillasActual = jugadores2.get(i)->getTarjetasAmarillas();
        if (amarillasActual == 0 && (rand() % 100) < 6) {
            jugadores2.get(i)->agregarTarjetaAmarilla();
        }
        else if (amarillasActual == 1 && (rand() % 100) < 1) {
            jugadores2.get(i)->agregarTarjetaAmarilla();
            jugadores2.get(i)->agregarTarjetaRoja();
        }
    }

    for (int i = 0; i < jugadores1.size(); i++) {
        int faltasActual = jugadores1.get(i)->getFaltas();
        if (faltasActual == 0 && (rand() % 100) < 13) {
            jugadores1.get(i)->agregarFalta();
        }
        else if (faltasActual == 1 && (rand() % 100) < 2) {
            jugadores1.get(i)->agregarFalta();
        }
    }

    for (int i = 0; i < jugadores2.size(); i++) {
        int faltasActual = jugadores2.get(i)->getFaltas();
        if (faltasActual == 0 && (rand() % 100) < 13) {
            jugadores2.get(i)->agregarFalta();
        }
        else if (faltasActual == 1 && (rand() % 100) < 2) {
            jugadores2.get(i)->agregarFalta();
        }
    }
}

void Partido::simular(bool esEliminatoria) {
    calcularPosesion();
    simularEventos();

    equipo1->agregarGolesAFavor(goles1);
    equipo1->agregarGolesEnContra(goles2);
    equipo2->agregarGolesAFavor(goles2);
    equipo2->agregarGolesEnContra(goles1);

    if (goles1 > goles2) {
        ganador = equipo1;
        equipo1->agregarVictoria();
        equipo2->agregarDerrota();
    }
    else if (goles2 > goles1) {
        ganador = equipo2;
        equipo2->agregarVictoria();
        equipo1->agregarDerrota();
    }
    else {
        if (esEliminatoria) {
            huboProrroga = true;
            if (equipo1->getRanking() < equipo2->getRanking()) {
                goles1++;
                ganador = equipo1;
                equipo1->agregarVictoria();
                equipo2->agregarDerrota();
            }
            else {
                goles2++;
                ganador = equipo2;
                equipo2->agregarVictoria();
                equipo1->agregarDerrota();
            }
            ArrayList<Jugador*>& jugadores1 = equipo1->getJugadores();
            ArrayList<Jugador*>& jugadores2 = equipo2->getJugadores();
            for (int i = 0; i < jugadores1.size(); i++) {
                jugadores1.get(i)->agregarMinutos(30);
            }
            for (int i = 0; i < jugadores2.size(); i++) {
                jugadores2.get(i)->agregarMinutos(30);
            }
        }
        else {
            equipo1->agregarEmpate();
            equipo2->agregarEmpate();
            ganador = nullptr;
        }
    }

    ArrayList<Jugador*>& jugadores1 = equipo1->getJugadores();
    ArrayList<Jugador*>& jugadores2 = equipo2->getJugadores();
    for (int i = 0; i < jugadores1.size(); i++) {
        jugadores1.get(i)->agregarMinutos(90);
        jugadores1.get(i)->agregarPartido();
    }
    for (int i = 0; i < jugadores2.size(); i++) {
        jugadores2.get(i)->agregarMinutos(90);
        jugadores2.get(i)->agregarPartido();
    }
}

void Partido::imprimirResultado() const {
    cout << "+---------------------------------------------------+" << endl;
    cout << "|                   MARCADOR                       |" << endl;
    cout << "+---------------------------------------------------+" << endl;
    cout << "|     " << goles1 << "  -  " << goles2 << "          "
        << equipo1->getPais().c_str() << "  vs  " << equipo2->getPais().c_str() << endl;
    cout << "+---------------------------------------------------+" << endl;
    cout << "|  Posesion: " << posesion1 << "%  -  " << posesion2 << "%";

    if (ganador) {
        cout << "     |  Ganador: " << ganador->getPais().c_str();
    }
    else {
        cout << "        |  EMPATE";
    }
    cout << endl;

    if (huboProrroga) {
        cout << "|  (Prórroga - 120 minutos)                    |" << endl;
    }
    cout << "+---------------------------------------------------+" << endl;
    cout << endl;
}

Equipo* Partido::getGanador() const {
    return ganador;
}