#pragma once
#include <vector>
#include <cmath>

namespace hft {

inline std::vector<double> sma(const std::vector<double>& x, int w) {
    std::vector<double> y(x.size(), std::nan(""));
    if (w <= 0) return y;
    double s = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        s += x[i];
        if (i >= (size_t)w) s -= x[i - w];
        if (i + 1 >= (size_t)w) y[i] = s / w;
    }
    return y;
}

inline std::vector<double> ema(const std::vector<double>& x, int w) {
    std::vector<double> y(x.size(), std::nan(""));
    if (w <= 0) return y;
    double alpha = 2.0 / (w + 1);
    double e = x[0];
    for (size_t i = 0; i < x.size(); ++i) {
        e = alpha * x[i] + (1 - alpha) * e;
        if (i + 1 >= (size_t)w) y[i] = e;
    }
    return y;
}

inline std::vector<double> rsi(const std::vector<double>& x, int w) {
    std::vector<double> y(x.size(), std::nan(""));
    if (w <= 0 || x.size() < (size_t)w) return y;
    double avg_gain = 0.0, avg_loss = 0.0;
    for (int i = 1; i <= w; ++i) {
        double d = x[i] - x[i - 1];
        if (d > 0) avg_gain += d; else avg_loss -= d;
    }
    avg_gain /= w; avg_loss /= w;
    for (size_t i = w + 1; i < x.size(); ++i) {
        double d = x[i] - x[i - 1];
        double gain = d > 0 ? d : 0.0;
        double loss = d < 0 ? -d : 0.0;
        avg_gain = (avg_gain * (w - 1) + gain) / w;
        avg_loss = (avg_loss * (w - 1) + loss) / w;
        double rs = (avg_loss == 0.0) ? 100.0 : (avg_gain / avg_loss);
        y[i] = 100.0 - (100.0 / (1.0 + rs));
    }
    return y;
}

}
