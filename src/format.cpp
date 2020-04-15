#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

// Output string to 2 place precision
string Format::Format(int time) {
  string timeAsString = std::to_string(time);
  return string(2 - timeAsString.length(), '0') + timeAsString;
}

// Convert seconds to string HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hour = 0;
  int min = 0;
  int sec = 0;

  hour = seconds / 3600;
  seconds = seconds % 3600;
  min = seconds / 60;
  seconds = seconds % 60;
  sec = seconds;

  return Format(hour) + ':' + Format(min) + ':' + Format(sec);
}

// Convert float in KB to string in MB
string Format::KBinMB(float kb) {
  float mb = kb / 1024;
  std::stringstream mb_stream;
  mb_stream << std::fixed << std::setprecision(1) << mb;
  return mb_stream.str();
}