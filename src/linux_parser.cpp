#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
//#include <filesystem> 
#include <ctype.h>
#include "linux_parser.h"
#include <iterator>
#include <cstdio>
#include <fstream>
#include <stdio.h>

//namespace fs=std::filesystem;

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// ~BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
 /* ifstream prostream(kProcDirectory);
    for (const auto & entry : fs::directory_iterator(kProcDirectory))
        if (isdigit(entry))
        pids.push_back(entry);
        return pids;
 */

   DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}
  


// ~TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() 
{ 
  bool MemTotalfound=false;
  bool  MemAvilfound=false;
  float MemTotal, MemAvil, MemUtil;
  string  namedescription, line, sizedescripter;
  std::ifstream streamMem(kProcDirectory + kMeminfoFilename);
     if (streamMem.is_open())
     {   
        while(std::getline(streamMem,line))
          {
           std::istringstream stream(line);
           stream >> namedescription ;
              if (namedescription.compare("MemTotal:")==0) 
               { 
                 stream >> MemTotal >> sizedescripter; 
                 MemTotalfound = true;
                         if (sizedescripter.compare("mB"))
                              { MemTotal*=1024;}
                         else if ( sizedescripter.compare("gB"))
                              { MemTotal*=1024*1024;}
               }
             
             if (namedescription.compare("MemAvailable:")==0) 
               { 
                 stream >> MemAvil >> sizedescripter; 
                 MemAvilfound= true;
                         if (sizedescripter.compare("mB"))
                              { MemAvil*=1024; }
                         else if ( sizedescripter.compare("gB"))
                              { MemAvil*=1024*1024; }
               }
             
              if (MemTotalfound && MemAvilfound)
                  { break;}
          }
          
       if (MemTotalfound==false || MemAvilfound==false )
         {throw (" MemTotal or MemAvailable not found");}

       MemUtil=((MemTotal - MemAvil)/MemTotal);
        return MemUtil;
   }
    else throw ("proc/Meminfo file not accessible");  
}
  

// ~TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{ 
  long suspend, idle;
  bool suspendFound=false, idleFound=false;
  string line;
   std::ifstream stream(kProcDirectory + kUptimeFilename);
      if (stream.is_open())
      {
            while (std::getline(stream, line))
             {
              std::istringstream stream(line);
              stream >> suspend >> idle;  
              suspendFound=true;
              idleFound=true;
                  if (suspendFound && idleFound)  {break;}
             }
             return suspend;
       }
  
     else throw ("proc/uptime file not accessible");
}

//~TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{ 
  // long jiffies;
   return LinuxParser::UpTime();// sysconf(_SC_CLK_TCK);
  
}

//~TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid_)
{
 long sum=0, utime=0, cstime=0;
 int x=1;
  std::ifstream upstream (kProcDirectory + to_string (pid_) + kStatFilename);
   if (upstream.is_open())
   {
    std::istream_iterator<long> begin(upstream);
    std::istream_iterator<long>eof;
      while ((begin!=eof) && (x<=18))
      {
           if (x==14)
           utime = *begin;

           else if (x==17)
           cstime = *begin;   
           begin++;
           x++; 
       }

       return sum=utime + cstime;
   }
   else throw ("/proc/[pid]/stat not accessible");
  
}
  

//~TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() 
{
  string line, cpu;
  long ActiveJiff, user, nice, system, idle, iowait, irq, softriq, steal, guest, guest_nice;
  std::ifstream actstream(kProcDirectory + kStatFilename);
 if (actstream.is_open())
     {
         while (std::getline(actstream, line))
         {
            std::istringstream stream(line);
            stream >> cpu;
            if (cpu.compare("cpu")) 
            {
               stream >> user >> nice >> system >> idle >> iowait >> irq >> softriq >> steal >> guest >> guest_nice;
               return  ActiveJiff=(user + nice + system + idle + iowait + irq + softriq + steal+ guest + guest_nice);
            }  
        
         }
        throw (" Running Processes not found");
     }
      else throw ("proc/meminfo file not accessible");
  
}

//~TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() //{ return 0; }
{
  long idle, iowait, idleJiff=0;
  string line;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
     if (stream.is_open())
     
     {
         std::getline(stream,line);
         const char *cline=line.c_str();
         sscanf(cline,"%*s:%*d:%*d:%*d:%d:%d", idle, iowait);
         idleJiff =idle + iowait;
         return idleJiff;
     }
      else throw("proc/stat not accessible");  

}

