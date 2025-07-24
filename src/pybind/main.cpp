#include <nanobind/nanobind.h>

#include "axis.hpp"
#include "constituent.hpp"
#include "inference.hpp"
#include "tidal_model.hpp"
#include "tide.hpp"

NB_MODULE(_core, m) {
  instantiate_axis(m);
  instantiate_constituent(m);
  instantiate_inference(m);
  instantiate_tidal_model(m);
  instantiate_tide(m);
}
