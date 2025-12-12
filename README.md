# HFT Backtester (C++)

A compact, production-style C++ backtesting engine tailored to the PACE Software Developer Intern JD. It demonstrates high-performance systems thinking, data processing, quantitative strategies, and robust metrics.

## Highlights
- Pure C++17, zero external deps, cross-platform (Windows/Linux).
- Modules: CSV data loader, strategy framework, indicators, backtesting engine, metrics.
- Strategies included: Momentum, Mean-Reversion.
- Outputs Sharpe Ratio, Max Drawdown, final equity.

## Project Structure
- `include/`: headers for core modules
- `src/`: implementations and `main.cpp`
- `tests/`: minimal test harness
- `data/`: sample dataset

## Build & Run (PowerShell)
```powershell
# From workspace root
mkdir build
Push-Location build
cmake -G "Ninja" ..
ninja
# Run app
./hft_backtester.exe ../data/sample.csv
# Run tests
./hft_tests.exe
Pop-Location
```
If Ninja isn't installed, replace with:
```powershell
cmake ..
cmake --build . --config Release
```

## Data Format
CSV with header: `ts,open,high,low,close,volume` (epoch ms).

## Future Extensions
- Multi-asset and pairs trading (cointegration, Kalman filter).
- Transaction costs, slippage models, order book simulator.
- Walk-forward optimization, parameter search, and risk management.
- gRPC service for batch backtests and dashboard UI.

## Why this aligns to PACE
- Backtesting automation and strategy design in C++.
- Efficient data processing on time-series.
- Clear metrics and extensible architecture ready for HFT-oriented evolution.
