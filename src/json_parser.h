#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>

class JsonValue {
public:
    enum Type { Null, Bool, Number, String, Array, Object };
    Type type = Null;
    bool boolVal = false;
    double numVal = 0;
    std::string strVal;
    std::vector<JsonValue> arrVal;
    std::map<std::string, JsonValue> objVal;

    bool has(const std::string& k) const { return objVal.count(k) > 0; }
    const JsonValue& operator[](const std::string& k) const { return objVal.at(k); }
    const JsonValue& operator[](size_t i) const { return arrVal.at(i); }
    size_t size() const { return type == Array ? arrVal.size() : objVal.size(); }

    // Удобные аксессоры с валидацией
    std::string asString() const;
    std::vector<std::string> asStringArray() const;
};

class JsonParser {
public:
    static JsonValue parseFile(const std::string& path);
    static JsonValue parseString(const std::string& s);
private:
    const std::string& src;
    size_t pos = 0;
    JsonParser(const std::string& s) : src(s) {}
    void skipWs();
    JsonValue parseValue();
    JsonValue parseObject();
    JsonValue parseArray();
    JsonValue parseStringLit();
    JsonValue parseNumber();
    JsonValue parseKeyword();
};