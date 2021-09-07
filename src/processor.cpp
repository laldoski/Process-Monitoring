#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string> 
#include <istream>
#include <iostream>
#include <unistd.h>

// ~TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
      std::string idleJiffS, activeJiffS, line;
      float idleJiff=0, activeJiff=0, cpuutil=0, idleTime=0, activeTime=0;
      std::ofstream myfile;
      myfile.open("processor.txt");
      std::vector<std::string>cpuUtil = LinuxParser::CpuUtilization();
      idleJiffS = cpuUtil[0];
      activeJiffS = cpuUtil[1];
      myfile << "activeJiffS(string):" << activeJiffS << std::endl;
      myfile << "idleJiffS(string):"  << idleJiffS << std::endl;
      if ((idleJiffS!="")&&(activeJiffS!="")){
         idleJiff = std::stof(idleJiffS);
         activeJiff = std::stof(activeJiffS);
         myfile << "activeJiff(float):"  << activeJiff << std::endl;  
         myfile << "idleJiff(float):"  << idleJiff << std::endl;
         idleTime = idleJiff/sysconf(_SC_CLK_TCK);
         activeTime = activeJiff/sysconf(_SC_CLK_TCK);
         myfile << "activeTime(float):"  << activeTime << std::endl;  
         myfile << "idleTime(float):"  << idleTime << std::endl;
         cpuutil=(1-(idleTime/(idleTime + activeTime))) * 100;
         myfile << "cpuutil:"  << cpuutil << std::endl; 
         return cpuutil;

       }   
     
      else return 0; 
}
