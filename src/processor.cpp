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
};
