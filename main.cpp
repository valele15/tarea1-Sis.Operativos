#include <iostream>
#include <cstdlib>
#include "dag.hpp"
#include "executor.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <archivo_plan.txt> <K_concurrencia>" << endl;
        return 1;
    }

    string filename = argv[1];
    int K = atoi(argv[2]);

    if (K <= 0) {
        cerr << "El limite de concurrencia K debe ser mayor a 0." << endl;
        return 1;
    }

    DAG dag = parse_plan(filename);
    run_planner(dag, K);

    return 0;
}
