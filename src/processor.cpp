#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string> 
#include <istream>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <cstdlib>
#include <thread>


//DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
      long idleJiff_old, idleJiff_now, activeJiff_old, activeJiff_now;
      float activeJiff_Delta, idleJiff_Delta, activeTime, idleTime, cpuutil;    
      idleJiff_old = LinuxParser::IdleJiffies();
      activeJiff_old = LinuxParser::ActiveJiffies(); 
      while(true){
          std::this_thread::sleep_for(std::chrono::seconds(3));
          idleJiff_now = LinuxParser::IdleJiffies();
          activeJiff_now = LinuxParser::ActiveJiffies();
          idleJiff_Delta = abs(idleJiff_now - idleJiff_old);
          activeJiff_Delta = abs(activeJiff_now - activeJiff_old);
          idleTime = idleJiff_Delta / sysconf(_SC_CLK_TCK);
          activeTime = activeJiff_Delta / sysconf(_SC_CLK_TCK);
          cpuutil = (1-(idleTime / (idleTime + activeTime)));
          idleJiff_old = idleJiff_now;
          activeJiff_old = activeJiff_now;
          return cpuutil;      
     }
}

      
         