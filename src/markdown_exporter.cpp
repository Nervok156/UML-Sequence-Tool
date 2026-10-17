#include "markdown_exporter.h"
#include <fstream>
#include <stdexcept>

void exportClassDiagramMarkdown(const std::vector<ClassInfo>& classes, const std::string& path) {
    std::ofstream f(path);
    if (!f) throw std::runtime_error("Не могу открыть для записи: " + path);
    f << "# Class Diagram\n\n";
    for (auto& c : classes) {
        f << "## Class: " << c.name << "\n\n";
        if (!c.attributes.empty()) {
            f << "**Attributes:**\n";
            for (auto& a : c.attributes) f << "- " << a << "\n";
            f << "\n";
        }
        if (!c.methods.empty()) {
            f << "**Methods:**\n";
            for (auto& m : c.methods) f << "- " << m << "\n";
            f << "\n";
        }
        if (!c.relationships.empty()) {
            f << "**Relationships:** ";
            for (size_t i = 0; i < c.relationships.size(); ++i) {
                if (i) f << ", ";
                f << c.relationships[i];
            }
            f << "\n\n";
        }
    }
}

void exportSequenceDiagramMarkdown(const std::vector<SequenceStep>& steps, const std::string& path) {
    std::ofstream f(path);
    if (!f) throw std::runtime_error("Не могу открыть для записи: " + path);
    f << "# Sequence Diagram\n\n";
    for (auto& s : steps) {
        f << "`" << s.actor << "` -> `" << s.target << "`: **" << s.action << "**";
        if (!s.parameters.empty()) {
            f << "(";
            for (size_t i = 0; i < s.parameters.size(); ++i) {
                if (i) f << ", ";
                f << s.parameters[i];
            }
            f << ")";
        }
        f << "\n\n";
    }
}