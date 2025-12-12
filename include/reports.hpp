#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "backtester.hpp"

namespace hft {

inline void write_equity_csv(const std::string& path, const std::vector<double>& equity) {
    std::ofstream f(path);
    f << "index,equity\n";
    for (size_t i = 0; i < equity.size(); ++i) {
        f << i << "," << equity[i] << "\n";
    }
}

inline void write_summary_csv(const std::string& path, const std::string& name, const BacktestResult& r) {
    std::ofstream f(path);
    f << "strategy,sharpe,max_drawdown,final_equity,trade_count\n";
    f << name << "," << r.sharpe << "," << r.drawdown << "," << r.final_equity << "," << r.trades.size() << "\n";
}

}
