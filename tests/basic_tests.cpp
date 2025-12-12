#include <iostream>
#include "data_loader.hpp"
#include "backtester.hpp"
#include "strategies/momentum.hpp"

int main() {
    using namespace hft;
    auto bars = DataLoader::load_csv("data/sample.csv");
    if (bars.empty()) { std::cout << "FAIL: sample.csv missing\n"; return 1; }
    MomentumStrategy s(5, 1);
    auto res = Backtester::run(bars, s);
    if (res.equity_curve.size() != bars.size()) { std::cout << "FAIL: equity size mismatch\n"; return 1; }
    std::cout << "OK: tests passed\n";
    return 0;
}
