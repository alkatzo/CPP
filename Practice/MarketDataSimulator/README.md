# Market Data Simulator

A C++ system that simulates 3 market data feeds with synchronized queues and timestamp-based reading.

## Features

- **3 Market Data Feeds**: Simulates real-time price updates for AAPL, GOOGL, and TSLA
- **Synchronized Queue**: Thread-safe priority queue that orders data by timestamp
- **Timestamp-based Reading**: Reader processes data in chronological order
- **Real-time Simulation**: Generates realistic price movements with random volatility
- **Multi-threaded Architecture**: Each feed runs in its own thread for concurrent data generation

## System Architecture

### Components

1. **MarketData Structure**: Contains symbol, price, volume, and timestamp
2. **SynchronizedQueue**: Thread-safe priority queue using mutex and condition variables
3. **MarketDataFeed**: Simulates a market data feed with random price movements
4. **MarketDataReader**: Reads and processes data in timestamp order

### Key Design Features

- **Priority Queue**: Uses `std::priority_queue` with custom comparator to order by timestamp
- **Thread Safety**: Mutex and condition variables ensure thread-safe operations
- **Graceful Shutdown**: Proper cleanup of threads and resources
- **Realistic Simulation**: Normal distribution for price changes, uniform distribution for volumes

## Building the Project

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.10 or higher
- Threading library (usually included with compiler)

### Build Instructions

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .
```

### Alternative: Direct Compilation

```bash
# On Linux/macOS with GCC
g++ -std=c++17 -O2 -pthread -o market_data_simulator market_data_simulator.cpp

# On Windows with MSVC
cl /std:c++17 /O2 market_data_simulator.cpp
```

## Running the Simulator

```bash
./market_data_simulator
```

The simulator will:
1. Start 3 market data feeds (AAPL, GOOGL, TSLA)
2. Begin generating price updates with random movements
3. Display timestamped data in chronological order
4. Continue until you press Enter to stop

## Sample Output

```
Market Data Simulator Starting...
Press Enter to stop the simulation...
[14:23:45.123] AAPL - Price: $150.25 Volume: 456
[14:23:45.234] GOOGL - Price: $2801.50 Volume: 789
[14:23:45.345] TSLA - Price: $798.75 Volume: 234
[14:23:45.456] AAPL - Price: $149.80 Volume: 567
...
```

## Technical Details

### Thread Safety
- Uses `std::mutex` for exclusive access to the queue
- `std::condition_variable` for efficient waiting when queue is empty
- RAII-style locking with `std::lock_guard` and `std::unique_lock`

### Performance Considerations
- Priority queue ensures O(log n) insertion and removal
- Condition variables prevent busy waiting
- Minimal locking scope to reduce contention

### Extensibility
- Easy to add more market data feeds
- Configurable price volatility and update frequency
- Modular design allows for different data processing strategies

## Customization

You can modify the following parameters in `main()`:
- Symbol names and base prices
- Volatility levels for price movements
- Update frequency ranges
- Number of feeds

## Error Handling

The system includes:
- Graceful shutdown handling
- Thread-safe operations
- Proper resource cleanup
- Input validation for price bounds 