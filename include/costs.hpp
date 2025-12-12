#pragma once
namespace hft {

struct CostModel {
    double commission_per_share = 0.0; // fixed per unit
    double slippage_bps = 0.0;         // basis points of price

    double cost(double price, int qty) const {
        double comm = commission_per_share * std::abs(qty);
        double slip = (slippage_bps / 10000.0) * price * std::abs(qty);
        return comm + slip;
    }
};

}
