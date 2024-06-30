#include <err.h>

#include <MPerf/Core.hpp>
#include <MPerf/Backends/CPPChrono.hpp>
#include <stdexcept>

namespace MPerf {

using _Measure = CPPChrono::_Measure;

_Measure::_Measure(std::string label) {
  if (label != "time")
    throw std::invalid_argument("CPPChrono only supports 'time'");

  startTime = cppclock::now();
}

void _Measure::Reset() {
  startTime = cppclock::now();
}

void _Measure::DoMeasure() { stopTime = cppclock::now(); }

json _Measure::GetJSON() {
  json j;
  auto delta = stopTime - startTime;
  j["time"] = delta.count();
  return j;
}

Measure *CPPChrono::MakeMeasure(const std::string label) {
  return new _Measure(label);
  Measure *m = new _Measure(label);
  return m;
}

Measure *CPPChrono::MakeMeasure(const std::vector<std::string> labels) {
  if (labels.size() > 1)
    throw std::invalid_argument("Cannot create multiple measures");
  return MakeMeasure(labels.at(0));
}

} // namespace MPerf
