#include "tide.hpp"

#include <nanobind/eigen/dense.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/tuple.h>

#include "perth/tide.hpp"

namespace nb = nanobind;

auto instantiate_tide(nanobind::module_ &m) -> void {
  nb::class_<perth::Perth<double>>(m, "Perth")
      .def(nb::init<std::shared_ptr<perth::TidalModel<double>>, bool>(),
           nb::arg("model"), nb::arg("group_modulations") = false,
           "Initialize Perth with a tidal model and optional group modulations")
      .def("evaluate", &perth::Perth<double>::evaluate, nb::arg("lon"),
           nb::arg("lat"), nb::arg("time"), nb::arg("time_tolerance") = 0.0,
           nb::arg("inference") = nullptr,
           nb::call_guard<nb::gil_scoped_release>());
}