//~TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{

 vector<string> cpuUtil;
  string activeJiff, idleJiff;
  activeJiff=to_string(ActiveJiffies());
  idleJiff=to_string(IdleJiffies());
     cpuUtil.push_back(activeJiff);
     cpuUtil.push_back(idleJiff);
            // cpuutil=(1-(idle/systotal_time);
       return cpuUtil;
   
}

//~ TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{ 
  string line;
  bool procAvail=false;
  string key;
  int totalProc;
  std::ifstream stream(kProcDirectory + kStatFilename);
     if (stream.is_open())
     {
         while(std::getline(stream, line))
           {
              std::istringstream totstream(line);
               totstream >> key;
               if (key.compare("processes")) 
               { 
                     totstream >> totalProc;
                     procAvail=true;
                     return totalProc;
               }
           }
           if (procAvail==false)
                {throw ("processes not found");}
     }
     
      else throw ("proc/meminfo file not accessible");
}

//~TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{ 
  int RunPro;
  bool RunAvail=false;
  string key, line;
  std::ifstream runstream(kProcDirectory + kStatFilename);
     if (runstream.is_open())
     {
         while (std::getline(runstream, line))
         {
             std::istringstream runstream(line);
              if (key == "procs_running") 
              runstream >> RunPro;
              RunAvail=true;
              return RunPro;
         }  
         if (RunAvail=false)
             { throw (" Running Processes not found");}
     }
      else throw ("proc/meminfo file not accessible");
  
}
//~TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)  //return string(); }
{
   string command;
   string line;
   std::ifstream commandstream(kProcDirectory + to_string (pid) + kCmdlineFilename);
      if (commandstream.is_open())
      {  
         getline(commandstream, line);
         return line;
      }
      else throw (" the proces cmdline file not accessible ");
 }

//~TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid)
{
  float RamNum;
  string vmsize, line, RamSize;
  bool RamFound=false;
  std::ifstream streamMem(kProcDirectory + to_string(pid) + kStatusFilename);
     if (streamMem.is_open())
      {   
           while (std::getline(streamMem, line))
           {
                std::istringstream stream(line);
                 stream >> vmsize;
                 if (vmsize.compare("VmSize:"))
                 {
                      stream >> RamNum >> RamSize;
                      RamFound=true;
                             if (RamSize.compare("mB"))
                                { RamNum*=1024;}
                              else if (RamSize.compare("gB"))
                                { RamNum*=1024*1024;}         
                 }    
                 if (RamFound==true)
                 {
                      return std::to_string(RamNum);
                      break;
                 }    
              
            }
            
       }
  
    else throw (" proc/pid/status file not accessible");
};

//~TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)
 {  
   string userID;
   string key, line;
   bool uidfound=false;
   std::ifstream uidstream(kProcDirectory + to_string (pid) + kStatusFilename);
      if (uidstream.is_open())
      { 
          while (std::getline(uidstream, line)) 
           {
             std::istringstream dstream(line);
             if (userID.compare ("uid")) 
             dstream >> userID;
             uidfound=true;
             return userID;
             if (uidfound==true) {break;}
           }
           if (uidfound==false)
            {throw ("UID not found");}
      }
      else throw ( "process status file not accessible");
  }

//~TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) //{ return string(); }
{
 string username, line, user, userid, uid;
 bool uidfound=false;
 std::ifstream userstream(kProcDirectory +to_string(pid) + kStatusFilename);
   if (userstream.is_open())
    {
       while (std::getline(userstream,line))
       {
          std::istringstream sstream(line);
          sstream >> user;
          if (user.compare("uid:")) 
            {
               sstream >> uid;
               uidfound=true;
               if (uidfound)
                 {break;}
            }
  
       }
       
    }
    else throw ("/etc/password file is not accessible");
     
      if (uidfound==false) 
      {
        throw (" uid not found");
      }
 
  std::ifstream uidstream(kPasswordPath); //rename
     if (uidstream.is_open())
    {
       while (std::getline(uidstream,line))
       {
         const char *cline=line.c_str();
         sscanf(cline,"%s:%*s:%d:%*s", username, userid);
            if (uid.compare(userid))
            { return username; }   
         }
       
    }
     throw ("could not find username");
   
}

//~TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)    //{ return 0; }
{
int x=1;
 std::ifstream upstream(kProcDirectory + to_string(pid) + kStatFilename);
   if (upstream.is_open())
    {
        std::istream_iterator <long> begin(upstream);
        std::istream_iterator<long>eof();
          while (begin!=eof() && x<=22)
          {
            begin++;
            x++;
          }
              if (begin==eof())
                 { throw ("UpTime not found"); }
        
          return *begin;
    }
    else throw ("process stat file not accessible");
}
