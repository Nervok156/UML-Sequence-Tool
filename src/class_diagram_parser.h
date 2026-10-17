#pragma once
#include "json_parser.h"
#include <string>
#include <vector>

struct ClassInfo {
    std::string name;
    std::vector<std::string> attributes;
    std::vector<std::string> methods;
    std::vector<std::string> relationships;
};

std::vector<ClassInfo> parseClassDiagram(const JsonValue& root);
std::vector<ClassInfo> loadClassDiagram(const std::string& path);