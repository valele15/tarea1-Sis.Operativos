#include "dag.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Función auxiliar para limpiar espacios en blanco y saltos de línea
static string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

DAG parse_plan(const string& filename) {
    DAG dag;
    ifstream archivo(filename);

    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << filename << endl;
        return dag;
    }

    srand(time(NULL));
    string linea;

    while (getline(archivo, linea)) {
        linea = trim(linea);
        if (linea.empty()) continue;

        // Separar la línea por los ':'
        vector<string> campos;
        size_t inicio = 0;
        while (true) {
            size_t pos = linea.find(':', inicio);
            if (pos == string::npos) {
                campos.push_back(linea.substr(inicio));
                break;
            }
            campos.push_back(linea.substr(inicio, pos - inicio));
            inicio = pos + 1;
        }

        if (campos.size() < 3) continue;

        Activity act;
        act.id = trim(campos[0]);
        act.name = trim(campos[1]);

        string tiempoStr = trim(campos[2]);
        if (tiempoStr.empty()) {
            // Tiempo aleatorio entre 100 y 5000 ms si el campo está vacío
            act.duration_ms = 100 + rand() % 4901;
        } else {
            act.duration_ms = stoi(tiempoStr);
        }

        // Separar dependencias por comas ','
        if (campos.size() > 3 && !trim(campos[3]).empty()) {
            string depsStr = trim(campos[3]);
            stringstream ss(depsStr);
            string dep_id;
            while (getline(ss, dep_id, ',')) {
                dep_id = trim(dep_id);
                if (!dep_id.empty()) {
                    act.dependencies.push_back(dep_id);
                }
            }
        }

        act.pending_deps = act.dependencies.size();
        act.failed = false;

        dag[act.id] = act;
    }

    archivo.close();
    return dag;
}