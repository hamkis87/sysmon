#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  float memUtilization, memTotal, memFree;
  string line, key;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream1(line);
    linestream1 >> key >> memTotal;

    std::getline(stream, line);
    std::istringstream linestream2(line);
    linestream2 >> key >> memFree;

    memUtilization = (memTotal - memFree) / memTotal; 
    return memUtilization;
  }
  return 0.0; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long upTime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::replace(line.begin(), line.end(), '.', ' ');
    std::istringstream linestream(line);
    linestream >> upTime;
    return upTime;
  }
  return 0; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return sysconf(_SC_CLK_TCK) * LinuxParser::UpTime(); 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  long activeJiffies;
  int startId = 13;
  int endId = 16;
  int i = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
      if (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        long token;
        while (i < startId) {
          linestream >> token;
          i++;
        }
        while (i <= endId) {
          linestream >> token;
          activeJiffies += token;
          i++;
        }
      return activeJiffies;
    }
  }
  return 0; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line;
  long activeJiffies = 0;
  int count = 10;
  int i = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      long token;
      string cpuName;
      linestream >> cpuName;
      while (i < count) {
        linestream >> token;
        activeJiffies += token;
        i++;
      }
      return activeJiffies;
    }
  }
  return 0; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line;
  long idleJiffies = 0;
  int startId = 4;
  int i = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      long token;
      string ignore;
      while (i < startId) {
        linestream >> ignore;
        i++;
      }
      while (i <= startId + 1) {
        linestream >> token;
        idleJiffies += token;
        i++;
      }
      return idleJiffies;    
    }
  }
  return 0; 
}

// TODO: Read and return CPU utilization
// float LinuxParser::CpuUtilization() {
//   float activeJiffies = ActiveJiffies();
//   float idleJiffies = IdleJiffies();
//   if (activeJiffies + idleJiffies > 0.0f) {
//     return activeJiffies / (activeJiffies + idleJiffies);
//   }
//   return 0.0f; 
// }
 
std::vector <std::string> LinuxParser::CpuUtilization() {
  string line;
  std::vector <std::string> result;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string cpuName, util_str;
      linestream >> cpuName;
      while (linestream >> util_str) {
        result.push_back(util_str);;
      }
    }
  }
  return result;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key, line;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if ((linestream >> key) && (key == "processes")) {
        linestream >> value;
        return value; 
      }
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, line;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if ((linestream >> key) && (key == "procs_running")) {
        linestream >> value;
        return value; 
      }
    }
  }
  return 0; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
      return line; 
    }
  }
  return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string key, line, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if ((linestream >> key) && (key == "VmSize:")) {
        linestream >> value;
        return to_string(stoi(value)/1000); 
      }
    }
  }
  return string(); 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string key, line, value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      if ((linestream >> key) && (key == "Uid:")) {
        linestream >> value;
        return value; 
      }
    }
  }
  return string(); 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line, key, value, x, uid;
  uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> x >> value; 
      if (value == uid) {
        return key;      
      }
    }
  }  
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, ignore, uid;
  long value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line) && std::getline(filestream, line) ) {
      std::istringstream linestream(line);
      linestream >> ignore >> ignore >> ignore >> ignore >> value; 
      return value;
    }
  } 
  return 0; 
}