#include "../src/plugin_loader.h"
#include <fstream>

static void export_class_json(const std::vector<ClassInfo>* classes, const char* out) {
    std::ofstream f(out);
    f << "{\n  \"classes\": [\n";
    for (size_t i = 0; i < classes->size(); ++i) {
        auto& c = (*classes)[i];
        f << "    {\"name\": \"" << c.name << "\", "
          << "\"attributes_count\": " << c.attributes.size() << ", "
          << "\"methods_count\": " << c.methods.size() << "}";
        if (i + 1 < classes->size()) f << ",";
        f << "\n";
    }
    f << "  ]\n}\n";
}

static void export_seq_json(const std::vector<SequenceStep>* steps, const char* out) {
    std::ofstream f(out);
    f << "{\n  \"sequence\": [\n";
    for (size_t i = 0; i < steps->size(); ++i) {
        auto& s = (*steps)[i];
        f << "    {\"actor\": \"" << s.actor << "\", \"action\": \"" << s.action
          << "\", \"target\": \"" << s.target << "\"}";
        if (i + 1 < steps->size()) f << ",";
        f << "\n";
    }
    f << "  ]\n}\n";
}

static DiagramPlugin g_plugin = {
    "JSON Exporter Plugin",
    export_class_json,
    export_seq_json
};

extern "C" DiagramPlugin* create_plugin() { return &g_plugin; }