from collections.abc import Callable
import os
from typing import NamedTuple
import netCDF4
import numpy
import dataclasses


from . import _core

#: Conversion factors for metric units
METRIC = {"m": 1.0, "km": 1000.0, "cm": 0.01, "mm": 0.001}

#: Conversion functions for angular units
ANGULAR: dict[str, Callable[[numpy.ndarray], numpy.ndarray]] = {
    "rad": lambda x: x,
    "deg": lambda x: numpy.radians(x),
}


def _convert_to_meters(values: numpy.ndarray, units: str) -> numpy.ndarray:
    """Convert values to meters based on the specified units."""
    if units not in METRIC:
        raise ValueError(f"Unknown units: {units}")
    return values * METRIC[units]


def _convert_to_radians(values: numpy.ndarray, units: str) -> numpy.ndarray:
    """Convert values to radians based on the specified units."""
    if units.lower() in ["degree", "degrees", "deg"]:
        units = "deg"
    elif units.lower() in ["radian", "radians", "rad"]:
        units = "rad"
    if units not in ANGULAR:
        raise ValueError(f"Unknown units: {units}")
    return ANGULAR[units](values)


class ModelMetadata(NamedTuple):
    """Metadata extracted from the first constituent dataset."""

    amplitude_units: str
    phase_units: str
    dtype: numpy.dtype
    shape: tuple[int, ...]
    row_major: bool
    x_axis: numpy.ndarray
    y_axis: numpy.ndarray


@dataclasses.dataclass
class VariableNames:
    """Variable names used in the netCDF datasets."""

    latitude: str = "latitude"
    longitude: str = "longitude"
    amplitude: str = "amplitude"
    phase: str = "phase"


def _get_variable_unit(var: netCDF4.Variable) -> str:
    """Extract units from a netCDF variable."""
    if not hasattr(var, "units"):
        raise ValueError(
            f"Variable {var.name} does not have a 'units' attribute."
        )
    return var.units


def _validate_required_variables(
    dataset: netCDF4.Dataset, var_names: VariableNames, constituent_name: str
) -> None:
    """Validate that all required variables exist in the dataset."""
    required_vars = [
        var_names.latitude,
        var_names.longitude,
        var_names.amplitude,
        var_names.phase,
    ]

    for var_name in required_vars:
        if var_name not in dataset.variables:
            raise RuntimeError(
                f"Variable {var_name} not found in dataset for "
                f"constituent {constituent_name}."
            )


def _validate_amplitude_phase_consistency(
    amp: netCDF4.Variable, ph: netCDF4.Variable, constituent_name: str
) -> None:
    """Validate that amplitude and phase variables have consistent shapes."""
    if amp.shape != ph.shape:
        raise RuntimeError(
            f"Amplitude and phase variables must have the same shape, "
            f"but found {amp.shape} and {ph.shape} for "
            f"constituent {constituent_name}."
        )


def _extract_initial_metadata(
    dataset: netCDF4.Dataset, var_names: VariableNames
) -> ModelMetadata:
    """Extract metadata from the first constituent dataset."""
    lon = dataset.variables[var_names.longitude]
    lat = dataset.variables[var_names.latitude]
    amp = dataset.variables[var_names.amplitude]
    ph = dataset.variables[var_names.phase]

    dtype = max(amp.dtype, ph.dtype)
    amp_units = _get_variable_unit(amp)
    ph_units = _get_variable_unit(ph)
    row_major = amp.shape[0] == lon.shape[0]

    return ModelMetadata(
        amplitude_units=amp_units,
        phase_units=ph_units,
        dtype=dtype,
        shape=amp.shape,
        row_major=row_major,
        x_axis=lon[:],
        y_axis=lat[:],
    )


def _validate_consistency_with_metadata(
    dataset: netCDF4.Dataset,
    var_names: VariableNames,
    metadata: ModelMetadata,
    constituent_name: str,
) -> numpy.dtype:
    """Validate that the current dataset is consistent with the reference
    metadata."""
    lon = dataset.variables[var_names.longitude]
    lat = dataset.variables[var_names.latitude]
    amp = dataset.variables[var_names.amplitude]
    ph = dataset.variables[var_names.phase]

    # Check coordinate consistency
    if not numpy.array_equal(lon[:], metadata.x_axis):
        raise RuntimeError(
            f"Longitude variable must have the same values for all "
            f"constituents, but found different values for "
            f"constituent {constituent_name}."
        )

    if not numpy.array_equal(lat[:], metadata.y_axis):
        raise RuntimeError(
            f"Latitude variable must have the same values for all "
            f"constituents, but found different values for "
            f"constituent {constituent_name}."
        )

    # Check shape consistency
    if metadata.shape != amp.shape:
        raise RuntimeError(
            f"All constituents must have the same shape, "
            f"but found {amp.shape} for constituent "
            f"{constituent_name} and {metadata.shape} for previous "
            "constituents."
        )

    # Check row-major consistency
    current_row_major = amp.shape[0] == lon.shape[0]
    if metadata.row_major != current_row_major:
        raise RuntimeError(
            f"All constituents must have the same row-major "
            f"order, but found {current_row_major} for "
            f"constituent {constituent_name} and {metadata.row_major} for "
            "previous constituents."
        )

    # Check units consistency
    if metadata.amplitude_units != _get_variable_unit(amp):
        raise RuntimeError(
            f"All amplitude variables must have the same units, "
            f"but found {_get_variable_unit(amp)} for constituent "
            f"{constituent_name} and {metadata.amplitude_units} for previous "
            "constituents."
        )

    if metadata.phase_units != _get_variable_unit(ph):
        raise RuntimeError(
            f"All phase variables must have the same units, "
            f"but found {_get_variable_unit(ph)} for constituent "
            f"{constituent_name} and {metadata.phase_units} for previous "
            "constituents."
        )

    # Return updated dtype (max of current and previous)
    return max(metadata.dtype, amp.dtype, ph.dtype)


