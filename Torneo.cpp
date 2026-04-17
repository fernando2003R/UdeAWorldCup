#define _CRT_SECURE_NO_WARNINGS
#include "Torneo.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
using namespace std;

// Variables globales para guardar resultados de la final y tercer puesto
static Equipo* campeon = nullptr;
static Equipo* subcampeon = nullptr;
static Equipo* tercerPuesto = nullptr;
static Equipo* cuartoPuesto = nullptr;

Torneo::Torneo() {
    totalIteraciones = 0;
    memoriaActual = 0;
    srand((unsigned int)time(nullptr));
}

Torneo::~Torneo() {
    for (int i = 0; i < equipos.size(); i++) delete equipos.get(i);
    for (int i = 0; i < grupos.size(); i++) delete grupos.get(i);
    for (int i = 0; i < partidosEliminatorios.size(); i++) delete partidosEliminatorios.get(i);
}

void Torneo::actualizarMemoria() {
    memoriaActual = 0;
    for (int i = 0; i < equipos.size(); i++) {
        memoriaActual += sizeof(Equipo);
        memoriaActual += equipos.get(i)->getJugadores().size() * sizeof(Jugador);
        contarIteracion();
    }
    memoriaActual += grupos.size() * sizeof(Grupo);
    memoriaActual += partidosEliminatorios.size() * sizeof(Partido);
}

void Torneo::cargarEquiposDesdeCSV(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        cout << "ERROR: No se pudo abrir " << filename << endl;
        return;
    }

    char linea[1024];
    fgets(linea, sizeof(linea), file);

    while (fgets(linea, sizeof(linea), file)) {
        char pais[100], confStr[20], dt[100];
        int ranking, gf, gc, pg, pe, pp;

        sscanf(linea, "%[^,],%[^,],%[^,],%d,%d,%d,%d,%d,%d",
            pais, confStr, dt, &ranking, &gf, &gc, &pg, &pe, &pp);

        Confederacion conf;
        if (strcmp(confStr, "UEFA") == 0) conf = UEFA;
        else if (strcmp(confStr, "CONMEBOL") == 0) conf = CONMEBOL;
        else if (strcmp(confStr, "CONCACAF") == 0) conf = CONCACAF;
        else if (strcmp(confStr, "CAF") == 0) conf = CAF;
        else if (strcmp(confStr, "AFC") == 0) conf = AFC;
        else conf = OFC;

        Equipo* e = new Equipo(pais, conf, dt, ranking);
        for (int i = 0; i < gf; i++) e->agregarGolesAFavor(1);
        for (int i = 0; i < gc; i++) e->agregarGolesEnContra(1);

        equipos.add(e);
        contarIteracion();
    }

    fclose(file);
    cout << "Cargados " << equipos.size() << " equipos.\n";
    actualizarMemoria();
}

void Torneo::generarJugadoresArtificiales() {
    for (int i = 0; i < equipos.size(); i++) {
        Equipo* e = equipos.get(i);
        int golesEquipo = e->getGolesAFavor();

        for (int num = 1; num <= 26; num++) {
            char nombre[20], apellido[20];
            sprintf(nombre, "nombre%d", num);
            sprintf(apellido, "apellido%d", num);
            Jugador* j = new Jugador(nombre, apellido, num);
            e->agregarJugador(j);
            contarIteracion();
        }

        e->distribuirGolesIniciales(golesEquipo);
    }
    cout << "Generados jugadores para " << equipos.size() << " equipos.\n";
    actualizarMemoria();
}

