#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

#include <iostream>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, long hertz) : pid_(pid), hertz_(hertz) {
  string line;

  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kStatFilename);

  getline(stream, line);
  std::istringstream linestream(line);
  std::istream_iterator<string> beg(linestream), end;
  vector<string> statInfo(beg, end);
  
  utime_ = stof(statInfo[13]);
  stime_ = stof(statInfo[14]);
  cutime_ = stof(statInfo[15]);
  cstime_ = stof(statInfo[16]);
  starttime_ = stof(statInfo[21]);
}
// Return this process's ID
int Process::Pid() {
  return pid_;
}

// Return this process's CPU utilization
float Process::CpuUtilization() {
  long uptime = LinuxParser::UpTime();
  float total_time = utime_ + stime_ + cutime_ + cstime_;

  float seconds = uptime - (starttime_ / hertz_);
  float cpu_usage = (total_time / hertz_) / seconds;

  return cpu_usage;
}

// Return the command that generated this process
string Process::Command() {
  return LinuxParser::Command(pid_);
}

// Return this process's memory utilization
string Process::Ram() {
  return LinuxParser::Ram(pid_);
}

// Return the user (name) that generated this process
string Process::User() {
  return LinuxParser::User(LinuxParser::Uid(pid_));
}

// Return the age of this process (in seconds)
long int Process::UpTime() {
  long uptime = LinuxParser::UpTime();
  long seconds = uptime - (starttime_ / hertz_);
  return seconds;
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process &a) {
  return CpuUtilization() < a.CpuUtilization();
}