def _create_axis(
    values: numpy.ndarray, epsilon: float, is_periodic: bool = False
) -> _core.Axis:
    """Create an Axis object with filled values and specified properties."""
    if values.dtype == numpy.float32:
        # For 32-bit floats, recalculate axis properties to avoid precision
        # loss when dealing with small grid step sizes
        start = values[0].astype(numpy.float64)
        end = values[-1].astype(numpy.float64)
        step = (end - start) / (len(values) - 1)
        return _core.Axis(
            start,
            end,
            step,
            epsilon=epsilon,
            is_periodic=is_periodic,
        )

    return _core.Axis(
        numpy.ma.filled(values, fill_value=numpy.nan),
        epsilon=epsilon,
        is_periodic=is_periodic,
    )


def _create_tidal_model(
    metadata: ModelMetadata, dtype: numpy.dtype
) -> _core.TidalModelFloat32 | _core.TidalModelFloat64:
    """Create the appropriate tidal model based on the data type."""
    x_axis = _create_axis(
        numpy.ma.filled(metadata.x_axis, fill_value=numpy.nan),
        epsilon=1e-6,
        is_periodic=True,
    )
    y_axis = _create_axis(
        numpy.ma.filled(metadata.y_axis, fill_value=numpy.nan),
        epsilon=1e-6,
    )
    if dtype == numpy.float32:
        return _core.TidalModelFloat32(
            x_axis,
            y_axis,
            row_major=metadata.row_major,
        )
    elif dtype == numpy.float64:
        return _core.TidalModelFloat64(
            x_axis,
            y_axis,
            row_major=metadata.row_major,
        )
    else:
        raise ValueError(
            f"Unsupported data type {dtype} for tidal model. "
            "Only float32 and float64 are supported."
        )


def _process_constituent_data(
    dataset: netCDF4.Dataset, var_names: VariableNames, metadata: ModelMetadata
) -> numpy.ndarray:
    """Process amplitude and phase data for a single constituent."""
    amp_raw = numpy.ma.filled(
        dataset.variables[var_names.amplitude][:],
        fill_value=numpy.nan,
    )
    ph_raw = numpy.ma.filled(
        dataset.variables[var_names.phase][:],
        fill_value=numpy.nan,
    )

    # Apply unit conversions
    amp = _convert_to_meters(amp_raw, metadata.amplitude_units)
    ph = _convert_to_radians(ph_raw, metadata.phase_units)

    # Convert to complex wave representation
    return amp * numpy.cos(ph) + 1j * amp * numpy.sin(ph)


def load_model(
    files: dict[_core.Constituent, str],
    *,
    latitude: str | None = None,
    longitude: str | None = None,
    amplitude: str | None = None,
    phase: str | None = None,
) -> _core.TidalModelFloat32 | _core.TidalModelFloat64:
    """
    Load a tidal model from netCDF files.

    Args:
        files: Dictionary mapping constituents to their file paths
        latitude: Name of latitude variable (default: 'latitude')
        longitude: Name of longitude variable (default: 'longitude')
        amplitude: Name of amplitude variable (default: 'amplitude')
        phase: Name of phase variable (default: 'phase')

    Returns:
        Tidal model instance (Float32 or Float64 based on data precision)

    Raises:
        ValueError: If no files provided or unsupported data type
        RuntimeError: If datasets are inconsistent
    """
    if len(files) == 0:
        raise ValueError("No files provided for loading the model.")

    var_names = VariableNames(
        latitude=latitude or "latitude",
        longitude=longitude or "longitude",
        amplitude=amplitude or "amplitude",
        phase=phase or "phase",
    )

    # First pass: validate datasets and extract metadata
    metadata: ModelMetadata | None = None
    dtype: numpy.dtype | None = None

    for constituent, path in files.items():
        if not os.path.exists(path):
            raise FileNotFoundError(
                f"File for constituent {constituent.name} not found: {path}"
            )

        with netCDF4.Dataset(path, "r") as dataset:
            _validate_required_variables(
                dataset,
                var_names,
                constituent.name,
            )

            amp = dataset.variables[var_names.amplitude]
            ph = dataset.variables[var_names.phase]
            _validate_amplitude_phase_consistency(
                amp,
                ph,
                constituent.name,
            )

            if metadata is None:
                metadata = _extract_initial_metadata(
                    dataset,
                    var_names,
                )
                dtype = metadata.dtype
            else:
                dtype = _validate_consistency_with_metadata(
                    dataset,
                    var_names,
                    metadata,
                    constituent.name,
                )

    # At this point, metadata and dtype are guaranteed to be set
    # (files is not empty)
    assert metadata is not None
    assert dtype is not None

    # Create the tidal model
    model = _create_tidal_model(
        metadata,
        dtype,
    )

    # Second pass: load and add constituent data
    for constituent, path in files.items():
        with netCDF4.Dataset(path, "r") as dataset:
            wave = _process_constituent_data(
                dataset,
                var_names,
                metadata,
            )
            model.add_constituent(constituent, wave)

    return model
