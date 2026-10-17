#include "logger.h"
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <filesystem>

void logLine(const std::string& msg) {
    std::filesystem::create_directories("logs");
    std::ofstream f("logs/app.log", std::ios::app);
    if (!f) return;
    auto t = std::time(nullptr);
    std::tm tm{}; localtime_r(&t, &tm);
    f << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " | " << msg << "\n";
}