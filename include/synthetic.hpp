#pragma once
#include <vector>
#include <random>
#include <cstdint>
#include "data_loader.hpp"

namespace hft {

inline std::vector<Bar> generate_random_walk(int n, double start=100.0, double drift=0.0002, double vol=0.005, std::int64_t ts0=1731321600000, std::int64_t dt_ms=60000) {
    std::mt19937_64 rng(42);
    std::normal_distribution<double> z(0.0, 1.0);
    std::vector<Bar> bars; bars.reserve(n);
    double price = start;
    for (int i=0;i<n;++i) {
        double ret = drift + vol * z(rng);
        double close = price * (1.0 + ret);
        double high = std::max(price, close) * (1.0 + 0.001);
        double low  = std::min(price, close) * (1.0 - 0.001);
        double open = price;
        double volu = 10000 + 1000 * std::abs(z(rng));
        bars.push_back({ts0 + (std::int64_t)i*dt_ms, open, high, low, close, volu});
        price = close;
    }
    return bars;
}

}
