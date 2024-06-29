#ifndef CPPCHRONO_BACKEND_HPP
#define CPPCHRONO_BACKEND_HPP

#include <MPerf/Core.hpp>
#include <chrono>

namespace MPerf {

class CPPChrono : public Backend {
public:
  class _Measure : public Measure {
  private:
    std::string label;
    using cppclock = std::chrono::steady_clock;
    using time_point = std::chrono::time_point<cppclock, cppclock::duration>;
    using display_unit = std::chrono::nanoseconds;
    time_point stopTime, startTime;

  public:
    _Measure(const std::string label);
    void Reset();
    void DoMeasure();
    json GetJSON();
    std::vector<std::string> GetLabels() { return {label}; };
  };

public:
  Measure *MakeMeasure(const std::string label);
  Measure *MakeMeasure(const std::vector<std::string> labels);
};

} // namespace MPerf
#endif
