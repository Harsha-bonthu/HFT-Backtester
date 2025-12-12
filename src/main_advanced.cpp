#include <iostream>
#include <iomanip>
#include "data_loader.hpp"
#include "advanced_backtester.hpp"
#include "advanced_reports.hpp"
#include "strategies/momentum.hpp"
#include "strategies/mean_reversion.hpp"
#include "synthetic.hpp"

int main(int argc, char** argv) {
    using namespace hft;
    
    // Generate synthetic assets
    std::cout << "=== HFT BACKTESTER: ADVANCED DEMO ===\n\n";
    std::vector<std::string> assets = {"AAPL", "GOOGL", "MSFT"};
    std::map<std::string, std::vector<Bar>> asset_data;
    
    std::cout << "Generating synthetic market data...\n";
    for (const auto& asset : assets) {
        // Each asset with different drift and volatility
        double drift = (asset == "AAPL") ? 0.0005 : (asset == "GOOGL" ? 0.0008 : 0.0003);
        double vol = (asset == "AAPL") ? 0.008 : (asset == "GOOGL" ? 0.012 : 0.006);
        asset_data[asset] = generate_random_walk(1000, 100.0, drift, vol);
        std::cout << "  " << asset << ": drift=" << drift << " vol=" << vol << "\n";
    }
    
    // Risk controls and order book
    RiskControl risk;
    risk.max_position = 100;
    risk.stop_loss_pct = 0.02;
    risk.take_profit_pct = 0.05;
    risk.max_daily_loss = 2000;
    risk.use_vol_scaling = true;
    
    OrderBook lob;
    lob.mid_price = 100.0;
    lob.bid_spread = 2.0;   // 0.02% bid spread
    lob.ask_spread = 2.0;   // 0.02% ask spread
    lob.impact_coeff = 0.5; // impact coefficient
    
    CostModel costs;
    costs.commission_per_share = 0.001;
    costs.slippage_bps = 0.5;
    
    // Strategies
    MomentumStrategy mom(30, 5);
    MeanReversionStrategy mr(20, 0.004, 3);
    
    // Run backtests
    std::cout << "\nRunning advanced backtests with LOB and risk controls...\n";
    std::vector<AssetBacktest> results_mom, results_mr;
    
    for (const auto& asset : assets) {
        std::cout << "  Testing " << asset << "...\n";
        
        auto res_m = AdvancedBacktester::run(asset + "_MOM", asset_data[asset], mom, costs, risk, lob);
        results_mom.push_back(res_m);
        
        auto res_mr = AdvancedBacktester::run(asset + "_MR", asset_data[asset], mr, costs, risk, lob);
        results_mr.push_back(res_mr);
    }
    
    // Report results
    std::cout << "\n=== MOMENTUM STRATEGY RESULTS ===\n";
    std::cout << std::left << std::setw(12) << "Asset" 
              << std::setw(12) << "Sharpe" 
              << std::setw(12) << "MaxDD" 
              << std::setw(15) << "Final Equity"
              << std::setw(10) << "Trades" << "\n";
    std::cout << std::string(60, '-') << "\n";
    for (const auto& r : results_mom) {
        std::cout << std::left << std::setw(12) << r.asset 
                  << std::setw(12) << std::fixed << std::setprecision(4) << r.sharpe
                  << std::setw(12) << std::fixed << std::setprecision(6) << r.max_dd
                  << std::setw(15) << std::fixed << std::setprecision(2) << r.final_equity
                  << std::setw(10) << r.num_trades << "\n";
    }
    
    std::cout << "\n=== MEAN REVERSION STRATEGY RESULTS ===\n";
    std::cout << std::left << std::setw(12) << "Asset" 
              << std::setw(12) << "Sharpe" 
              << std::setw(12) << "MaxDD" 
              << std::setw(15) << "Final Equity"
              << std::setw(10) << "Trades" << "\n";
    std::cout << std::string(60, '-') << "\n";
    for (const auto& r : results_mr) {
        std::cout << std::left << std::setw(12) << r.asset 
                  << std::setw(12) << std::fixed << std::setprecision(4) << r.sharpe
                  << std::setw(12) << std::fixed << std::setprecision(6) << r.max_dd
                  << std::setw(15) << std::fixed << std::setprecision(2) << r.final_equity
                  << std::setw(10) << r.num_trades << "\n";
    }
    
    // Compute portfolio metrics
    double total_mom_return = 0, total_mr_return = 0;
    int total_mom_trades = 0, total_mr_trades = 0;
    for (const auto& r : results_mom) {
        total_mom_return += (r.final_equity - 100000.0);
        total_mom_trades += r.num_trades;
    }
    for (const auto& r : results_mr) {
        total_mr_return += (r.final_equity - 100000.0);
        total_mr_trades += r.num_trades;
    }
    
    std::cout << "\n=== PORTFOLIO SUMMARY ===\n";
    std::cout << "Momentum Strategy:\n";
    std::cout << "  Total P&L: $" << std::fixed << std::setprecision(2) << total_mom_return << "\n";
    std::cout << "  Total Trades: " << total_mom_trades << "\n";
    std::cout << "Mean Reversion Strategy:\n";
    std::cout << "  Total P&L: $" << std::fixed << std::setprecision(2) << total_mr_return << "\n";
    std::cout << "  Total Trades: " << total_mr_trades << "\n";
    
    // Write reports
    std::cout << "\nWriting advanced reports...\n";
    write_advanced_report("results_momentum", results_mom);
    write_advanced_report("results_meanreversion", results_mr);
    
    std::cout << "Done. Reports written:\n";
    std::cout << "  results_momentum_summary.csv\n";
    std::cout << "  results_momentum_details.csv\n";
    std::cout << "  results_momentum_equity.csv\n";
    std::cout << "  results_meanreversion_summary.csv\n";
    std::cout << "  results_meanreversion_details.csv\n";
    std::cout << "  results_meanreversion_equity.csv\n";
    
    return 0;
}