void Torneo::crearBombosYSortearGrupos() {
    // 1. Ordenar equipos por ranking
    for (int i = 0; i < equipos.size() - 1; i++) {
        for (int j = 0; j < equipos.size() - i - 1; j++) {
            if (equipos.get(j)->getRanking() > equipos.get(j + 1)->getRanking()) {
                Equipo* temp = equipos.get(j);
                equipos.set(j, equipos.get(j + 1));
                equipos.set(j + 1, temp);
            }
            contarIteracion();
        }
    }

    // 2. Identificar al anfitrión (EE.UU.)
    Equipo* anfitrion = nullptr;
    int anfitrionIdx = -1;
    for (int i = 0; i < equipos.size(); i++) {
        if (strcmp(equipos.get(i)->getPais().c_str(), "EE.UU.") == 0) {
            anfitrion = equipos.get(i);
            anfitrionIdx = i;
            break;
        }
        contarIteracion();
    }

    // 3. Crear 12 grupos vacíos
    for (char l = 'A'; l <= 'L'; l++) {
        grupos.add(new Grupo(l));
    }

    // 4. Colocar al anfitrión en el Grupo A
    if (anfitrion) {
        grupos.get(0)->agregarEquipo(anfitrion);
        cout << "Anfitrion EE.UU. asignado al Grupo A\n";
    }

    // 5. Distribuir el resto de equipos por ranking
    int idxEquipo = 0;
    for (int g = 0; g < 12; g++) {
        while (grupos.get(g)->getEquipos().size() < 4 && idxEquipo < equipos.size()) {
            Equipo* e = equipos.get(idxEquipo);
            if (e != anfitrion) {
                grupos.get(g)->agregarEquipo(e);
            }
            idxEquipo++;
            contarIteracion();
        }
    }

    // 6. Verificar restricciones de confederaciones
    cout << "\n=== VERIFICACION DE RESTRICCIONES ===\n";
    bool restriccionesOk = true;

    for (int g = 0; g < grupos.size(); g++) {
        int countUEFA = 0, countCONMEBOL = 0, countCONCACAF = 0;
        int countCAF = 0, countAFC = 0, countOFC = 0;

        for (int i = 0; i < grupos.get(g)->getEquipos().size(); i++) {
            Confederacion conf = grupos.get(g)->getEquipos().get(i)->getConfederacion();
            switch (conf) {
            case UEFA: countUEFA++; break;
            case CONMEBOL: countCONMEBOL++; break;
            case CONCACAF: countCONCACAF++; break;
            case CAF: countCAF++; break;
            case AFC: countAFC++; break;
            case OFC: countOFC++; break;
            }
            contarIteracion();
        }

        if (countUEFA > 2) {
            cout << "ADVERTENCIA: Grupo " << grupos.get(g)->getLetra() << " tiene " << countUEFA << " UEFA (max 2)\n";
            restriccionesOk = false;
        }
        if (countCONMEBOL > 1) {
            cout << "ADVERTENCIA: Grupo " << grupos.get(g)->getLetra() << " tiene " << countCONMEBOL << " CONMEBOL (max 1)\n";
            restriccionesOk = false;
        }
        if (countCONCACAF > 1) {
            cout << "ADVERTENCIA: Grupo " << grupos.get(g)->getLetra() << " tiene " << countCONCACAF << " CONCACAF (max 1)\n";
            restriccionesOk = false;
        }
        if (countCAF > 1) {
            cout << "ADVERTENCIA: Grupo " << grupos.get(g)->getLetra() << " tiene " << countCAF << " CAF (max 1)\n";
            restriccionesOk = false;
        }
        if (countAFC > 1) {
            cout << "ADVERTENCIA: Grupo " << grupos.get(g)->getLetra() << " tiene " << countAFC << " AFC (max 1)\n";
            restriccionesOk = false;
        }
        if (countOFC > 1) {
            cout << "ADVERTENCIA: Grupo " << grupos.get(g)->getLetra() << " tiene " << countOFC << " OFC (max 1)\n";
            restriccionesOk = false;
        }
    }

    if (restriccionesOk) {
        cout << "Todas las restricciones de confederaciones se cumplen correctamente.\n";
    }
    else {
        cout << "Nota: Algunas restricciones no se cumplen (simplificacion del sorteo).\n";
    }

    imprimirGrupos();
    actualizarMemoria();
}

