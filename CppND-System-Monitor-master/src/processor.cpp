#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
      std::string line, cpu, value;
  std::vector <std::string> utilization = LinuxParser::CpuUtilization();
  
  float user = stof(utilization[0]);// These are the currently 
  float nice = stof(utilization[1]);// extracted value from the
  float system = stof(utilization[2]);//system
  float idle = stof(utilization[3]);
  float iowait = stof(utilization[4]);
  float irq = stof(utilization[5]);
  float softirq = stof(utilization[6]);
  float steal = stof(utilization[7]);
  float PrevIdle = previdle + previowait;
  float Idle = idle + iowait;
  
  float PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
  float NonIdle = user + nice + system + irq + softirq + steal;
  
  float PrevTotal = PrevIdle + PrevNonIdle;
  float Total = Idle + NonIdle;
  
  float totald = Total - PrevTotal;
  float idled = Idle - PrevIdle;
  
  //Final output
  float output = (totald - idled) / totald;
  
  // Once you are done with the final output what you will do 
  // is that you will set the values of "prev_*" variables 
  // the values of current values which will become previous
  // values in the next iteration
  previdle = idle;
  previowait = iowait;
  prevuser = user;
  prevnice = nice;
  prevsystem = system;
  previrq = irq;
  prevsoftirq = softirq;
  prevsteal = steal;
  
  return output;
}