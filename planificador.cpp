#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Actividad {
    string id;
    string nombre;
    int tiempo;
    vector<string> dependencias;
};

int main() {

    srand(time(NULL));

    ifstream archivo("plan.txt");

    if (!archivo.is_open()) {
        cout << "No se pudo abrir plan.txt" << endl;
        return 1;
    }

    string linea;

    while (getline(archivo, linea)) {

        if (linea.empty()) {
            continue;
        }

        vector<string> campos;

        size_t inicio = 0;

        while (true) {

            size_t posicion = linea.find(':', inicio);

            if (posicion == string::npos) {
                campos.push_back(linea.substr(inicio));
                break;
            }

            campos.push_back(linea.substr(inicio, posicion - inicio));

            inicio = posicion + 1;
        }

        Actividad actividad;

        actividad.id = campos[0];
        actividad.nombre = campos[1];

        if (campos[2].empty()) {
            actividad.tiempo = 100 + rand() % 4901;
        } else {
            actividad.tiempo = stoi(campos[2]);
        }

        if (!campos[3].empty()) {

            size_t inicioDependencia = 0;

            while (true) {

                size_t posicion = campos[3].find(',', inicioDependencia);

                if (posicion == string::npos) {

                    actividad.dependencias.push_back(
                        campos[3].substr(inicioDependencia)
                    );

                    break;
                }

                actividad.dependencias.push_back(
                    campos[3].substr(
                        inicioDependencia,
                        posicion - inicioDependencia
                    )
                );

                inicioDependencia = posicion + 1;
            }
        }

        cout << "ID: " << actividad.id << endl;
        cout << "Nombre: " << actividad.nombre << endl;
        cout << "Tiempo: " << actividad.tiempo << " ms" << endl;

        cout << "Dependencias: ";

        if (actividad.dependencias.empty()) {
            cout << "ninguna";
        } else {
            for (string dependencia : actividad.dependencias) {
                cout << dependencia << " ";
            }
        }

        cout << endl << endl;
    }

    archivo.close();

    return 0;
}
