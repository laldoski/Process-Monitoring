#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <unistd.h>
#include <cstdio>
#include "process.h"
#include "linux_parser.h"

using namespace LinuxParser;

using std::string;
using std::to_string;
using std::vector;

//~TODO: Return this process's ID
int Process::Pid() { return this->pid_; }
  
    Process::Process(int pid_)
     {
    this->pid_=pid_;  
     }
//~TODO: Return this process's CPU utilization
float Process::CpuUtilization()  // { return 0; }
{
float utime, stime, cutime, cstime, starttime, total_time, cpu_usage, seconds;
int x=1;
 std::ifstream upstream(LinuxParser::kProcDirectory + to_string(this->pid_)+ LinuxParser::kStatFilename);
   if (upstream.is_open())
   {
     std::istream_iterator<float> begin(upstream);
     std::istream_iterator<float> eos;
      while ((*begin!=upstream.eof()) && (x<=23))
      {
           switch(x)
           {
               case 14:
               utime=*begin; 
               case 15:
               stime=*begin; 
               case 16:
               cutime=*begin; 
               case 17:
               cstime=*begin; 
               case 22:
               starttime=*begin;
               default:
               break;
            }
            begin ++; 
            x++;

      }

   total_time = utime + stime + cutime + cstime;
   seconds = starttime - (starttime/sysconf(_SC_CLK_TCK));
   cpu_usage  = (((total_time /sysconf(_SC_CLK_TCK))/ seconds ) * 100);
   return cpu_usage;

   }
   else throw ("/proc/[pid]/stat not accessible");
}



//~TODO: Return the command that generated this process
string Process::Command()// { return string(); }
{
 return LinuxParser::Command(this->pid_);
}

//~TODO: Return this process's memory utilization
string Process::Ram()  //{  return string(); }
{
 return LinuxParser::Ram(this->pid_);
}
//~TODO: Return the user (name) that generated this process
string Process::User()   // { return string(); }
{
 return LinuxParser::User(this->pid_);
}

//~TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(this->pid_); }

//~TODO: Overload the "less than" comparison operator for Process objects

bool Process::operator<(Process & a)  
{ 
   //return (a.LinuxParser::CpuUtilization() > this->LinuxParser::CpuUtilization());
    return (a.CpuUtilization() > this->CpuUtilization());

}