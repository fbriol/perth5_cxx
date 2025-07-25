from typing import Annotated, TypeAlias
from numpy.typing import NDArray
import numpy

from ._core import (
    Constituent,
    InterpolationType,
    Quality,
    Accelerator,
    PerthFloat32,
    PerthFloat64,
    TidalModelFloat32,
    TidalModelFloat64,
)

from .model import load_model

VectorDateTime64: TypeAlias = Annotated[NDArray[numpy.datetime64], "[m, 1]"]
VectorFloat64: TypeAlias = Annotated[NDArray[numpy.float64], "[m, 1]"]
VectorInt8: TypeAlias = Annotated[NDArray[numpy.int8], "[m, 1]"]

LINEAR_ADMITTANCE: InterpolationType = InterpolationType.LINEAR_ADMITTANCE
FOURIER_ADMITTANCE: InterpolationType = InterpolationType.FOURIER_ADMITTANCE

UNDEFINED: Quality = Quality.UNDEFINED
INTERPOLATED: Quality = Quality.INTERPOLATED
EXTRAPOLATED_1: Quality = Quality.EXTRAPOLATED_1
EXTRAPOLATED_2: Quality = Quality.EXTRAPOLATED_2
EXTRAPOLATED_3: Quality = Quality.EXTRAPOLATED_3

__all__ = [
    "EXTRAPOLATED_1",
    "EXTRAPOLATED_2",
    "EXTRAPOLATED_3",
    "FOURIER_ADMITTANCE",
    "INTERPOLATED",
    "LINEAR_ADMITTANCE",
    "UNDEFINED",
    "Accelerator",
    "Constituent",
    "InterpolationType",
    "Perth",
    "Quality",
    "load_model",
]


class Perth:
    def __init__(
        self,
        model: TidalModelFloat32 | TidalModelFloat64,
        group_modulations: bool = False,
    ) -> None:
        self._handler: PerthFloat32 | PerthFloat64
        if isinstance(model, TidalModelFloat32):
            self._handler = PerthFloat32(
                model,
                group_modulations,
            )
        elif isinstance(model, TidalModelFloat64):
            self._handler = PerthFloat64(
                model,
                group_modulations,
            )
        else:
            raise TypeError(
                "Model must be of type TidalModelFloat32 or TidalModelFloat64"
            )

    @property
    def tidal_model(self) -> TidalModelFloat32 | TidalModelFloat64:
        """Return the tidal model used by this Perth instance."""
        return self._handler.tidal_model

    def evaluate(
        self,
        lon: VectorFloat64,
        lat: VectorFloat64,
        time: VectorDateTime64,
        time_tolerance: float = 0.0,
        interpolation_type: InterpolationType | None = None,
    ) -> tuple[VectorFloat64, VectorFloat64, VectorInt8]:
        # Convert datetime64 to microseconds since epoch
        epoch = time.astype("M8[us]").astype("i8")
        return self._handler.evaluate(
            lon,
            lat,
            epoch,
            time_tolerance,
            interpolation_type,
        )
