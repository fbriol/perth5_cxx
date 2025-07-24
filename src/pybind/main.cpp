#include <nanobind/nanobind.h>

#include "axis.hpp"
#include "constituent.hpp"
#include "tidal_model.hpp"

NB_MODULE(_core, m) {
  instantiate_axis(m);
  instantiate_constituent(m);
  instantiate_tidal_model(m);
}
