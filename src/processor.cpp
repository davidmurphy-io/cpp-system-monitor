#include <string>
#include <vector>

#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<std::string> cpu_utilisation = LinuxParser::CpuUtilization();

  float curr_user = stof(cpu_utilisation[0]);
  float curr_nice = stof(cpu_utilisation[1]);
  float curr_system = stof(cpu_utilisation[2]);
  float curr_idle = stof(cpu_utilisation[3]);
  float curr_iowait = stof(cpu_utilisation[4]);
  float curr_irq = stof(cpu_utilisation[5]);
  float curr_softirq = stof(cpu_utilisation[6]);
  float curr_steal = stof(cpu_utilisation[7]);

  float prevIdle = prev_idle_ + prev_iowait_;
  float idle = curr_idle + curr_iowait;

  float prevNonIdle =
      prev_user_ + prev_nice_ + prev_system_ + prev_irq_ + prev_softirq_ + prev_steal_;
  float nonIdle = curr_user + curr_nice + curr_system + curr_irq + curr_softirq + curr_steal;

  float prevTotal = prevIdle + prevNonIdle;
  float total = idle + nonIdle;

  double totald = total - prevTotal;

  float idled = idle - prevIdle;

  float CPU_Percentage = (totald - idled) / totald;

  // Update 'prev' values
  PrevUser(curr_user);
  PrevNice(curr_nice);
  PrevSystem(curr_system);
  PrevIdle(curr_idle);
  PrevIowait(curr_iowait);
  PrevIRQ(curr_irq);
  PrevSoftIRQ(curr_softirq);
  PrevSteal(curr_steal);

  return CPU_Percentage;
}