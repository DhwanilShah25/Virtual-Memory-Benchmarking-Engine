#pragma once

#ifndef POLICY_HPP
#define POLICY_HPP

#include "types.hpp"
#include <string>

class Policy {
public:
    // Virtual destructor is mandatory for abstract base classes
    virtual ~Policy() = default;

    // The core function: Simulates accessing a page and returns true if it was a Page Fault
    virtual bool accessPage(PageID pageID) = 0;

    // Useful for our benchmarking table output
    virtual std::string getName() const = 0;
};

#endif