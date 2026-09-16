#include "dag.hpp"
#include <iostream>

using namespace std;

DAG parse_plan(const string& filename) {
    (void)filename;
    DAG dag;

    // Grafo simulado para pruebas:
    // 1: prender_carbon (500ms)
    // 2: comprar_carne (800ms)
    // 3: asar_longaniza (600ms) -> depende de 1 y 2
    dag["1"] = {"1", "prender_carbon", 500, {}, {}, 0, false};
    dag["2"] = {"2", "comprar_carne", 800, {}, {}, 0, false};
    dag["3"] = {"3", "asar_longaniza", 600, {"1", "2"}, {}, 2, false};

    return dag;
}
