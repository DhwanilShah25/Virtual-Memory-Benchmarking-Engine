#include "fifo.hpp"

// Constructor initializes the capacity
FIFO::FIFO(FrameCount numFrames) : capacity(numFrames) {}

bool FIFO::accessPage(PageID pageID) {

    // Check if the page is already in memory (Page Hit)
    if (memorySet.find(pageID) != memorySet.end()) {
        return false; // Return false because there was NO page fault
    }

    // Page Fault 
    if (memorySet.size() >= capacity) { //memory is already full, evict the oldest page first.
        PageID oldestPage = memoryQueue.front();
        memoryQueue.pop();                    
        memorySet.erase(oldestPage);         
    }

    // Insert the newly requested page into memory
    memoryQueue.push(pageID);
    memorySet.insert(pageID);

    return true; // page fault
}

std::string FIFO::getName() const {
    return "FIFO";
}