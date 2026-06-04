# Virtual Memory Benchmarking Engine

A high-performance C++ simulation engine designed to mathematically evaluate and benchmark Operating System memory management policies. This project ingests massive memory reference traces to analyze the time-space trade-offs of page replacement algorithms, bridging the gap between software mathematics and physical hardware constraints.

## Core Architecture & Features

This engine evaluates four distinct memory management algorithms:

* **LRU (Least Recently Used) - $O(1)$ Time Complexity:** Implemented using a strict combination of a `std::unordered_map` and a `std::list` (Doubly Linked List). This guarantees constant time $O(1)$ lookups and evictions, avoiding the $O(N)$ bottlenecks of naive array-based queues.
* **Clock (Second Chance) - Hardware Sympathy:** Simulates real-world OS hardware constraints (like Linux/Windows). Strict LRU is too expensive for CPU cache hits due to constant pointer updates. The Clock algorithm approximates LRU using a circular array and a boolean *Reference Bit*, achieving near-LRU efficiency with negligible overhead.
* **Optimal (Clairvoyant) Policy:** Establishes the absolute mathematical floor for page faults by looking ahead into the memory trace to evict the page that will not be used for the longest time. Serves as the benchmark baseline.
* **FIFO (First-In, First-Out):** The naive queue-based approach, used primarily to demonstrate failure under high-locality workloads.

### Programmatic Belady's Anomaly Profiler
Normally, allocating more physical RAM reduces page faults. Under specific cyclic workloads, FIFO violates this rule (Belady's Anomaly). This repository includes an automated C++ detection script that mathematically proves this anomaly by scaling physical frames and trapping fault inversions.

---

## Project Structure

The codebase strictly adheres to Object-Oriented Programming (OOP) principles, utilizing abstract base classes and polymorphism to ensure the engine is easily scalable for future algorithms.

```text
Memory-Benchmarking-Engine/
├── README.md                   
├── src/                        # Core C++ Engine (Business Logic)
│   ├── types.hpp               # Strict type aliases for memory safety
│   ├── policy.hpp              # Abstract base class (Polymorphism)
│   ├── simulator.cpp / .hpp    # Trace ingestion and execution engine
│   ├── fifo.cpp / .hpp         # FIFO Algorithm
│   ├── lru.cpp / .hpp          # O(1) LRU Algorithm
│   ├── clock.cpp / .hpp        # Clock Algorithm
│   └── optimal.cpp / .hpp      # Optimal Look-ahead Algorithm
│
├── executables/                # C++ Entry Points
│   ├── main.cpp                # Step-by-step console demonstrator
│   ├── benchmark.cpp           # Heavy-duty CSV data exporter
│   └── belady_detector.cpp     # Automated anomaly hunter
│
├── scripts/                    # Python Analytics Layer
│   ├── generate_trace.py       # Generates Locality, Cyclic, and Random traces
│   ├── plot_metrics.py         # Matplotlib visualization script
│   └── plot_metrics.py              # 1-Click Master Automation Pipeline
│
└── data/                       # I/O Folder
    └── (Contains .txt traces, .csv outputs, and .png graphs)

```

---

## Build and Execution

This project uses **C++17** for the high-speed simulation backend and **Python 3** (Pandas, Matplotlib) for orchestration and data visualization.

### 1. Compile the C++ Engine

```bash
# Compile the main benchmarking engine
g++ -std=c++17 -Isrc executables/benchmark.cpp src/simulator.cpp src/fifo.cpp src/lru.cpp src/clock.cpp src/optimal.cpp -o benchmark

# Compile the Belady's Anomaly Detector
g++ -std=c++17 -Isrc executables/belady_detector.cpp src/simulator.cpp src/fifo.cpp -o belady_detector

```

### 2. Generate Memory Traces

Simulate different CPU behaviors (80/20 Locality, Pure Random Chaos, and Mathematical Cyclic Traps) by generating 10,000-access trace files.

```bash
python3 scripts/generate_trace.py

```

### 3. The "One-Click" Master Pipeline

To run the full suite, execute the master orchestrator. This script will automatically fire up the compiled C++ engine, blast all the trace workloads from 1 to 100 frames, export the data to CSVs, and render textbook-quality graphs.

```bash
python3 scripts/plot_metrics.py

```

---

## Benchmarking Results

### The 80/20 High Locality Workload

When simulating a real-world database or gaming application (where 80% of accesses hit a concentrated 20% of memory), the benchmark mathematically proves why OS kernels abandon FIFO. LRU and Clock trace closely to the Optimal theoretical baseline, successfully exploiting temporal locality.
<img width="3000" height="1800" alt="Graph_3_High_Locality" src="https://github.com/user-attachments/assets/de426f5e-230e-44e0-a5cb-a221f24671ed" />

### Catching Belady's Anomaly

Running the dedicated `belady_detector` the trace and if detected, it yields the following proof:

```text
============================================
  HUNTING FOR BELADY'S ANOMALY (FIFO)
============================================
Frames: N-1 | Faults: X
Frames: N | Faults: Y (Y > X)  <-- ANOMALY DETECTED! Added RAM, but faults increased!

Belady's Anomaly is real.
