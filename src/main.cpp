#include "json_parser.h"
#include "class_diagram_parser.h"
#include "sequence_diagram_parser.h"
#include "cycle_analyzer.h"
#include "markdown_exporter.h"
#include "logger.h"
#include <iostream>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

void runPlugins(const std::string&, const std::vector<ClassInfo>&,
                const std::vector<SequenceStep>&, const std::string&);

static void printClassDiagram(const std::vector<ClassInfo>& classes) {
    std::cout << "\nClass Diagram:\n";
    for (auto& c : classes) {
        std::cout << "  Class: " << c.name << "\n";
        if (!c.attributes.empty()) {
            std::cout << "    Attributes: ";
            for (size_t i = 0; i < c.attributes.size(); ++i) { if (i) std::cout << ", "; std::cout << c.attributes[i]; }
            std::cout << "\n";
        }
        if (!c.methods.empty()) {
            std::cout << "    Methods: ";
            for (size_t i = 0; i < c.methods.size(); ++i) { if (i) std::cout << ", "; std::cout << c.methods[i]; }
            std::cout << "\n";
        }
        if (!c.relationships.empty()) {
            std::cout << "    Relationships: ";
            for (size_t i = 0; i < c.relationships.size(); ++i) { if (i) std::cout << ", "; std::cout << c.relationships[i]; }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

static void printSequenceDiagram(const std::vector<SequenceStep>& steps) {
    std::cout << "Sequence Diagram:\n";
    for (auto& s : steps) {
        std::cout << "  " << s.actor << " -> " << s.target << ": " << s.action;
        if (!s.parameters.empty()) {
            std::cout << "(";
            for (size_t i = 0; i < s.parameters.size(); ++i) { if (i) std::cout << ", "; std::cout << s.parameters[i]; }
            std::cout << ")";
        }
        std::cout << "\n";
    }
}

int main(int argc, char** argv) {
    std::string classPath    = (argc > 1) ? argv[1] : "data/class-diagram.json";
    std::string sequencePath = (argc > 2) ? argv[2] : "data/sequence-diagram.json";

    std::cout << "=== UML Sequence Tool ===\n\n";
    std::cout << "Входные файлы:\n  " << classPath << "\n  " << sequencePath << "\n";
    logLine("start");

    try {
        auto classes = loadClassDiagram(classPath);
        auto steps   = loadSequenceDiagram(sequencePath);

        printClassDiagram(classes);
        std::cout << "\n";
        printSequenceDiagram(steps);

        std::cout << "\nАнализ циклических зависимостей:\n";
        auto res = analyzeCycles(classes);
        if (res.cycles.empty()) {
            std::cout << "  Циклов не обнаружено.\n";
        } else {
            for (auto& cyc : res.cycles) {
                std::cout << "  ВНИМАНИЕ: обнаружен цикл: ";
                for (size_t i = 0; i < cyc.size(); ++i) { std::cout << cyc[i] << " -> "; }
                std::cout << cyc.front() << "\n";
            }
            logLine("cycles found: " + std::to_string(res.cycles.size()));
        }

        fs::create_directories("output");
        exportClassDiagramMarkdown(classes, "output/class-diagram.md");
        exportSequenceDiagramMarkdown(steps, "output/sequence-diagram.md");
        std::cout << "\nОтчёты сохранены:\n  output/class-diagram.md\n  output/sequence-diagram.md\n";

        // Плагины
        std::cout << "\nЗагрузка плагинов:\n";
        runPlugins("plugins", classes, steps, "output");

        logLine("done");
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nОШИБКА: " << e.what() << "\n";
        logLine(std::string("error: ") + e.what());
        return 1;
    }
}