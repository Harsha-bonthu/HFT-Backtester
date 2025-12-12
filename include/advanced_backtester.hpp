#pragma once
#include <vector>
#include <string>
#include <map>
#include "data_loader.hpp"
#include "strategy.hpp"
#include "risk.hpp"
#include "orderbook.hpp"
#include "costs.hpp"

namespace hft {

struct AssetBacktest {
    std::string asset;
    std::vector<Trade> trades;
    std::vector<double> equity_curve;
    std::vector<double> pnl_series;
    double sharpe = 0;
    double max_dd = 0;
    double final_equity = 0;
    int num_trades = 0;
};

class AdvancedBacktester {
public:
    static AssetBacktest run(const std::string& asset_name,
                             const std::vector<Bar>& bars,
                             Strategy& strat,
                             const CostModel& costs,
                             const RiskControl& risk,
                             const OrderBook& lob);
};

}
