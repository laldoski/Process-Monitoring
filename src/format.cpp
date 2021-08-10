#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) //{ return string(); }
{ 
 string second,  minutes, hours;
hours = to_string(seconds/3600);
minutes = to_string(seconds/60)%60;
second = to_string((seconds/60)%60)%60;
 std::ostringstream convert;

convert << hours << ":" << minutes << ":" << second;
   return convert;
}