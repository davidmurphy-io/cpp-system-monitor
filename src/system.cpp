#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  bool changed = false;
  // Get id's of current processes
  vector<int> current_pids = LinuxParser::Pids();

  // Add new processes to vector
  for (std::size_t i = 0; i < current_pids.size(); ++i) {
    int current_pid = current_pids[i];  
    if (std::find_if(processes_.begin(), processes_.end(),
                     [current_pid](Process& n) {
                       return n.Pid() == current_pid;
                     }) == processes_.end()) {
      changed = true;
      Process process(current_pids[i], sysconf(_SC_CLK_TCK));  
      processes_.push_back(process);
    }
  }

  // Remove any finished processes from the vector
  for (size_t i = 0; i < processes_.size(); i++) {
    int current_pid = processes_[i].Pid();

    if (std::find(current_pids.begin(), current_pids.end(), current_pid) ==
        current_pids.end()) {
      changed = true;

      processes_.erase(processes_.begin() + i);
    }
  }

  // sort()
  if (changed) {
    std::sort(processes_.begin(), processes_.end(), [](Process &a, Process &b){ return b < a; });
  }

  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() {
  return LinuxParser::Kernel();
}

// Return the system's memory utilization
float System::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}

// Return the operating system name
std::string System::OperatingSystem() {
  return LinuxParser::OperatingSystem();
}

// Return the number of processes actively running on the system
int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}

// Return the total number of processes on the system
int System::TotalProcesses() {
  return LinuxParser::TotalProcesses();
}

// Return the number of seconds since the system started running
long int System::UpTime() {
  return LinuxParser::UpTime();
}