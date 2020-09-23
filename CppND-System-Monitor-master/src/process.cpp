#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include "processor.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {   
    pid_ = pid;
    user_ =    LinuxParser::User(pid_);
    command_ = LinuxParser::Command(pid_);
    ram_=   LinuxParser::Ram(pid_);
    uptime_= LinuxParser::UpTime();
    //cpuUtilization_= Processor::Utilization();
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  std::vector <std::string> utilization = LinuxParser::CpuUtilization();
  float sum = 0.0;
  for (int i = 0;i < 8; i++) {
    sum += stof(utilization[i]);
  }
  cpuUtilization_ = sum;
  return cpuUtilization_; 
}

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return this->pid_ < a.pid_; 
}