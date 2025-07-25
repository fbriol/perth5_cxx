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
    """A tidal analysis and prediction engine.

    Perth provides high-performance tidal constituent evaluation using either
    float32 or float64 precision tidal models.

    Args:
        model: A TidalModelFloat32 or TidalModelFloat64 instance containing
            tidal harmonic data for predictions.
        group_modulations: If True, applies nodal modulations to grouped
            constituents. Default is False. Only enable if you understand
            the tidal harmonic analysis implications.
    """

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

    def evaluate(  # noqa: PLR0913
        self,
        lon: VectorFloat64,
        lat: VectorFloat64,
        time: VectorDateTime64,
        *,
        time_tolerance: float = 0.0,
        interpolation_type: InterpolationType | None = None,
        num_threads: int = 0,
    ) -> tuple[VectorFloat64, VectorFloat64, VectorInt8]:
        """Evaluate tidal constituents at specified locations and times.

        Args:
            lon: Longitudes in degrees, shape [m, 1].
            lat: Latitudes in degrees, shape [m, 1].
            time: Timestamps as numpy.datetime64, shape [m, 1].
            time_tolerance: Maximum time difference in seconds before
                recomputing lunisolar fundamental arguments and nodal
                corrections. If 0, these values are recomputed for each call
                unless the timestamp is identical to the previous call.
            interpolation_type: Method for constituent inference:
                - None: No inference; only input constituents used for
                  prediction
                - LINEAR_ADMITTANCE: Piecewise linear interpolation of
                  admittances
                - FOURIER_ADMITTANCE: Munk-Cartwright Fourier interpolation

                Inference requires minimum constituents: Q1, O1, K1, N2, M2, S2
                for short period; Mf, Mm for long period; and 18.6-year node
                tide.
            num_threads: Number of threads for parallel computation. If 0, uses
            all available CPU cores.

            .. note::

                If the 18.6-year node tide is not provided in the input,
                a simple equilibrium model is used as a substitute.

        Returns:
            A tuple containing:
            - Ocean tide heights in meters, shape [m, 1]
            - Long period tide heights in meters, shape [m, 1]
            - Quality flags indicating interpolation/extrapolation status,
              shape [m, 1]
        """
        # Convert datetime64 to microseconds since epoch
        epoch = time.astype("M8[us]").astype("i8")
        return self._handler.evaluate(
            lon,
            lat,
            epoch,
            time_tolerance,
            interpolation_type,
            num_threads,
        )
