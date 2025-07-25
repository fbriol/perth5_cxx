#include "tide.hpp"

#include <nanobind/eigen/dense.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/tuple.h>

#include "perth/tide.hpp"

namespace nb = nanobind;

template <typename T>
auto bind_perth(nanobind::module_& m, const char* name) -> void {
  nb::class_<perth::Perth<T>>(m, name)
      .def(nb::init<std::shared_ptr<perth::TidalModel<T>>, bool>(),
           nb::arg("model"), nb::arg("group_modulations") = false,
           "Initialize Tide with a tidal model and optional group modulations")
      .def("evaluate", &perth::Perth<T>::evaluate, nb::arg("lon"),
           nb::arg("lat"), nb::arg("time"), nb::arg("time_tolerance") = 0.0,
           nb::arg("interpolation_type") = std::nullopt,
           nb::arg("num_threads") = 0,
           "Evaluate tidal values at a given longitude, latitude, and time",
           nb::call_guard<nb::gil_scoped_release>())
      .def_prop_ro("tidal_model", &perth::Perth<T>::tidal_model,
                   "Get the tidal model associated with this Perth instance");
}

auto instantiate_tide(nanobind::module_& m) -> void {
  bind_perth<float>(m, "PerthFloat32");
  bind_perth<double>(m, "PerthFloat64");
}
