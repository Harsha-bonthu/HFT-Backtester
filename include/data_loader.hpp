#pragma once
#include <string>
#include <vector>
#include <optional>
#include <cstdint>

namespace hft {

struct Bar {
    std::int64_t ts; // epoch milliseconds
    double open;
    double high;
    double low;
    double close;
    double volume;
};

// Simple CSV loader: ts,open,high,low,close,volume
class DataLoader {
public:
    static std::vector<Bar> load_csv(const std::string& path);
};

}
