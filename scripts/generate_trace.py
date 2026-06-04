import random
import os

class TraceGenerator:

    def __init__(self, num_accesses, max_page_id):
        """num_accesses: How many total page requests to generate, 
        max_page_id: The universe of available virtual pages."""
        self.num_accesses = num_accesses
        self.max_page_id = max_page_id

    def generate_random(self):
        """Pure random page accesses (Uniform Distribution)"""
        return [random.randint(1, self.max_page_id) for _ in range(self.num_accesses)]

    def generate_locality(self, hot_access_pct=0.80, hot_page_pct=0.20):
        """
        Simulates Locality of Reference with configurable parameters.
        - hot_access_pct: The probability (0.0 to 1.0) of accessing a hot page.
        - hot_page_pct: The percentage (0.0 to 1.0) of total pages considered "hot".
        """
        trace = []
        # Calculate how many absolute pages make up the "hot" working set
        hot_pages_count = max(1, int(self.max_page_id * hot_page_pct))
        
        for _ in range(self.num_accesses):
            if random.random() < hot_access_pct:
                # Access a hot page (High probability, narrow range)
                trace.append(random.randint(1, hot_pages_count))
            else:
                # Access a cold page (Low probability, wide range)
                trace.append(random.randint(hot_pages_count + 1, self.max_page_id))
        return trace

    def generate_cyclic(self, loop_size):
        """
        Generates a looping access pattern (e.g., 1, 2, 3, 4, 5, 1, 2...).
        """
        trace = []
        for i in range(self.num_accesses):
            trace.append((i % loop_size) + 1)
        return trace

    def save_trace(self, trace, filename):
        """Saves the trace to a text file (one page ID per line) for C++ ingestion."""
        # Ensure the data directory exists
        os.makedirs(os.path.dirname(filename), exist_ok=True)
        
        with open(filename, 'w') as f:
            for page in trace:
                f.write(f"{page}\n")
        print(f"Saved {len(trace)} memory accesses to {filename}")

import os # Make sure this is at the very top of your file!

if __name__ == "__main__":
    
    # Dynamically find where this script is located, and point to the data folder next to it
    script_dir = os.path.dirname(os.path.abspath(__file__))
    data_dir = os.path.join(script_dir, "..", "data")

    # Parameters for our benchmark
    TOTAL_ACCESSES = 10000
    MAX_VIRTUAL_PAGES = 100

    generator = TraceGenerator(TOTAL_ACCESSES, MAX_VIRTUAL_PAGES)

    # Generate and save all traces using the robust absolute path
    random_trace = generator.generate_random()
    generator.save_trace(random_trace, os.path.join(data_dir, "random_trace.txt"))

    standard_trace = generator.generate_locality(hot_access_pct=0.80, hot_page_pct=0.20)
    generator.save_trace(standard_trace, os.path.join(data_dir, "standard_locality_trace.txt"))

    high_locality_trace = generator.generate_locality(hot_access_pct=0.95, hot_page_pct=0.05)
    generator.save_trace(high_locality_trace, os.path.join(data_dir, "high_locality_trace.txt"))

    cyclic_trace = generator.generate_cyclic(loop_size=5)
    generator.save_trace(cyclic_trace, os.path.join(data_dir, "cyclic_trace.txt"))