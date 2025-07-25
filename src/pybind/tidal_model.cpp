#include "tidal_model.hpp"

#include <nanobind/eigen/dense.h>
#include <nanobind/stl/complex.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/vector.h>

#include "perth/tidal_model.hpp"

namespace nb = nanobind;

template <typename T>
auto bind_tidal_model(nanobind::module_& m, const char* name) -> void {
  nb::class_<perth::TidalModel<T>>(m, name)
      .def(nb::init<perth::Axis, perth::Axis, bool>(), nb::arg("lon"),
           nb::arg("lat"), nb::arg("row_major") = true,
           "Initialize a tidal model with longitude and latitude axes")
      .def("add_constituent", &perth::TidalModel<T>::add_constituent,
           nb::arg("constituent"), nb::arg("wave"),
           "Add a tidal constituent with its corresponding wave data")
      .def(
          "interpolate",
          [](const perth::TidalModel<T>& self, const double lon,
             const double lat, perth::TideTable& table,
             perth::Accelerator& acc) {
            if (self.size() != acc.size()) {
              throw std::invalid_argument(
                  "The size of the tidal model does not match the "
                  "accelerator.");
            }
            return self.interpolate(lon, lat, table, &acc);
          },
          nb::arg("lon"), nb::arg("lat"), nb::arg("table"), nb::arg("acc"),
          "Interpolate tidal values into a tide table")
      .def("empty", &perth::TidalModel<T>::empty,
           "Check if the model contains any constituents")
      .def("size", &perth::TidalModel<T>::size,
           "Get the number of tidal constituents in the model")
      .def("identifiers", &perth::TidalModel<T>::identifiers,
           "Get the list of constituent identifiers")
      .def("accelerator", &perth::TidalModel<T>::accelerator,
           nb::arg("time_tolerance"),
           "Create an accelerator for efficient repeated interpolations",
           nb::rv_policy::take_ownership)
      .def(
          "shared_from_this",
          [](perth::TidalModel<T>& self) { return self.shared_from_this(); },
          "Get a shared pointer to the tidal model instance");
}

auto instantiate_tidal_model(nanobind::module_& m) -> void {
  // First bind the enums and helper classes
  nb::enum_<perth::Quality>(m, "Quality")
      .value("UNDEFINED", perth::Quality::kUndefined)
      .value("EXTRAPOLATED_1", perth::Quality::kExtrapolated1)
      .value("EXTRAPOLATED_2", perth::Quality::kExtrapolated2)
      .value("EXTRAPOLATED_3", perth::Quality::kExtrapolated3)
      .value("INTERPOLATED", perth::Quality::kInterpolated);

  // Bind the Accelerator class
  nb::class_<perth::Accelerator>(m, "Accelerator")
      .def(nb::init<double, size_t>(), nb::arg("time_tolerance"),
           nb::arg("n_constituents"),
           "Initialize an accelerator with a time tolerance and number of "
           "constituents")
      .def("clear", &perth::Accelerator::clear,
           "Clear the cached interpolated values")
      .def_prop_ro("x1", &perth::Accelerator::x1, "Get the x1 coordinate")
      .def_prop_ro("x2", &perth::Accelerator::x2, "Get the x2 coordinate")
      .def_prop_ro("y1", &perth::Accelerator::y1, "Get the y1 coordinate")
      .def_prop_ro("y2", &perth::Accelerator::y2, "Get the y2 coordinate")
      .def_prop_ro("values", &perth::Accelerator::values,
                   "Get the cached interpolated values");

  // Bind both float and double versions of TidalModel
  bind_tidal_model<float>(m, "TidalModelFloat32");
  bind_tidal_model<double>(m, "TidalModelFloat64");
};
