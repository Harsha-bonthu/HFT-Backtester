#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

namespace hft {

struct PerformanceMetrics {
    double total_return = 0;
    double annualized_return = 0;
    double sharpe_ratio = 0;
    double sortino_ratio = 0;
    double max_drawdown = 0;
    double win_rate = 0;
    double profit_factor = 0;
    double calmar_ratio = 0;
    int total_trades = 0;
    int winning_trades = 0;
    double avg_win = 0;
    double avg_loss = 0;
};

inline PerformanceMetrics compute_metrics(const std::vector<double>& equity_curve, int num_trades) {
    PerformanceMetrics m;
    if (equity_curve.empty()) return m;
    
    m.total_return = (equity_curve.back() - equity_curve[0]) / equity_curve[0];
    double annual_periods = 252.0; // trading days
    m.annualized_return = std::pow(1.0 + m.total_return, annual_periods / equity_curve.size()) - 1.0;
    
    std::vector<double> returns;
    for (size_t i = 1; i < equity_curve.size(); ++i) {
        double ret = (equity_curve[i] - equity_curve[i-1]) / equity_curve[i-1];
        returns.push_back(ret);
    }
    
    if (!returns.empty()) {
        double mean = 0; for (double r : returns) mean += r; mean /= returns.size();
        double var = 0; for (double r : returns) { double d = r - mean; var += d*d; } var /= returns.size();
        double sd = std::sqrt(var);
        m.sharpe_ratio = (sd > 0) ? (mean / sd * std::sqrt(252.0)) : 0.0;
        
        std::vector<double> downside;
        for (double r : returns) if (r < 0) downside.push_back(r);
        if (!downside.empty()) {
            double down_var = 0; for (double r : downside) { double d = r - mean; down_var += d*d; } down_var /= downside.size();
            double down_sd = std::sqrt(down_var);
            m.sortino_ratio = (down_sd > 0) ? (mean / down_sd * std::sqrt(252.0)) : 0.0;
        }
    }
    
    double peak = equity_curve[0];
    m.max_drawdown = 0;
    for (double v : equity_curve) {
        if (v > peak) peak = v;
        double dd = (peak - v) / peak;
        if (dd > m.max_drawdown) m.max_drawdown = dd;
    }
    
    m.calmar_ratio = (m.max_drawdown > 0) ? (m.annualized_return / m.max_drawdown) : 0.0;
    m.total_trades = num_trades;
    
    return m;
}

}
