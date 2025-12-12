#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <map>
#include "advanced_backtester.hpp"
#include "performance.hpp"

namespace hft {

inline void write_advanced_report(const std::string& base_path, const std::vector<AssetBacktest>& results) {
    // Summary CSV
    std::ofstream sum(base_path + "_summary.csv");
    sum << "asset,sharpe,max_dd,final_equity,trades,total_return\n";
    for (const auto& r : results) {
        double tr = (r.equity_curve.empty() ? 0 : (r.final_equity - 100000.0) / 100000.0);
        sum << r.asset << "," << r.sharpe << "," << r.max_dd << "," << r.final_equity << "," << r.num_trades << "," << tr << "\n";
    }
    
    // Detailed metrics CSV
    std::ofstream det(base_path + "_details.csv");
    det << "asset,bar_idx,equity,pnl\n";
    for (const auto& r : results) {
        for (size_t i = 0; i < r.equity_curve.size(); ++i) {
            det << r.asset << "," << i << "," << r.equity_curve[i] << "," << r.pnl_series[i] << "\n";
        }
    }
    
    // Combined equity curve for plotting
    std::ofstream eq(base_path + "_equity.csv");
    eq << "bar_idx";
    for (const auto& r : results) eq << "," << r.asset;
    eq << "\n";
    size_t max_bars = results.empty() ? 0 : results[0].equity_curve.size();
    for (size_t i = 0; i < max_bars; ++i) {
        eq << i;
        for (const auto& r : results) {
            eq << "," << (i < r.equity_curve.size() ? r.equity_curve[i] : r.equity_curve.back());
        }
        eq << "\n";
    }
}

}
