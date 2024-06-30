#ifndef LIBPFM4_BACKEND_HPP
#define LIBPFM4_BACKEND_HPP

#include <MPerf/Core.hpp>
#include <perfmon/pfmlib_perf_event.h>

namespace MPerf {

class LibPFM4 : public Backend {
public:
  class _Measure : public Measure {
  private:
    std::string label;
    perf_event_attr attr;
    pfm_perf_encode_arg_t args;
    char *fstr;
    int id, fd;
    struct ReadFormat {
      uint64_t value;        /* The value of the event */
      uint64_t time_enabled; /* if PERF_FORMAT_TOTAL_TIME_ENABLED */
      uint64_t time_running; /* if PERF_FORMAT_TOTAL_TIME_RUNNING */
      uint64_t id;           /* if PERF_FORMAT_ID */
      uint64_t lost;         /* if PERF_FORMAT_LOST */
    } result;

  public:
    _Measure(const std::string label);
    void Reset();
    void DoMeasure();
    json GetJSON();
    std::vector<std::string> GetLabels() { return {label}; };
  };

public:
  LibPFM4();
  Measure *MakeMeasure(const std::string label);
  Measure *MakeMeasure(const std::vector<std::string> labels);
};

} // namespace MPerf
#endif
