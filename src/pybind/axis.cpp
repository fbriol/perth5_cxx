#include "axis.hpp"

#include <nanobind/eigen/dense.h>

#include "perth/axis.hpp"

namespace nb = nanobind;

auto instantiate_axis(nanobind::module_ &m) -> void {
  nb::class_<perth::Axis>(m, "Axis",
                          "Represents an axis, which is a mathematical object "
                          "that relates the coordinate positions in a vector "
                          "to the physical values represented by the axis.")
      .def(nb::init<const Eigen::Ref<const Eigen::VectorXd> &, double, bool>(),
           nb::arg("points"), nb::arg("epsilon") = 1e-6,
           nb::arg("is_periodic") = false)
      .def(nb::init<double, double, double, double, bool>(), nb::arg("start"),
           nb::arg("end"), nb::arg("step"), nb::arg("epsilon") = 1e-6,
           nb::arg("is_periodic") = false)
      .def("__str__", [](const perth::Axis &axis) {
        return "Axis(start: " + std::to_string(axis.start()) +
               ", end: " + std::to_string(axis.end()) +
               ", size: " + std::to_string(axis.size()) +
               ", is_periodic: " + (axis.is_periodic() ? "True" : "False") +
               ")";
      });
}
