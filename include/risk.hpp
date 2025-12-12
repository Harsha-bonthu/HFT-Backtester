#pragma once
#include <vector>
#include <cmath>
#include <algorithm>

namespace hft {

struct RiskControl {
    double max_position = 100;        // max units long/short
    double stop_loss_pct = 0.02;      // 2% stop-loss
    double take_profit_pct = 0.05;    // 5% take-profit
    double max_daily_loss = 2000;     // max loss per day
    bool use_vol_scaling = true;      // scale position by inverse volatility
};

// Compute rolling volatility (annualized)
inline double compute_volatility(const std::vector<double>& prices, int lookback = 20) {
    if ((int)prices.size() < lookback) return 0.01;
    std::vector<double> rets;
    for (size_t i = prices.size() - lookback; i < prices.size(); ++i) {
        double ret = (prices[i] - prices[i-1]) / prices[i-1];
        rets.push_back(ret);
    }
    double mean = 0; for (double r : rets) mean += r; mean /= rets.size();
    double var = 0; for (double r : rets) { double d = r - mean; var += d*d; } var /= rets.size();
    return std::sqrt(var) * std::sqrt(252.0); // annualized
}

// Apply position sizing based on volatility
inline int scale_position_by_vol(int base_qty, double vol) {
    if (vol < 0.01) vol = 0.01;
    double scale = 0.20 / vol; // target 20% annualized vol
    return (int)(base_qty * std::min(scale, 2.0)); // cap at 2x
}

}
