#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string> 
#include <istream>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

// ~TODO: Return the aggregate CPU utilization
/*float Processor::Utilization() {
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
         cpuutil=(1-(idleTime/(idleTime + activeTime)) ) ;
         myfile << "cpuutil:"  << cpuutil << std::endl; 
         return cpuutil;

       }   
     
      else return 0; 
}

*/
float Processor::Utilization() {
      std::string S_idleJiff_now, S_idleJiff_prev, S_activeJiff_now, 
      activeJiffS_prev, line;
      float idleJiff_now=0, idleJiff_prev, activeJiff_now=0, idleJiff_Delta=0,
       activeJiff_Delta=0, activeJiff_prev, cpuutil=0, idleTime=0, activeTime=0;
      while (true) {
          std::ofstream myfile;
          myfile.open("update.txt");
          std::vector<std::string>cpuUtil = LinuxParser::CpuUtilization();
          S_idleJiff_now = cpuUtil[0];
          S_activeJiff_now = cpuUtil[1];
          myfile << "S_idleJiff_now: "  << S_idleJiff_now << std::endl;
          myfile << "S_activeJiff_now: "  << S_activeJiff_now << std::endl;
          if ((S_idleJiff_now!="") && (S_activeJiff_now!="")){
             idleJiff_now = std::stof(S_idleJiff_now);
             activeJiff_now = std::stof(S_activeJiff_now); 
             myfile << "idleJiff_now: "  << idleJiff_now << std::endl;
             myfile << "activeJiff_now: "  << activeJiff_now << std::endl;
             myfile << "idleJiff_prev: "  << idleJiff_prev << std::endl;
             myfile << "activeJiff_prev: "  << activeJiff_prev << std::endl;
             idleJiff_Delta = abs(idleJiff_now - idleJiff_prev);
             activeJiff_Delta = abs(activeJiff_now - activeJiff_prev);
             myfile << "idleJiff_Delta: "  << idleJiff_Delta << std::endl;
             myfile << "activeJiff_Delta: "  << activeJiff_Delta << std::endl;
             idleTime = idleJiff_Delta/sysconf(_SC_CLK_TCK);
             activeTime = activeJiff_Delta/sysconf(_SC_CLK_TCK);
             idleJiff_prev = idleJiff_now;
             activeJiff_prev = activeJiff_now;
              myfile << "idleJiff_prev: "  << idleJiff_prev << std::endl;
             myfile << "activeJiff_prev: "  << activeJiff_prev << std::endl;
             cpuutil = (1-(idleTime/(idleTime + activeTime)) ) ;
             return cpuutil;
            }
       sleep(3);
      } 
     
       return 0;
       
}