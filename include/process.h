#ifndef PROCESS_H
#define PROCESS_H

#include "linux_parser.h"
#include <string>

using namespace  LinuxParser;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid_);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process & a) ;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
 int pid_;
};

#endif