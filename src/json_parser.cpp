#include "json_parser.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstdlib>

std::string JsonValue::asString() const {
    if (type != String) throw std::runtime_error("Ожидалась строка в JSON");
    return strVal;
}

std::vector<std::string> JsonValue::asStringArray() const {
    if (type != Array) throw std::runtime_error("Ожидался массив в JSON");
    std::vector<std::string> r;
    for (auto& v : arrVal) r.push_back(v.asString());
    return r;
}

JsonValue JsonParser::parseFile(const std::string& path) {
    std::ifstream f(path);
    if (!f) throw std::runtime_error("Не удалось открыть файл: " + path);
    std::stringstream ss; ss << f.rdbuf();
    return parseString(ss.str());
}

JsonValue JsonParser::parseString(const std::string& s) {
    JsonParser p(s);
    p.skipWs();
    JsonValue v = p.parseValue();
    p.skipWs();
    if (p.pos != p.src.size())
        throw std::runtime_error("Лишние данные в JSON после позиции " + std::to_string(p.pos));
    return v;
}

void JsonParser::skipWs() {
    while (pos < src.size() && std::isspace((unsigned char)src[pos])) ++pos;
}

JsonValue JsonParser::parseValue() {
    skipWs();
    if (pos >= src.size()) throw std::runtime_error("Неожиданный конец JSON");
    char c = src[pos];
    if (c == '{') return parseObject();
    if (c == '[') return parseArray();
    if (c == '"') return parseStringLit();
    if (c == 't' || c == 'f' || c == 'n') return parseKeyword();
    return parseNumber();
}

JsonValue JsonParser::parseObject() {
    JsonValue v; v.type = JsonValue::Object;
    ++pos; // {
    skipWs();
    if (pos < src.size() && src[pos] == '}') { ++pos; return v; }
    while (true) {
        skipWs();
        if (pos >= src.size() || src[pos] != '"')
            throw std::runtime_error("Ожидался ключ в объекте JSON");
        JsonValue key = parseStringLit();
        skipWs();
        if (pos >= src.size() || src[pos] != ':')
            throw std::runtime_error("Ожидался ':' в объекте JSON");
        ++pos;
        v.objVal[key.strVal] = parseValue();
        skipWs();
        if (pos < src.size() && src[pos] == ',') { ++pos; continue; }
        if (pos < src.size() && src[pos] == '}') { ++pos; break; }
        throw std::runtime_error("Ожидался ',' или '}' в объекте JSON");
    }
    return v;
}

JsonValue JsonParser::parseArray() {
    JsonValue v; v.type = JsonValue::Array;
    ++pos; // [
    skipWs();
    if (pos < src.size() && src[pos] == ']') { ++pos; return v; }
    while (true) {
        v.arrVal.push_back(parseValue());
        skipWs();
        if (pos < src.size() && src[pos] == ',') { ++pos; continue; }
        if (pos < src.size() && src[pos] == ']') { ++pos; break; }
        throw std::runtime_error("Ожидался ',' или ']' в массиве JSON");
    }
    return v;
}

JsonValue JsonParser::parseStringLit() {
    JsonValue v; v.type = JsonValue::String;
    ++pos; // "
    std::string out;
    while (pos < src.size() && src[pos] != '"') {
        char c = src[pos++];
        if (c == '\\') {
            if (pos >= src.size()) throw std::runtime_error("Обрыв escape-последовательности");
            char e = src[pos++];
            switch (e) {
                case '"': out += '"'; break;
                case '\\': out += '\\'; break;
                case '/': out += '/'; break;
                case 'n': out += '\n'; break;
                case 't': out += '\t'; break;
                case 'r': out += '\r'; break;
                case 'u': {
                    // пропускаем 4 hex — для базового парсера достаточно
                    if (pos + 4 > src.size()) throw std::runtime_error("Некорректный \\u escape");
                    out += '?'; // упрощение
                    pos += 4;
                    break;
                }
                default: throw std::runtime_error(std::string("Неизвестный escape: \\") + e);
            }
        } else out += c;
    }
    if (pos >= src.size()) throw std::runtime_error("Незакрытая строка JSON");
    ++pos; // closing "
    v.strVal = std::move(out);
    return v;
}

JsonValue JsonParser::parseNumber() {
    size_t start = pos;
    if (pos < src.size() && (src[pos] == '-' || src[pos] == '+')) ++pos;
    while (pos < src.size() && (std::isdigit((unsigned char)src[pos]) || src[pos] == '.' ||
           src[pos] == 'e' || src[pos] == 'E' || src[pos] == '+' || src[pos] == '-')) ++pos;
    if (start == pos) throw std::runtime_error("Ожидалось число в JSON");
    JsonValue v; v.type = JsonValue::Number;
    v.numVal = std::strtod(src.substr(start, pos - start).c_str(), nullptr);
    return v;
}

JsonValue JsonParser::parseKeyword() {
    if (src.compare(pos, 4, "true") == 0)  { pos += 4; JsonValue v; v.type = JsonValue::Bool; v.boolVal = true;  return v; }
    if (src.compare(pos, 5, "false") == 0) { pos += 5; JsonValue v; v.type = JsonValue::Bool; v.boolVal = false; return v; }
    if (src.compare(pos, 4, "null") == 0)  { pos += 4; return JsonValue(); }
    throw std::runtime_error("Неизвестный литерал в JSON");
}