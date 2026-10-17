#pragma once
#include "class_diagram_parser.h"
#include <vector>
#include <string>

struct CycleResult {
    std::vector<std::vector<std::string>> cycles;
};

CycleResult analyzeCycles(const std::vector<ClassInfo>& classes);