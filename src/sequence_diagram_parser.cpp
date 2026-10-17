#include "sequence_diagram_parser.h"

static std::vector<std::string> optStrArray(const JsonValue& obj, const std::string& key) {
    if (!obj.has(key)) return {};
    return obj[key].asStringArray();
}

std::vector<SequenceStep> parseSequenceDiagram(const JsonValue& root) {
    if (!root.has("sequence"))
        throw std::runtime_error("В sequence-diagram JSON отсутствует 'sequence'");
    const auto& arr = root["sequence"];
    if (arr.type != JsonValue::Array)
        throw std::runtime_error("'sequence' должен быть массивом");
    std::vector<SequenceStep> steps;
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto& s = arr[i];
        for (auto req : {"actor", "action", "target"})
            if (!s.has(req))
                throw std::runtime_error("Шаг #" + std::to_string(i) + " без поля '" + req + "'");
        SequenceStep step;
        step.actor  = s["actor"].asString();
        step.action = s["action"].asString();
        step.target = s["target"].asString();
        step.parameters = optStrArray(s, "parameters");
        steps.push_back(std::move(step));
    }
    return steps;
}

std::vector<SequenceStep> loadSequenceDiagram(const std::string& path) {
    return parseSequenceDiagram(JsonParser::parseFile(path));
}