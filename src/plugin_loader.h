#pragma once
#include "class_diagram_parser.h"
#include "sequence_diagram_parser.h"
#include <string>

// C-интерфейс плагина
extern "C" {
    struct DiagramPlugin {
        const char* name;    // человекочитаемое имя
        // Экспорт class-диаграммы в свой формат; путь указывается плагином
        void (*export_class)(const std::vector<ClassInfo>* classes, const char* outPath);
        void (*export_sequence)(const std::vector<SequenceStep>* steps, const char* outPath);
    };
    typedef DiagramPlugin* (*create_plugin_t)();
}