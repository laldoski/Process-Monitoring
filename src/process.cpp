#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <unistd.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "process.h"
#include "linux_parser.h"

using namespace std;
using namespace LinuxParser;


using std::string;
using std::to_string;
using std::vector;


//~TODO: Return this process's ID
int Process::Pid() { 
    return this->pid_;

 }
  
 Process::Process(int pid_){
         this->pid_=pid_;  

 }
//~TODO: Return this process's CPU utilization
float Process::CpuUtilization(){  // { return 0; }{
       float utime, stime, cutime, cstime, starttime, total_time, 
       cpu_usage=0, seconds;
       int x=1;
     //  std::ofstream myfile; 
     //  string pidtext=this->pid_;
     //  myfile.open("output_"+ to_string(this->pid_)+".txt");
      std::ifstream upstream(LinuxParser::kProcDirectory + to_string(this->pid_) + 
       LinuxParser::kStatFilename);
     //  myfile <<  LinuxParser::kProcDirectory + to_string(this->pid_) + 
     //  LinuxParser::kStatFilename << std::endl;
       if (upstream.is_open()){
           std::istream_iterator<string> begin(upstream);
           std::istream_iterator<string> eos;
          //  myfile << "pid:" << this->pid_ <<std::endl;
           while ((begin!=eos) && (x<=23)){  
           switch(x){
               case 14:
               utime=std::stof(*begin);
              // myfile << "case 14:" << utime << std::endl;
               break;
               case 15:
               stime=std::stof(*begin);
             //   myfile << "case 15:" << stime << std::endl;
               break;
               case 16:
               cutime=std::stof(*begin);
             //  myfile << "case 16:" << cutime << std::endl;
               break; 
               case 17:
               cstime=std::stof(*begin);
              //  myfile << "case 17:" << cstime << std::endl;
               break;
               case 22:
               starttime=std::stof(*begin);
              // myfile << "case 22:" << starttime << std::endl;
               break;
               

            }
            

       begin ++; 
       x++;
       

      }

    total_time = utime + stime + cutime + cstime;
    seconds =LinuxParser::UpTime() - (starttime/sysconf(_SC_CLK_TCK));
   // myfile << "uptime:" << LinuxParser::UpTime(this->pid_) << std::endl;
    cpu_usage  = (((total_time / sysconf(_SC_CLK_TCK)) / seconds )*100);
   // myfile << "total_time:" <<total_time << std::endl;
   // myfile << "seconds:"<< seconds << std::endl;
  //  myfile << "cpu_usage:" << cpu_usage << std::endl;
  //  myfile.close();
    return cpu_usage;

  }
   else throw ("/proc/[pid]/stat not accessible");
}

//~TODO: Return the command that generated this process
string Process::Command(){
       return LinuxParser::Command(this->pid_);

}

//~TODO: Return this process's memory utilization
string Process::Ram() { 
       return LinuxParser::Ram(this->pid_);

}
//~TODO: Return the user (name) that generated this process
string Process::User() {  
       return LinuxParser::User(this->pid_);

}

//~TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
       return LinuxParser::UpTime(this->pid_);

 }

//~TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process & a){ 
       return (a.CpuUtilization() > this->CpuUtilization());

}

