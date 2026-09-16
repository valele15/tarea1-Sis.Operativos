#include "executor.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>
#include <chrono>
#include <thread>

using namespace std;

vector<pid_t> running_pids;

void handle_sigint(int sig) {
    (void)sig;
    cout << "\n[SEREMI] Inspeccion en progreso (Ctrl+C). Abortando..." << endl;
    for (pid_t pid : running_pids) {
        if (pid > 0) kill(pid, SIGKILL);
    }
    exit(1);
}

// Función auxiliar para abortar recursivamente ramas con error
void abort_branch(DAG& dag, const string& failed_id) {
    for (const string& dep_id : dag[failed_id].dependents) {
        if (!dag[dep_id].failed) {
            dag[dep_id].failed = true;
            cout << "[ERROR AISLADO] Cancelando tarea dependiente: " << dag[dep_id].name << endl;
            abort_branch(dag, dep_id);
        }
    }
}

void run_planner(DAG& dag, int K) {
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Calcular dependencias inversas (dependents)
    for (auto& [id, act] : dag) {
        for (const string& dep_id : act.dependencies) {
            if (dag.count(dep_id)) {
                dag[dep_id].dependents.push_back(id);
            }
        }
    }

    queue<string> ready_queue;
    for (auto& [id, act] : dag) {
        if (act.pending_deps == 0) {
            ready_queue.push(id);
        }
    }

    int active_processes = 0;

    while (!ready_queue.empty() || active_processes > 0) {

        while (active_processes < K && !ready_queue.empty()) {
            string current_id = ready_queue.front();
            ready_queue.pop();

            Activity& act = dag[current_id];
            if (act.failed) continue;

            int pipefd[2];
            if (pipe(pipefd) == -1) { perror("Error en pipe"); exit(1); }

            pid_t pid = fork();

            if (pid < 0) {
                perror("Error en fork");
                exit(1);
            } 
            else if (pid == 0) { 
                close(pipefd[0]);
                cout << "[PROCESO " << getpid() << "] Iniciando: " 
                     << act.name << " (" << act.duration_ms << " ms)" << endl;

                this_thread::sleep_for(chrono::milliseconds(act.duration_ms));

                // Simular fallo voluntario para probar aislamiento de errores
                if (act.name == "comprar_carne_FALLO") {
                    cout << "[PROCESO " << getpid() << "] FALLO EN TAREA: " << act.name << endl;
                    _exit(1); 
                }

                string msg = "Insumo de " + act.name + " listo.";
                write(pipefd[1], msg.c_str(), msg.length() + 1);
                close(pipefd[1]);

                _exit(0);
            } 
            else { 
                close(pipefd[1]);
                active_processes++;
                running_pids.push_back(pid);

                // Leer mensaje del pipe
                char buffer[256];
                ssize_t bytes = read(pipefd[0], buffer, sizeof(buffer));
                if (bytes > 0) {
                    cout << "[PIPE] Padre recibio mensaje: \"" << buffer << "\"" << endl;
                }
                close(pipefd[0]);
            }
        }

        if (active_processes > 0) {
            int status;
            pid_t finished_pid = waitpid(-1, &status, 0);

            if (finished_pid > 0) {
                active_processes--;

                for (auto it = running_pids.begin(); it != running_pids.end(); ++it) {
                    if (*it == finished_pid) {
                        running_pids.erase(it);
                        break;
                    }
                }

                // Evaluar si terminó con éxito o con error
                if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                    // Buscar qué tarea terminó y liberar sus dependientes
                    for (auto& [id, act] : dag) {
                        // Desbloqueo de tareas dependientes
                        for (const string& child_id : act.dependents) {
                            dag[child_id].pending_deps--;
                            if (dag[child_id].pending_deps == 0 && !dag[child_id].failed) {
                                ready_queue.push(child_id);
                            }
                        }
                    }
                } else {
                    // Manejo de error: Abortar solo la rama afectada
                    for (auto& [id, act] : dag) {
                        act.failed = true;
                        abort_branch(dag, id);
                    }
                }
            }
        }
    }

    cout << "\n[EVALUACION COMPLETADA] Proceso de planificacion finalizado." << endl;
}
