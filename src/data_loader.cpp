#include "data_loader.hpp"
#include <fstream>
#include <sstream>

namespace hft {

std::vector<Bar> DataLoader::load_csv(const std::string& path) {
    std::vector<Bar> out;
    std::ifstream f(path);
    if (!f.is_open()) return out;
    std::string line;
    // Skip header if present
    if (std::getline(f, line)) {
        if (line.find("ts") == std::string::npos || line.find(",") == std::string::npos) {
            // First line is data, push back after parsing below
            std::stringstream ss(line);
            Bar b{}; char c;
            ss >> b.ts >> c >> b.open >> c >> b.high >> c >> b.low >> c >> b.close >> c >> b.volume;
            out.push_back(b);
        }
    }
    while (std::getline(f, line)) {
        std::stringstream ss(line);
        Bar b{}; char c;
        ss >> b.ts >> c >> b.open >> c >> b.high >> c >> b.low >> c >> b.close >> c >> b.volume;
        if (!ss.fail()) out.push_back(b);
    }
    return out;
}

}
