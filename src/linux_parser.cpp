#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "format.h"

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
  string version;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: TODO Update this to use std::filesystem
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string memTotal = "MemTotal:";
  string memFree = "MemFree:";
  float Total = findValueByKey<float>(memTotal, kMeminfoFilename);// "/proc/memInfo"
  float Free = findValueByKey<float>(memFree, kMeminfoFilename);
  return (Total - Free) / Total;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string uptimeString;
  std::ifstream filestream(kProcDirectory + kUptimeFilename); // "/proc/uptime"
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptimeString;
  }
  long uptime = stol(uptimeString);
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + kTimerListFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "jiffies") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return stol(value);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> CpuUtilization;
  string line;
  string key;
  string user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> system >> idle >> iowait 
                        >> irq >> softirq >> steal >> guest >>  guest_nice) {
        if (key == "cpu") {
          CpuUtilization = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
          break;
        }
      }
    }
  }
  return CpuUtilization;
}

int LinuxParser::TotalProcesses() {
  return findValueByKey<int>(processes, kStatFilename);
}

int LinuxParser::RunningProcesses() {
  return findValueByKey<int>(running_procs, kStatFilename);
}

string LinuxParser::Command(int pid) {
  return std::string(getValueOfFile<std::string>(std::to_string(pid) + kCmdlineFilename));
}

string LinuxParser::Ram(int pid) {
  float memInKB = LinuxParser::findValueByKey<float>(LinuxParser::vm_data,
                  to_string(pid) + LinuxParser::kStatusFilename);
  return Format::KBinMB(memInKB);
}

// TODO: Read and return the user ID associated with a process
int LinuxParser::Uid(int pid) {
  return LinuxParser::findValueByKey<int>("Uid:",
                                          std::to_string(pid) +
                                          LinuxParser::kStatusFilename);
}


string LinuxParser::User(int UID) {
  /**
   * Not using generic "LinuxParser::findValueByKey()" here due to specif formatting of /ete/passwd
   */
  string line, username, x;
  int fileUID;
  // Parse /etc/passwd
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> x >> fileUID) {
        if (fileUID == UID) {
          return username;
        }
      }
    }
  }
  return username;
  }

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  return 0;
}