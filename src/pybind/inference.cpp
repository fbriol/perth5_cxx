#include "inference.hpp"

#include <nanobind/nanobind.h>

#include "perth/inference.hpp"

namespace nb = nanobind;

auto instantiate_inference(nanobind::module_ &m) -> void {
  nb::enum_<perth::InterpolationType>(m, "InterpolationType")
      .value("LINEAR_ADMITTANCE", perth::InterpolationType::kLinearAdmittance)
      .value("FOURIER_ADMITTANCE",
             perth::InterpolationType::kFourierAdmittance);
  nb::class_<perth::Inference>(m, "Inference")
      .def(
          nb::init<const perth::ConstituentTable &, perth::InterpolationType>(),
          nb::arg("components"), nb::arg("interpolation_type"))
      .def("__call__", &perth::Inference::operator(), nb::arg("hc"),
           nb::arg("lat") = 0.0,
           "Perform inference on the constituent table with optional latitude");
}
