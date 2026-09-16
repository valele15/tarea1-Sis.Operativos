#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct Activity {
    std::string id;
    std::string name;
    int duration_ms;
    std::vector<std::string> dependencies; // Tareas que deben terminar antes
    std::vector<std::string> dependents;   // Tareas que dependen de ESTA tarea
    int pending_deps = 0;
    bool failed = false;
};

using DAG = std::unordered_map<std::string, Activity>;

DAG parse_plan(const std::string& filename);
