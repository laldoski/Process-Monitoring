#include <string>
#include <iostream>
#include <sstream>
#include "format.h"
#include <fstream>
#include <cmath>
#include <chrono>
#include <iomanip>
using std::string;
using namespace std;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long sec){ 
    std::chrono::seconds seconds{sec};
    std::chrono::hours hours =
    std::chrono::duration_cast<std::chrono::hours>(seconds);
    seconds -= std::chrono::duration_cast<std::chrono::seconds>(hours);
    std::chrono::minutes minutes =
    std::chrono::duration_cast<std::chrono::minutes>(seconds);
    seconds -= std::chrono::duration_cast<std::chrono::seconds>(minutes);
    std::stringstream ss{};
    ss << std::setw(2) << std::setfill('0') << hours.count()     // HH
       << std::setw(1) << ":"                                    // :
       << std::setw(2) << std::setfill('0') << minutes.count()   // MM
       << std::setw(1) << ":"                                    // :
       << std::setw(2) << std::setfill('0') << seconds.count();  // SS
     return ss.str();   
  }
