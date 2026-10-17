#include "class_diagram_parser.h"

static std::vector<std::string> optStrArray(const JsonValue& obj, const std::string& key) {
    if (!obj.has(key)) return {};
    return obj[key].asStringArray();
}

std::vector<ClassInfo> parseClassDiagram(const JsonValue& root) {
    if (!root.has("classes"))
        throw std::runtime_error("В class-diagram JSON отсутствует ключ 'classes'");
    const auto& arr = root["classes"];
    if (arr.type != JsonValue::Array)
        throw std::runtime_error("'classes' должен быть массивом");
    std::vector<ClassInfo> result;
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto& c = arr[i];
        if (!c.has("name"))
            throw std::runtime_error("Класс #" + std::to_string(i) + " без имени");
        ClassInfo info;
        info.name = c["name"].asString();
        info.attributes    = optStrArray(c, "attributes");
        info.methods       = optStrArray(c, "methods");
        info.relationships = optStrArray(c, "relationships");
        result.push_back(std::move(info));
    }
    return result;
}

std::vector<ClassInfo> loadClassDiagram(const std::string& path) {
    return parseClassDiagram(JsonParser::parseFile(path));
}