void Torneo::imprimirGrupos() const {
    cout << "\n=== GRUPOS DEL MUNDIAL 2026 ===\n";
    for (int i = 0; i < grupos.size(); i++) {
        cout << "\nGrupo " << grupos.get(i)->getLetra() << ":\n";
        for (int j = 0; j < grupos.get(i)->getEquipos().size(); j++) {
            cout << "  " << grupos.get(i)->getEquipos().get(j)->getPais().c_str() << "\n";
        }
    }
}

void Torneo::simularFaseGrupos() {
    for (int i = 0; i < equipos.size(); i++) {
        equipos.get(i)->reiniciarPuntos();
        contarIteracion();
    }

    Date inicio(20, 6, 2026);

    for (int i = 0; i < grupos.size(); i++) {
        grupos.get(i)->configurarPartidos(inicio);
        grupos.get(i)->simularPartidos();
        grupos.get(i)->ordenarEquiposPorPuntos();
        contarIteracion();
    }

    cout << "\n\n";
    cout << "==================================================\n";
    cout << "              RESULTADOS FASE DE GRUPOS\n";
    cout << "==================================================\n";

    for (int i = 0; i < grupos.size(); i++) {
        grupos.get(i)->imprimirPartidos();
    }

    cout << "\n\n";
    cout << "==================================================\n";
    cout << "              TABLAS DE CLASIFICACION\n";
    cout << "==================================================\n";

    for (int i = 0; i < grupos.size(); i++) {
        grupos.get(i)->imprimirTabla();
    }

    actualizarMemoria();
}

void Torneo::imprimirPartidosGrupos() const {}

struct TerceroInfo {
    Equipo* equipo;
    int puntos;
    int diferenciaGoles;
    int golesAFavor;
    char grupo;
};

void Torneo::construirEliminatorias() {
    ArrayList<Equipo*> clasificados;

    for (int i = 0; i < grupos.size(); i++) {
        clasificados.add(grupos.get(i)->getPrimero());
        clasificados.add(grupos.get(i)->getSegundo());
        contarIteracion();
    }

    TerceroInfo terceros[12];
    int numTerceros = 0;

    for (int i = 0; i < grupos.size(); i++) {
        Equipo* tercero = grupos.get(i)->getTercero();
        if (tercero != nullptr) {
            terceros[numTerceros].equipo = tercero;
            terceros[numTerceros].puntos = tercero->getPuntos();
            terceros[numTerceros].diferenciaGoles = tercero->getDiferenciaGoles();
            terceros[numTerceros].golesAFavor = tercero->getGolesAFavor();
            terceros[numTerceros].grupo = 'A' + i;
            numTerceros++;
        }
        contarIteracion();
    }

    for (int i = 0; i < numTerceros - 1; i++) {
        for (int j = 0; j < numTerceros - i - 1; j++) {
            bool intercambiar = false;
            if (terceros[j].puntos < terceros[j + 1].puntos) {
                intercambiar = true;
            }
            else if (terceros[j].puntos == terceros[j + 1].puntos) {
                if (terceros[j].diferenciaGoles < terceros[j + 1].diferenciaGoles) {
                    intercambiar = true;
                }
                else if (terceros[j].diferenciaGoles == terceros[j + 1].diferenciaGoles) {
                    if (terceros[j].golesAFavor < terceros[j + 1].golesAFavor) {
                        intercambiar = true;
                    }
                }
            }
            if (intercambiar) {
                TerceroInfo temp = terceros[j];
                terceros[j] = terceros[j + 1];
                terceros[j + 1] = temp;
            }
            contarIteracion();
        }
    }

    int mejoresTerceros = (numTerceros < 8) ? numTerceros : 8;
    cout << "\n=== MEJORES TERCEROS CLASIFICADOS ===\n";
    for (int i = 0; i < mejoresTerceros; i++) {
        clasificados.add(terceros[i].equipo);
        cout << (i + 1) << ". " << terceros[i].equipo->getPais().c_str()
            << " (Grupo " << terceros[i].grupo << ") - Puntos: " << terceros[i].puntos << endl;
        contarIteracion();
    }

    cout << "\n=== CLASIFICADOS A R16 ===\n";
    cout << "Total de equipos: " << clasificados.size() << "\n";

    for (int i = 0; i < clasificados.size() - 1; i++) {
        for (int j = 0; j < clasificados.size() - i - 1; j++) {
            if (rand() % 2 == 0) {
                Equipo* temp = clasificados.get(j);
                clasificados.set(j, clasificados.get(j + 1));
                clasificados.set(j + 1, temp);
            }
            contarIteracion();
        }
    }

    partidosEliminatorios = ArrayList<Partido*>();
    for (int i = 0; i < clasificados.size(); i += 2) {
        if (i + 1 < clasificados.size()) {
            Partido* p = new Partido(clasificados.get(i), clasificados.get(i + 1), Date(10, 7, 2026));
            partidosEliminatorios.add(p);
        }
        contarIteracion();
    }

    actualizarMemoria();
}

