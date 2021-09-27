#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"

using namespace std;


//DONE: Return the system's CPU
Processor& System::Cpu() {
   return cpu_;    
}

//DONE: Return a container composed of the system's processes
vector<Process>& System::Processes(){
   processes_.clear();
   vector<int>processID = LinuxParser::Pids();
     for (int start: processID){ 
           if ((LinuxParser::Ram(start) !="") && (LinuxParser::Command(start) !="")){
                processes_.push_back(start);
            }
            else start++;
      }
   std::sort(processes_.begin(),processes_.end());
   return processes_;
}

//DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { 
      return LinuxParser::Kernel();
}   

//DONE: Return the system's memory utilization
float System::MemoryUtilization() { 
      return LinuxParser::MemoryUtilization();
} 

//DONE: Return the operating system name
std::string System::OperatingSystem() {  
      return LinuxParser::OperatingSystem();
} 

//DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { 
      return LinuxParser::RunningProcesses();
} 

//DONE: Return the total number of processes on the system
int System::TotalProcesses() {  
      return LinuxParser::TotalProcesses();
} 
 
//DONE: Return the number of seconds since the system started running
long int System::UpTime() {  
       return LinuxParser::UpTime();
}