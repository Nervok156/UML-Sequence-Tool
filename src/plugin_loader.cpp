#include "plugin_loader.h"
#include "logger.h"
#include <dlfcn.h>
#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

void runPlugins(const std::string& pluginDir,
                const std::vector<ClassInfo>& classes,
                const std::vector<SequenceStep>& steps,
                const std::string& outDir) {
    if (!fs::exists(pluginDir)) {
        std::cout << "  (папка плагинов отсутствует: " << pluginDir << ")\n";
        return;
    }
    fs::create_directories(outDir);

    for (auto& entry : fs::directory_iterator(pluginDir)) {
        auto path = entry.path();
        if (path.extension() != ".so") continue;

        std::cout << "  [plugin] загрузка " << path.filename() << " ...\n";

        dlerror(); // сбросить старые ошибки
        void* handle = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (!handle) {
            std::cerr << "  [plugin] НЕ ЗАГРУЖЕН " << path << ": "
                      << (dlerror() ? dlerror() : "unknown") << "\n";
            logLine(std::string("plugin load error: ") + path.string());
            continue;
        }

        auto create = (create_plugin_t)dlsym(handle, "create_plugin");
        if (!create) {
            std::cerr << "  [plugin] " << path << " без create_plugin()\n";
            dlclose(handle);
            continue;
        }

        DiagramPlugin* p = create();
        if (!p) {
            std::cerr << "  [plugin] create_plugin() вернул nullptr\n";
            dlclose(handle);
            continue;
        }

        std::string baseName = path.stem().string();
        std::string outClass = outDir + "/class-diagram." + baseName + ".out";
        std::string outSeq   = outDir + "/sequence-diagram." + baseName + ".out";

        try {
            if (p->export_class)    p->export_class(&classes, outClass.c_str());
            if (p->export_sequence) p->export_sequence(&steps, outSeq.c_str());
            std::cout << "  [plugin] " << (p->name ? p->name : "?") << " выполнен\n";
            logLine(std::string("plugin executed: ") + (p->name ? p->name : "?"));
        } catch (const std::exception& e) {
            std::cerr << "  [plugin] исключение в плагине: " << e.what() << "\n";
            logLine(std::string("plugin exception: ") + e.what());
        }

        dlclose(handle);
    }
}