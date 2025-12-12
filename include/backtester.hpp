#pragma once
#include <vector>
#include "data_loader.hpp"
#include "strategy.hpp"
#include "metrics.hpp"
#include "costs.hpp"

namespace hft {

struct BacktestResult {
    std::vector<Trade> trades;
    std::vector<double> equity_curve;
    double sharpe;
    double drawdown;
    double final_equity;
};

class Backtester {
public:
    static BacktestResult run(const std::vector<Bar>& bars, Strategy& strat, const CostModel& costs = {}, int lot = 1);
};

}
