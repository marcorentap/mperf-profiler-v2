#ifndef MPERF_CORE_HPP
#define MPERF_CORE_HPP

#include <MPerf/Lib/Json.hpp>
#include <vector>

namespace MPerf {

using json = nlohmann::json;

class Measure {
public:
  virtual void Reset() = 0;
  virtual void DoMeasure() = 0;
  virtual json GetJSON() = 0;
  // Probably redundant, probably should get labels from json
  virtual std::vector<std::string> GetLabels() = 0;
};

class Backend {
  const std::string name;
  const std::vector<std::string> labels;
  public:
  virtual Measure *MakeMeasure(const std::string label) = 0;
  virtual Measure *MakeMeasure(const std::vector<std::string> labels) = 0;
};

} // namespace MPerf
#endif
