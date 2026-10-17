#pragma once
#include "json_parser.h"
#include <string>
#include <vector>

struct SequenceStep {
    std::string actor;
    std::string action;
    std::string target;
    std::vector<std::string> parameters;
};

std::vector<SequenceStep> parseSequenceDiagram(const JsonValue& root);
std::vector<SequenceStep> loadSequenceDiagram(const std::string& path);