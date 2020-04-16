#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
  public:
   float Utilization();  // TODO: See src/processor.cpp

   void PrevUser(float user) { prev_user_ = user; };
   void PrevNice(float nice) { prev_nice_ = nice; };
   void PrevSystem(float system) { prev_system_ = system; };
   void PrevIdle(float idle) { prev_idle_ = idle; };
   void PrevIowait(float iowait) { prev_iowait_ = iowait; };
   void PrevIRQ(float irq) { prev_irq_ = irq; };
   void PrevSoftIRQ(float softirq) { prev_softirq_ = softirq; };
   void PrevSteal(float steal) { prev_steal_ = steal; };

  // TODO: Declare any necessary private members
  private:
    float prev_user_;
    float prev_nice_;
    float prev_system_;
    float prev_idle_;
    float prev_iowait_;
    float prev_irq_;
    float prev_softirq_;
    float prev_steal_;
};

#endif