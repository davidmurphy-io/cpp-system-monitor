#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
  public:
    Process(int pid, long hertz);
    int Pid();
    std::string User();
    std::string Command();
    float CpuUtilization();
    std::string Ram();
    long int UpTime();
    bool operator<(Process &a);

  private:
    int pid_;
    long hertz_;
    float utime_ = 0.0;
    float stime_ = 0.0;
    float cutime_ = 0.0;
    float cstime_ = 0.0;
    float starttime_ = 0.0;
};

#endif