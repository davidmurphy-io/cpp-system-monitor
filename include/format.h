#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string Format(int time);
std::string ElapsedTime(long times);
std::string KBinMB(float kb);
};

#endif