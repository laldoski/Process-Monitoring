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
#include <iostream>
#include <cctype>
#include <locale>
#include <algorithm>

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

//BONUS
/*vector<int> LinuxParser::Pids() {
    std::ifstream prostream(kProcDirectory);
    vector<string> pidsStr;
    vector<int> pidsNum;
    string value; 
    for (const auto & entry : fs::directory_iterator(kProcDirectory)){
         if (is_directory(entry)==true){ 
            std::istringstream directory(entry.path());
            while (std::getline(directory,value, '/')){
                   if (value.find_first_not_of("0123456789")==std::string::npos){
                       pidsStr.push_back(value);
                       std::transform(pidsStr.begin() , pidsStr.end(), std::back_inserter(pidsNum),
                       [](const std:: string & str) { return std::stoi(str);}) ;
                       return pidsNum;
                    } 
               }
          }
     }
     return {};  
}
 
*/
//DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization(){ 
      bool MemTotalfound=false, MemAvilfound=false;
      float MemTotal, MemAvil, MemUtil;
      string  namedescription, line, sizedescripter;
      std::ifstream streamMem(kProcDirectory + kMeminfoFilename);
      if(streamMem.is_open()) {   
           while(std::getline(streamMem,line)){
                 std::istringstream stream(line);
                 stream >> namedescription;
                 if(namedescription.compare("MemTotal:")==0){ 
                      stream >> MemTotal >> sizedescripter; 
                      MemTotalfound = true;
                      if(sizedescripter.compare("mB"))
                            { MemTotal*=1024; }
                      else if( sizedescripter.compare("gB"))
                            { MemTotal*=1024*1024; }
                    }    
                 if(namedescription.compare("MemAvailable:")==0){ 
                      stream >> MemAvil >> sizedescripter; 
                      MemAvilfound= true;
                      if(sizedescripter.compare("mB"))
                            { MemAvil*=1024; }
                      else if(sizedescripter.compare("gB"))
                            { MemAvil*=1024*1024; }
                    }
                  if (MemTotalfound && MemAvilfound)
                            { break;}
               }
               if(MemTotalfound==false || MemAvilfound==false )
                    { throw (" MemTotal or MemAvailable not found"); }
               MemUtil=((MemTotal - MemAvil)/MemTotal);
               return MemUtil;
          }
         else throw ("proc/Meminfo file not accessible");  
}
  

//DONE: Read and return the system uptime
long LinuxParser::UpTime(){ 
    long suspend, idle;
    bool suspendFound=false, idleFound=false;
    string line;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()){
         while (std::getline(stream, line)) {
               std::istringstream stream(line);
               stream >> suspend >> idle;  
               suspendFound=true;
               idleFound=true;
               if (suspendFound && idleFound) 
                  {break;}
          }
          return suspend; 
    } 
     else throw ("proc/uptime file not accessible");
}

//DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies(){ 
     return (LinuxParser::UpTime() / (sysconf(_SC_CLK_TCK))); 
}


//DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid_){
     long sum=0, utime=0, cstime=0;
     int x=1;
     std::ifstream upstream (kProcDirectory + to_string (pid_) + kStatFilename);
     if (upstream.is_open()) {
           std::istream_iterator<long> begin(upstream);
           std::istream_iterator<long>eos;
           while ((*begin!=upstream.eof()) && (x<=18)) {
                 if (x==14)
                      utime = *begin;
                 else if (x==17)
                      {cstime = *begin;}   
                 begin++;
                 x++; 
               }
        return sum = utime + cstime;
     }
   else throw ("/proc/[pid]/stat not accessible");
   return 0;
}
  

//DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies(){
     string line, cpu;
     long ActiveJiff, user, nice, system, idle, iowait, irq, 
     softriq, steal, guest, guest_nice;
     std::ifstream actstream(kProcDirectory + kStatFilename);
     if (actstream.is_open()){
           while (std::getline(actstream, line)){
                  std::istringstream stream(line);
                  stream >> cpu;
                  if (cpu.compare("cpu")) {
                        stream >> user >> nice >> system >> idle >> 
                        iowait >> irq >> softriq >> steal >> guest >> guest_nice;
                        ActiveJiff = user + nice + system + idle + iowait + 
                        irq + softriq + steal+ guest + guest_nice;
                        return ActiveJiff;
                    }    
              }
          throw (" Running Processes not found");
     } 
     else throw ("proc/meminfo file not accessible");
     return 0;
}

//DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
     long idle=0, iowait=0, idleJiff=0;
     string line;
     std::ifstream stream(kProcDirectory + kStatFilename);
     if (stream.is_open()) {
         std::getline(stream,line);
         const char *cline=line.c_str();
         sscanf(cline,"%*s %*d %*d %*d %ld %ld",&idle, &iowait);
         idleJiff =idle + iowait;
         return idleJiff;
     } 
     else throw("proc/stat not accessible");
     return 0;  
}


//DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization(){
     vector<string> cpuUtil;
     string activeJiff, idleJiff;
     activeJiff=to_string(ActiveJiffies());
     idleJiff=to_string(IdleJiffies());
     cpuUtil.push_back(activeJiff);
     cpuUtil.push_back(idleJiff);      
     return cpuUtil;  
}

//DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses(){ 
     bool procAvail=false;
     string line, key, totalProc;
     std::ifstream stream(kProcDirectory + kStatFilename);
     if(stream.is_open()){
         while(std::getline(stream, line)){
              std::istringstream totstream(line);
              totstream >> key;
              if(key.compare("processes")==0) { 
                     totstream >> totalProc;
                     procAvail=true;
                     if (totalProc!="")
                        {return stoi(totalProc);}
               }
          }
           if(procAvail==false)
              {throw ("processes not found");}
     }
      else throw ("proc/meminfo file not accessible");
      return 0;
}

//DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses(){ 
     bool RunAvail=false;
     string key, line, RunPro;
     std::ifstream runstream(kProcDirectory + kStatFilename);
     if (runstream.is_open()) {
         while (std::getline(runstream, line)){
              std::istringstream runstream(line);
              runstream >> key;
              if (key.compare("procs_running")==0){ 
                   runstream >> RunPro;
                   RunAvail=true; 
                   if (RunPro!="")
                      {return stoi(RunPro);}
               }
          }  
          if(RunAvail==false)
           { throw (" Running Processes not found");}
     }
      else throw ("proc/meminfo file not accessible");
      return 0;
}

//DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
     string command;
     string line;
     std::ifstream commandstream(kProcDirectory + to_string (pid) +
     kCmdlineFilename);
     if (commandstream.is_open()){  
          getline(commandstream, line);
          return line;
     }
      else throw (" the proces cmdline file not accessible ");
}

//DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
   int RamNum=0;
   string vmsize, line, RamSize;
   bool RamFound=false;
   std::ifstream streamMem(kProcDirectory + to_string(pid) + kStatusFilename);
   if (streamMem.is_open()) {   
       while (std::getline(streamMem, line)){
              std::istringstream stream(line);
               stream >> vmsize;
                 if (vmsize.compare("VmSize:")==0) { 
                    stream >> RamNum >> RamSize; 
                    RamFound=true;
                       if (RamSize.compare("kB")==0)
                            { RamNum=RamNum/1024; }
                       else if (RamSize.compare("gB")==0)
                            { RamNum=RamNum*1024; }                       
                 }    
                 if (RamFound==true) 
                    { return  std::to_string(RamNum); }           
          }        
     }   
     else throw (" Ram size not found ");
     return "0";
}


//DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid){  
   string userID;
   string key, line;
   bool uidfound=false;
   std::ifstream uidstream(kProcDirectory + to_string (pid) + kStatusFilename);
      if(uidstream.is_open()){ 
          while(std::getline(uidstream, line)) {
             std::istringstream dstream(line);
             if(userID.compare("uid:")) {
                  dstream >> userID;
                  uidfound=true;
                  return userID;
                  break;
               }
             if(uidfound==false)
                  {throw ("UID not found");}
          }
     }
    return "0";
}

//DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
    string username_string, line, user,userid_string, uid, userid_check;
    char username[20]={0}, userid[20]={0};
    std::ifstream uidstream(kPasswordPath);
    if(uidstream.is_open()) {
       while(std::getline(uidstream,line)){
             const char *cline=line.c_str();
             std::ofstream myfile;
             sscanf(cline, "%[^':']:%*[^':']:%[^':']:%*[^':']", username, userid);
             username_string = username;
             myfile << "username_string:" << username_string <<std::endl;
             userid_check = uid;
             userid_string = LinuxParser::Uid(pid);
             if (userid_string.compare(userid_check))
                 { return username_string; }   
          }    
     }  
     throw ("could not find username");
     return "0";
}


//DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
     int location=1;                                         
     string uptimeS;
     long uptime=0;
     std::ifstream upstream(kProcDirectory + to_string(pid) + kStatFilename);
     if (upstream.is_open()){
         std::istream_iterator<string>begin(upstream);
         std::istream_iterator<string>eos;
         while ((begin!=eos) && (location<=22)){
               if (location==22) {
                  uptimeS = *begin; 
                  break;
               }   
                else {
                   begin++;
                   location++;
               }
          }
     }     
         if (uptimeS!=""){
               uptime=std::stol(uptimeS);
               return (uptime/sysconf(_SC_CLK_TCK)); 
         }
         else return 0;    
}


