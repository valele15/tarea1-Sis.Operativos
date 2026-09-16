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
        if (pid > 0) {
            kill(pid, SIGKILL);
        }
    }
    exit(1);
}

void run_planner(DAG& dag, int K) {
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    queue<string> ready_queue;

    for (auto& [id, activity] : dag) {
        if (activity.pending_deps == 0) {
            ready_queue.push(id);
        }
    }

    int active_processes = 0;

    while (!ready_queue.empty() || active_processes > 0) {

        while (active_processes < K && !ready_queue.empty()) {
            string current_id = ready_queue.front();
            ready_queue.pop();

            Activity& act = dag[current_id];

            int pipefd[2];
            if (pipe(pipefd) == -1) {
                perror("Error en pipe");
                exit(1);
            }

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

                string msg = "Insumo de " + act.name + " listo.";
                write(pipefd[1], msg.c_str(), msg.length() + 1);
                close(pipefd[1]);

                cout << "[PROCESO " << getpid() << "] Finalizado: " << act.name << endl;
                _exit(0);
            } 
            else { 
                close(pipefd[1]);
                active_processes++;
                running_pids.push_back(pid);
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
            }
        }
    }

    cout << "\n[EXITO] Todas las actividades han finalizado." << endl;
}