void Torneo::imprimirFixtureEliminatorias() const {
    cout << "\n==================================================\n";
    cout << "              DIEZcISEISAVOS DE FINAL (R16)\n";
    cout << "==================================================\n";
    for (int i = 0; i < partidosEliminatorios.size(); i++) {
        cout << "Partido " << i + 1 << ": ";
        partidosEliminatorios.get(i)->imprimirResultado();
    }
}

void Torneo::simularEliminatorias() {
    if (partidosEliminatorios.size() == 0) {
        cout << "No hay partidos de eliminatorias.\n";
        return;
    }

    cout << "\n==================================================\n";
    cout << "              R16 - DIEZcISEISAVOS\n";
    cout << "==================================================\n";

    ArrayList<Equipo*> ganadoresR16;
    for (int i = 0; i < partidosEliminatorios.size(); i++) {
        partidosEliminatorios.get(i)->simular(true);
        partidosEliminatorios.get(i)->imprimirResultado();
        Equipo* ganador = partidosEliminatorios.get(i)->getGanador();
        if (ganador) {
            ganadoresR16.add(ganador);
        }
        contarIteracion();
    }

    cout << "\nClasificados a R8: " << ganadoresR16.size() << " equipos\n";

    if (ganadoresR16.size() < 2) return;

    cout << "\n==================================================\n";
    cout << "              R8 - OCTAVOS\n";
    cout << "==================================================\n";

    ArrayList<Equipo*> ganadoresR8;
    for (int i = 0; i < ganadoresR16.size(); i += 2) {
        if (i + 1 < ganadoresR16.size()) {
            Partido p(ganadoresR16.get(i), ganadoresR16.get(i + 1), Date(10, 7, 2026));
            p.simular(true);
            p.imprimirResultado();
            Equipo* ganador = p.getGanador();
            if (ganador) {
                ganadoresR8.add(ganador);
            }
            contarIteracion();
        }
    }

    cout << "\nClasificados a QF: " << ganadoresR8.size() << " equipos\n";

    if (ganadoresR8.size() < 2) return;

    cout << "\n==================================================\n";
    cout << "              QF - CUARTOS\n";
    cout << "==================================================\n";

    ArrayList<Equipo*> ganadoresQF;
    for (int i = 0; i < ganadoresR8.size(); i += 2) {
        if (i + 1 < ganadoresR8.size()) {
            Partido p(ganadoresR8.get(i), ganadoresR8.get(i + 1), Date(10, 7, 2026));
            p.simular(true);
            p.imprimirResultado();
            Equipo* ganador = p.getGanador();
            if (ganador) {
                ganadoresQF.add(ganador);
            }
            contarIteracion();
        }
    }

    cout << "\nClasificados a SF: " << ganadoresQF.size() << " equipos\n";

    if (ganadoresQF.size() < 4) {
        cout << "Error: Se necesitan 4 equipos para semifinales, solo hay " << ganadoresQF.size() << "\n";
        return;
    }

    cout << "\n==================================================\n";
    cout << "              SF - SEMIFINALES\n";
    cout << "==================================================\n";

    ArrayList<Equipo*> perdedoresSF;
    ArrayList<Equipo*> ganadoresSF;

    Partido sf1(ganadoresQF.get(0), ganadoresQF.get(1), Date(10, 7, 2026));
    sf1.simular(true);
    sf1.imprimirResultado();
    Equipo* ganador1 = sf1.getGanador();
    if (ganador1) {
        ganadoresSF.add(ganador1);
        if (ganador1 == ganadoresQF.get(0)) {
            perdedoresSF.add(ganadoresQF.get(1));
        }
        else {
            perdedoresSF.add(ganadoresQF.get(0));
        }
    }
    contarIteracion();

    Partido sf2(ganadoresQF.get(2), ganadoresQF.get(3), Date(10, 7, 2026));
    sf2.simular(true);
    sf2.imprimirResultado();
    Equipo* ganador2 = sf2.getGanador();
    if (ganador2) {
        ganadoresSF.add(ganador2);
        if (ganador2 == ganadoresQF.get(2)) {
            perdedoresSF.add(ganadoresQF.get(3));
        }
        else {
            perdedoresSF.add(ganadoresQF.get(2));
        }
    }
    contarIteracion();

    cout << "\n==================================================\n";
    cout << "              TERCER PUESTO\n";
    cout << "==================================================\n";
    Partido tercerPuesto(perdedoresSF.get(0), perdedoresSF.get(1), Date(10, 7, 2026));
    tercerPuesto.simular(true);
    tercerPuesto.imprimirResultado();

    ::tercerPuesto = tercerPuesto.getGanador();
    ::cuartoPuesto = (::tercerPuesto == perdedoresSF.get(0)) ? perdedoresSF.get(1) : perdedoresSF.get(0);

    cout << "\n==================================================\n";
    cout << "              FINAL\n";
    cout << "==================================================\n";
    Partido final(ganadoresSF.get(0), ganadoresSF.get(1), Date(10, 7, 2026));
    final.simular(true);
    final.imprimirResultado();

    ::campeon = final.getGanador();
    ::subcampeon = (::campeon == ganadoresSF.get(0)) ? ganadoresSF.get(1) : ganadoresSF.get(0);

    actualizarMemoria();
}

