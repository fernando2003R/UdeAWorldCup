#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Torneo.h"

using namespace std;

int main() {
    srand((unsigned int)time(nullptr));

    cout << "\n========================================\n";
    cout << "   UdeAWorldCup - Mundial FIFA 2026\n";
    cout << "========================================\n";

    Torneo mundial;

    cout << "\n[1/5] CARGANDO DATOS...\n";
    mundial.cargarEquiposDesdeCSV("C:\\Users\\PC\\Desktop\\equipos.csv");
    mundial.generarJugadoresArtificiales();
    mundial.imprimirMetricas("Carga de datos");

    cout << "\n[2/5] CONFORMANDO GRUPOS...\n";
    mundial.crearBombosYSortearGrupos();
    mundial.imprimirMetricas("Conformacion de grupos");

    cout << "\n[3/5] SIMULANDO FASE DE GRUPOS...\n";
    mundial.simularFaseGrupos();
    mundial.imprimirMetricas("Fase de grupos");

    cout << "\n[4/5] CONSTRUYENDO ELIMINATORIAS...\n";
    mundial.construirEliminatorias();
    mundial.imprimirFixtureEliminatorias();
    mundial.imprimirMetricas("Construccion eliminatorias");

    cout << "\n[5/5] SIMULANDO ELIMINATORIAS...\n";
    mundial.simularEliminatorias();
    mundial.imprimirMetricas("Eliminatorias completas");

    cout << "\n=== ESTADISTICAS FINALES ===\n";
    mundial.generarInformeFinal();
    mundial.imprimirMetricas("Estadisticas finales");

    cout << "\nSimulacion completada!\n";

    return 0;
}