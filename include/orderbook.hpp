#pragma once
#include <vector>
#include <map>
#include <cmath>

namespace hft {

struct OrderBook {
    double mid_price;
    double bid_spread; // bps
    double ask_spread; // bps
    double impact_coeff; // impact = coeff * qty / volume
    
    // Estimated fill price with impact
    double get_fill_price(double ref_price, int qty, bool is_buy) const {
        double spread = (is_buy ? ask_spread : bid_spread) / 10000.0;
        double impact = impact_coeff * std::abs((double)qty) / 100000.0;
        double slip = spread + impact;
        return is_buy ? ref_price * (1.0 + slip) : ref_price * (1.0 - slip);
    }
};

}
