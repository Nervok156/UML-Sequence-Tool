#pragma once
#include "class_diagram_parser.h"
#include "sequence_diagram_parser.h"

void exportClassDiagramMarkdown(const std::vector<ClassInfo>& classes, const std::string& path);
void exportSequenceDiagramMarkdown(const std::vector<SequenceStep>& steps, const std::string& path);