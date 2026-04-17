#define _CRT_SECURE_NO_WARNINGS
#include "Grupo.h"
#include <iostream>
#include <cstdio>
using namespace std;

Grupo::Grupo(char l) : letra(l) {}

Grupo::~Grupo() {
    for (int i = 0; i < partidos.size(); i++) {
        delete partidos.get(i);
    }
}

void Grupo::agregarEquipo(Equipo* e) {
    equipos.add(e);
}

void Grupo::configurarPartidos(const Date& fechaInicio) {
    // Limpiar partidos anteriores si los hay
    for (int i = 0; i < partidos.size(); i++) {
        delete partidos.get(i);
    }
    partidos = ArrayList<Partido*>();

    // Los 4 equipos del grupo: indices 0,1,2,3
    // 6 partidos: (0-1), (0-2), (0-3), (1-2), (1-3), (2-3)
    int partidosEquipos[6][2] = {
        {0, 1}, {0, 2}, {0, 3},
        {1, 2}, {1, 3}, {2, 3}
    };

    // Registrar la última fecha en que jugó cada equipo (días desde fechaInicio)
    int ultimaFechaEquipo[4] = { -1, -1, -1, -1 };
    int partidosHoy = 0;
    int diaActual = 0;

    for (int p = 0; p < 6; p++) {
        int eqA = partidosEquipos[p][0];
        int eqB = partidosEquipos[p][1];

        // Calcular el día mínimo permitido (por descanso de 3 días)
        int minDia = diaActual;
        if (ultimaFechaEquipo[eqA] != -1) {
            int minA = ultimaFechaEquipo[eqA] + 3;
            if (minA > minDia) minDia = minA;
        }
        if (ultimaFechaEquipo[eqB] != -1) {
            int minB = ultimaFechaEquipo[eqB] + 3;
            if (minB > minDia) minDia = minB;
        }

        // Si el día actual es menor que el mínimo, avanzar
        if (diaActual < minDia) {
            diaActual = minDia;
            partidosHoy = 0;
        }

        // Si ya hay 4 partidos hoy, pasar al siguiente día
        if (partidosHoy >= 4) {
            diaActual++;
            partidosHoy = 0;
            // Verificar nuevamente las restricciones de descanso
            if (diaActual < minDia) {
                diaActual = minDia;
            }
        }

        // Crear el partido con la fecha asignada
        Date fechaPartido = fechaInicio + diaActual;
        Partido* pObj = new Partido(equipos.get(eqA), equipos.get(eqB), fechaPartido);
        partidos.add(pObj);

        // Actualizar registros
        ultimaFechaEquipo[eqA] = diaActual;
        ultimaFechaEquipo[eqB] = diaActual;
        partidosHoy++;
    }
}

void Grupo::simularPartidos() {
    for (int i = 0; i < partidos.size(); i++) {
        partidos.get(i)->simular(false);
    }
}

void Grupo::ordenarEquiposPorPuntos() {
    for (int i = 0; i < equipos.size() - 1; i++) {
        for (int j = 0; j < equipos.size() - i - 1; j++) {
            if (equipos.get(j)->getPuntos() < equipos.get(j + 1)->getPuntos()) {
                Equipo* temp = equipos.get(j);
                equipos.set(j, equipos.get(j + 1));
                equipos.set(j + 1, temp);
            }
        }
    }
}

void Grupo::imprimirPartidos() const {
    cout << "\n========================================\n";
    cout << "              GRUPO " << letra << "\n";
    cout << "========================================\n";
    for (int i = 0; i < partidos.size(); i++) {
        partidos.get(i)->imprimirResultado();
    }
}

void Grupo::imprimirTabla() const {
    cout << "\n=== Grupo " << letra << " ===\n";
    cout << "Equipo               | Pts | GF | GC | DG\n";
    cout << "----------------------------------------\n";
    for (int i = 0; i < equipos.size(); i++) {
        printf("%-20s | %3d | %2d | %2d | %3d\n",
            equipos.get(i)->getPais().c_str(),
            equipos.get(i)->getPuntos(),
            equipos.get(i)->getGolesAFavor(),
            equipos.get(i)->getGolesEnContra(),
            equipos.get(i)->getDiferenciaGoles());
    }
}

ArrayList<Equipo*>& Grupo::getEquipos() {
    return equipos;
}

char Grupo::getLetra() const {
    return letra;
}

Equipo* Grupo::getPrimero() {
    if (equipos.size() > 0) return equipos.get(0);
    return nullptr;
}

Equipo* Grupo::getSegundo() {
    if (equipos.size() > 1) return equipos.get(1);
    return nullptr;
}

Equipo* Grupo::getTercero() {
    if (equipos.size() > 2) return equipos.get(2);
    return nullptr;
}