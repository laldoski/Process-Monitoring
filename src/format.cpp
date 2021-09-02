#include <string>
#include <iostream>
#include <sstream>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds){ //{ return string(); }{ 
   int hours=0, minutes=0, second=0;
   hours = seconds/3600;
   seconds = seconds%3600;
   minutes = seconds/60;
   seconds = seconds%60;
   second = seconds; 
   std::ostringstream sstream;
   sstream << hours << ":" << minutes << ":" << second;
   string timeformat=sstream.str();
   return timeformat;
}


