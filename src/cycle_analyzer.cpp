#include "cycle_analyzer.h"
#include <unordered_map>
#include <functional>
#include <set>

CycleResult analyzeCycles(const std::vector<ClassInfo>& classes) {
    std::unordered_map<std::string, std::vector<std::string>> graph;
    for (auto& c : classes) graph[c.name] = c.relationships;

    CycleResult out;
    std::set<std::string> visited, inStack;
    std::vector<std::string> stack;
    std::set<std::vector<std::string>> uniq; // дедупликация циклов

    std::function<void(const std::string&)> dfs = [&](const std::string& node) {
        visited.insert(node);
        inStack.insert(node);
        stack.push_back(node);
        for (auto& nb : graph[node]) {
            if (!graph.count(nb)) continue; // связь на несуществующий класс — пропускаем
            if (inStack.count(nb)) {
                // нашли цикл: от nb до текущего node
                auto it = std::find(stack.begin(), stack.end(), nb);
                std::vector<std::string> cyc(it, stack.end());
                // Нормализуем: начинаем с минимального по алфавиту узла
                auto minIt = std::min_element(cyc.begin(), cyc.end());
                std::rotate(cyc.begin(), minIt, cyc.end());
                if (uniq.insert(cyc).second) out.cycles.push_back(cyc);
            } else if (!visited.count(nb)) {
                dfs(nb);
            }
        }
        inStack.erase(node);
        stack.pop_back();
    };

    for (auto& kv : graph)
        if (!visited.count(kv.first)) dfs(kv.first);
    return out;
}