#pragma once
#include <vector>
#include <cmath>

namespace hft {

inline double sharpe_ratio(const std::vector<double>& rets, double rf = 0.0) {
    if (rets.empty()) return 0.0;
    double mean = 0.0; for (double r : rets) mean += r; mean /= rets.size();
    double var = 0.0; for (double r : rets) { double d = r - mean; var += d * d; }
    var /= rets.size();
    double sd = std::sqrt(var);
    if (sd == 0.0) return 0.0;
    return (mean - rf) / sd;
}

inline double max_drawdown(const std::vector<double>& equity) {
    double peak = equity.empty() ? 0.0 : equity[0];
    double mdd = 0.0;
    for (double v : equity) {
        if (v > peak) peak = v;
        double dd = (peak - v) / peak;
        if (dd > mdd) mdd = dd;
    }
    return mdd;
}

}