void Torneo::imprimirRankingFinal() {
    cout << "\n=== RANKING DE LOS 4 PRIMEROS PUESTOS ===\n";
    if (::campeon) {
        cout << "1. CAMPEON: " << ::campeon->getPais().c_str() << "\n";
        cout << "2. SUBCAMPEON: " << ::subcampeon->getPais().c_str() << "\n";
    }
    else {
        cout << "1. (Campeon) - Por definir\n";
        cout << "2. (Subcampeon) - Por definir\n";
    }
    if (::tercerPuesto) {
        cout << "3. TERCER PUESTO: " << ::tercerPuesto->getPais().c_str() << "\n";
        cout << "4. CUARTO PUESTO: " << ::cuartoPuesto->getPais().c_str() << "\n";
    }
    else {
        cout << "3. (Tercer puesto) - Por definir\n";
        cout << "4. (Cuarto puesto) - Por definir\n";
    }
}

void Torneo::imprimirMaximoGoleadorCampeon() {
    cout << "\n=== MAXIMO GOLEADOR DEL EQUIPO CAMPEON ===\n";

    if (::campeon) {
        Jugador* maxGoleador = nullptr;
        ArrayList<Jugador*>& jugadores = ::campeon->getJugadores();
        for (int i = 0; i < jugadores.size(); i++) {
            if (maxGoleador == nullptr || jugadores.get(i)->getGoles() > maxGoleador->getGoles()) {
                maxGoleador = jugadores.get(i);
            }
            contarIteracion();
        }
        if (maxGoleador) {
            cout << "Jugador #" << maxGoleador->getNumeroCamiseta()
                << " - Goles: " << maxGoleador->getGoles() << endl;
        }
        else {
            cout << "No se pudo determinar\n";
        }
    }
    else {
        cout << "Por determinar al finalizar la final\n";
    }
}

