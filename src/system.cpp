#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>


#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


//~TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_;}

//~TODO: Return a container composed of the system's processes
vector<Process>& System::Processes()
 {
     processes_.clear();

  vector<int>processID=LinuxParser::Pids();

        for (int start: processID)
        {
        // Process processVec(pid);
       //  processes_.push_back<processVec>;
       
       processes_.push_back(start);
        }

      std::sort(processes_.begin(),processes_.end());

        return processes_;
  }

//~TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel();}   //return string(); 

//~TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization();} //return 0.0;}

//~TODO: Return the operating system name
std::string System::OperatingSystem() {  return LinuxParser::OperatingSystem();} //return string(); 

//~TODO: Return the number of processes actively running on the system
int System::RunningProcesses() {  return LinuxParser::RunningProcesses();} // return 0; }

//~TODO: Return the total number of processes on the system
int System::TotalProcesses() {  return LinuxParser::TotalProcesses();} // return 0;
 
//~TODO: Return the number of seconds since the system started running
long int System::UpTime() {  return LinuxParser::UpTime();}