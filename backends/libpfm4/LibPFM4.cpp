#include <err.h>

#include <MPerf/Backends/LibPFM4.hpp>
#include <MPerf/Core.hpp>
#include <cstring>
#include <format>
#include <perfmon/pfmlib.h>
#include <perfmon/pfmlib_perf_event.h>
#include <stdexcept>

namespace MPerf {

using _Measure = LibPFM4::_Measure;

_Measure::_Measure(std::string label) : label(label) {
  int ret;
  std::memset(&args, 0, sizeof(args));
  std::memset(&attr, 0, sizeof(attr));

  args.attr = &attr;
  args.fstr = &fstr;
  args.size = sizeof(pfm_perf_encode_arg_t);

  // Get attr for perf_event_open
  ret = pfm_get_os_event_encoding(label.c_str(), PFM_PLM0 | PFM_PLM3,
                                  PFM_OS_PERF_EVENT, &args);
  if (ret != PFM_SUCCESS) {
    auto err = std::format("cannot get os event encoding for {}. {}", label,
                           pfm_strerror(ret));
    throw std::invalid_argument(err);
  }

  // Open fd
  attr.exclude_kernel = 1;
  attr.exclude_hv = 1;
  attr.exclude_idle = 1;
  fd = perf_event_open(&attr, 0, -1, -1, 0);
  if (fd < 0) {
    auto err = std::format("cannot open perf event for {}. {}", label, strerror(errno));
    throw std::invalid_argument(err);
  }
}

void _Measure::Reset() {
  if (ioctl(fd, PERF_EVENT_IOC_RESET) < 0) {
    auto err = std::format("cannot reset measure {}. {}", label, strerror(errno));
    throw std::invalid_argument(err);
  }
}

void _Measure::DoMeasure() {
  if (read(fd, &result, sizeof(result)) < 0) {
    auto err = std::format("cannot read measure {}. {}", label, strerror(errno));
    throw std::invalid_argument(err);
  }
}

json _Measure::GetJSON() {
  json j;
  j[label] = result.value;
  return j;
}

LibPFM4::LibPFM4() {
  int ret;
  ret = pfm_initialize();
  if (ret != PFM_SUCCESS) {
    auto err = std::format("cannot initialize libpfm4. {}", pfm_strerror(ret));
    throw std::runtime_error(err);
  }
}

Measure *LibPFM4::MakeMeasure(const std::string label) {
  return new _Measure(label);
}

Measure *LibPFM4::MakeMeasure(const std::vector<std::string> labels) {
  if (labels.size() > 1)
    throw std::invalid_argument("cannot create multiple measures");
  return MakeMeasure(labels.at(0));
}

} // namespace MPerf
