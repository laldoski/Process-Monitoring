#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string> 
#include <istream>
#include <iostream>

// ~TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{
    std::string idletimeS, activetimeS, line,sumstr;
    int idletime=0, activetime=0, cpuutil=0;
    std::vector<std::string>cpuUtil = LinuxParser::CpuUtilization();
    for (std::string start : cpuUtil)
     { sumstr=sumstr + start;}
    std::istringstream stream(sumstr);
          
    stream >> activetimeS >> idletimeS;

    idletime =std::stoi(idletimeS);
    activetime =std::stoi(activetimeS);
    cpuutil=(1-(idletime/idletime + activetime)) * 100;
    return cpuutil;
};
