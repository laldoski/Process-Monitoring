#include "processor.h"

// ~TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
    std::string idletimeS, activetimeS;
    int idletime=0, activetime=0, cpuutil=0;
    vector<string>cpuUtil = LinuxParser::CpuUtilization();
    std::getline(cpustream,line);
    istringstream stream (line);
    string idletimeS = cpuUtil.pop_back();
    string activetimeS = cpuUtil.pop_back();
    idletime =std::stoi(idletimeS);
    activetime =std::stoi(activetimeS);
    cpuutil=(1-(idletime/idletime + activetime)) * 100;
    return cpuutil;
}

   


/*
{
 string cpu;
 long user, nice, system, idle, iowait, irq, softriq, systotal_time, cpuutil;
 std::ifstream procstream(kProcDirectory + kStatFilename);
  if (procstream.is_open())
   {
        while (std::getline(actstream, line))
       {
            std::istringstream stream(line);
            stream >> cpu;
            if (cpu.compare("cpu")) 
            {
             procstream >> user >> nice >> system >> idle >> iowait >> irq << softriq;
             systotal_time = user + nice + system + idle + iowait + irq + softriq;
             cpuutil=(1-(idle/systotal_time)) * 100;
             return cpuutil;
            }
        

        }

   }

*/