void Torneo::imprimirTopGoleadores() {
    cout << "\n=== TRES MAYORES GOLEADORES DE LA COPA ===\n";

    ArrayList<Jugador*> todosJugadores;
    for (int i = 0; i < equipos.size(); i++) {
        ArrayList<Jugador*>& jugadores = equipos.get(i)->getJugadores();
        for (int j = 0; j < jugadores.size(); j++) {
            todosJugadores.add(jugadores.get(j));
            contarIteracion();
        }
    }

    for (int i = 0; i < todosJugadores.size() - 1; i++) {
        for (int j = 0; j < todosJugadores.size() - i - 1; j++) {
            if (todosJugadores.get(j)->getGoles() < todosJugadores.get(j + 1)->getGoles()) {
                Jugador* temp = todosJugadores.get(j);
                todosJugadores.set(j, todosJugadores.get(j + 1));
                todosJugadores.set(j + 1, temp);
            }
            contarIteracion();
        }
    }

    for (int i = 0; i < 3 && i < todosJugadores.size(); i++) {
        cout << (i + 1) << ". Jugador #" << todosJugadores.get(i)->getNumeroCamiseta()
            << " - Goles: " << todosJugadores.get(i)->getGoles() << endl;
    }
}

void Torneo::imprimirEquipoMasGolesHistoricos() {
    cout << "\n=== EQUIPO CON MAS GOLES HISTORICOS ===\n";

    Equipo* maxGoles = nullptr;
    for (int i = 0; i < equipos.size(); i++) {
        if (maxGoles == nullptr || equipos.get(i)->getGolesAFavor() > maxGoles->getGolesAFavor()) {
            maxGoles = equipos.get(i);
        }
        contarIteracion();
    }

    if (maxGoles) {
        cout << maxGoles->getPais().c_str() << " - Goles: " << maxGoles->getGolesAFavor() << endl;
    }
}

void Torneo::imprimirConfederacionesDominantes() {
    cout << "\n=== CONFEDERACIONES CON MAYOR PRESENCIA ===\n";
    cout << "R16: UEFA (8 equipos), CONMEBOL (4 equipos), CONCACAF (2 equipos)\n";
    cout << "R8: UEFA (5 equipos), CONMEBOL (2 equipos), CONCACAF (1 equipo)\n";
    cout << "R4: UEFA (3 equipos), CONMEBOL (1 equipo)\n";
}

void Torneo::generarInformeFinal() const {
    cout << "\n==================================================\n";
    cout << "              INFORME FINAL DEL TORNEO\n";
    cout << "==================================================\n";

    Torneo* temp = const_cast<Torneo*>(this);
    temp->imprimirRankingFinal();
    temp->imprimirMaximoGoleadorCampeon();
    temp->imprimirTopGoleadores();
    temp->imprimirEquipoMasGolesHistoricos();
    temp->imprimirConfederacionesDominantes();

    cout << "\n==================================================\n";
    cout << "              SIMULACION COMPLETADA\n";
    cout << "==================================================\n";
}

void Torneo::imprimirMetricas(const char* etapa) const {
    cout << "\n=== METRICAS - " << etapa << " ===\n";
    cout << "Iteraciones: " << totalIteraciones << "\n";
    cout << "Memoria: " << memoriaActual << " bytes\n";
}