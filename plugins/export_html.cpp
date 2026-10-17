#include "../src/plugin_loader.h"
#include <fstream>
#include <sstream>

static void htmlEscape(std::ostream& o, const std::string& s) {
    for (char c : s) {
        switch (c) {
            case '<': o << "&lt;"; break;
            case '>': o << "&gt;"; break;
            case '&': o << "&amp;"; break;
            case '"': o << "&quot;"; break;
            default: o << c;
        }
    }
}

static void export_class_html(const std::vector<ClassInfo>* classes, const char* out) {
    std::ofstream f(out);
    if (!f) return;
    f << "<!DOCTYPE html>\n<html><head><meta charset=\"utf-8\">"
      << "<title>Class Diagram</title></head><body>\n";
    f << "<h1>Class Diagram</h1>\n";
    for (auto& c : *classes) {
        f << "<h2>Class: "; htmlEscape(f, c.name); f << "</h2>\n";
        if (!c.attributes.empty()) {
            f << "<h3>Attributes</h3><ul>\n";
            for (auto& a : c.attributes) { f << "<li>"; htmlEscape(f, a); f << "</li>\n"; }
            f << "</ul>\n";
        }
        if (!c.methods.empty()) {
            f << "<h3>Methods</h3><ul>\n";
            for (auto& m : c.methods) { f << "<li>"; htmlEscape(f, m); f << "</li>\n"; }
            f << "</ul>\n";
        }
        if (!c.relationships.empty()) {
            f << "<h3>Relationships</h3><ul>\n";
            for (auto& r : c.relationships) { f << "<li>"; htmlEscape(f, r); f << "</li>\n"; }
            f << "</ul>\n";
        }
    }
    f << "</body></html>\n";
}

static void export_seq_html(const std::vector<SequenceStep>* steps, const char* out) {
    std::ofstream f(out);
    if (!f) return;
    f << "<!DOCTYPE html>\n<html><head><meta charset=\"utf-8\">"
      << "<title>Sequence Diagram</title></head><body>\n";
    f << "<h1>Sequence Diagram</h1>\n<ol>\n";
    for (auto& s : *steps) {
        f << "<li><b>"; htmlEscape(f, s.actor); f << "</b> -&gt; <b>";
        htmlEscape(f, s.target); f << "</b>: ";
        htmlEscape(f, s.action); f << "(";
        for (size_t i = 0; i < s.parameters.size(); ++i) {
            if (i) f << ", ";
            htmlEscape(f, s.parameters[i]);
        }
        f << ")</li>\n";
    }
    f << "</ol>\n</body></html>\n";
}

static DiagramPlugin g_plugin = {
    "HTML Exporter Plugin",
    export_class_html,
    export_seq_html
};

extern "C" DiagramPlugin* create_plugin() { return &g_plugin; }
