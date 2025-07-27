# Perth C++ - Ocean Tide Prediction Library

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Python](https://img.shields.io/badge/python-3.12%2B-blue.svg)](https://www.python.org/downloads/)

Perth C++ is a high-performance C++ implementation with Python bindings for computing ocean tide height predictions at any time and location. This project is a complete port of the original Fortran `perth5` library by Richard Ray ([https://codeberg.org/rray/perth5](https://codeberg.org/rray/perth5)), developed specifically for processing satellite altimeter data in SWOT (Surface Water and Ocean Topography) products under CNES funding.

## Overview

Perth C++ provides accurate tidal predictions using two-dimensional finite difference grids of constituent amplitudes and phases. The library currently processes NetCDF format tide model files and can handle any number of tidal constituents with optional inference capabilities for missing waves.

### Key Features

- **High Performance**: Modern C++20 implementation with Python bindings via nanobind
- **NetCDF Support**: Primary support for NetCDF format tide model files
- **Tidal Inference**: Linear and Fourier-series interpolation/extrapolation for missing constituents
- **Multiple Models**: Tested with GOT models, with potential support for other tide models
- **Parallel Processing**: Multi-threaded evaluation for large datasets
- **Satellite Applications**: Optimized for satellite altimeter data processing
- **Quality Metrics**: Interpolation quality indicators for each prediction

### Applications

The primary application is processing satellite altimeter data, particularly for:
- SWOT mission data products
- Ocean surface topography studies
- Tide corrections for altimetry measurements
- Oceanographic research and analysis

**Note**: This software is designed for scientific research and should NOT be used for coastal navigation or any applications that risk life or property.

## Installation

### Requirements

- Python 3.12 or higher
- CMake 3.14 or higher
- C++20 compatible compiler
- Eigen3 library
- NetCDF4 library

### From Source

```bash
git clone --recursive https://github.com/fbriol/perth5_cxx.git
cd perth5_cxx
pip install .
```

### Dependencies

The library automatically handles the following dependencies:
- **nanobind**: For Python-C++ bindings
- **Eigen3**: For linear algebra operations
- **NetCDF4**: For reading NetCDF tide model files
- **NumPy**: For Python array operations

## Quick Start

### Loading a Tide Model

```python
import perth
import numpy as np

# Load tide model from NetCDF files
tide_model_files = {
    perth.Constituent.M2: "path/to/m2.nc",
    perth.Constituent.S2: "path/to/s2.nc",
    perth.Constituent.K1: "path/to/k1.nc",
    perth.Constituent.O1: "path/to/o1.nc",
    # ... add more constituents
}

model = perth.load_model(tide_model_files)
predictor = perth.Perth(model)
```

### Computing Tide Predictions

```python
# Define coordinates and time
lon = np.array([-7.688])  # longitude in degrees
lat = np.array([59.195])  # latitude in degrees
time = np.array(["2023-01-01T12:00:00"], dtype="datetime64[ns]")

# Evaluate tides with inference
tide, tide_lp, quality = predictor.evaluate(
    lon, lat, time,
    time_tolerance=0.0,
    interpolation_type=perth.LINEAR_ADMITTANCE,
    num_threads=4  # Use 4 threads for parallel processing
)

print(f"Short-period tide: {tide[0]:.3f} m")
print(f"Long-period tide: {tide_lp[0]:.6f} m")
print(f"Quality: {quality[0]}")  # 4 = interpolated, lower values = extrapolated
```

### Supported Constituents

The library supports 77 tidal constituents as defined in [`include/perth/constituent.hpp`](include/perth/constituent.hpp).

> Note that some tidal constituents from the original Fortran code were intentionally not ported to maintain consistency with the same set of tidal waves used in pyfes (https://github.com/CNES/aviso-fes). The supported constituents include:

For a complete reference table with speeds and XDO codes for all constituents, see [CONSTITUENTS.md](CONSTITUENTS.md).

**Long Period:**
- Node, Sa, Ssa, Sta, MSm, Mm, MSf, Mf, MSt, Mtm, MSqm, Mq

**Diurnal (once daily):**
- 2Q1, Sigma1, Q1, Rho1, O1, Tau1, Beta1, M1, Chi1, Pi1, P1, S1, K1, Psi1, Phi1, Theta1, J1, SO1, OO1, Ups1, Sa1

**Semidiurnal (twice daily):**
- Eps2, 2N2, Mu2, N2, Nu2, Gamma2, Alpa2, M2, Beta2, Delta2, Lambda2, L2, T2, S2, R2, K2, Eta2, 2MN2, 2MS2, 2SM2, MSN2

**Terdiurnal:**
- M3, MO3, MK3, 2MK3

**Quarter-diurnal and higher:**
- M4, MS4, MN4, N4, R4, S4, SK4, SN4

**Sixth-diurnal:**
- M6, 2MK6, 2MN6, 2MS6, 2SM6, MSN6, MSK6, S6

**Eighth-diurnal:**
- M8

**Additional compound tides:**
- MK4, MKS2, M13

All constituents support automatic inference capabilities when missing from the input tide model data.

## API Reference

### Core Classes

#### `Perth`
Main class for tide evaluation.

```python
predictor = Perth(model, group_modulations=False)
```

- `model`: TidalModel instance
- `group_modulations`: Apply group modulations for nodal corrections

#### `TidalModel`
Container for tide model data.

```python
model = load_model(file_dict, dtype=np.float32)
```

### Methods

#### `evaluate(lon, lat, time, time_tolerance=0.0, interpolation_type=None, num_threads=0)`

Compute tide predictions at specified locations and times.

**Parameters:**
- `lon`: Longitude array (degrees)
- `lat`: Latitude array (degrees)
- `time`: Time array (datetime64)
- `time_tolerance`: Time tolerance for caching (seconds)
- `interpolation_type`: Inference method (`LINEAR_ADMITTANCE` or `FOURIER_ADMITTANCE`)
- `num_threads`: Number of threads for parallel processing (0 for auto-detect)

**Returns:**
- `tide`: Short-period tidal heights
- `tide_lp`: Long-period tidal heights
- `quality`: Quality flags (4=interpolated, 3-1=extrapolated, 0=undefined)

### Interpolation Types

- `perth.LINEAR_ADMITTANCE`: Linear interpolation of tidal admittances
- `perth.FOURIER_ADMITTANCE`: Fourier series interpolation for better accuracy

## Supported Tide Models

### Current Status

This version of Perth C++ is designed to process **NetCDF format** tide model files. While the architecture supports other data formats, the current implementation focuses on NetCDF for optimal performance and metadata handling.

### Tested Models

- **GOT (Goddard Ocean Tide)**: GOT5.5

### File Formats

- **NetCDF**: Primary supported format with full metadata support
- **Binary**: Architecture supports but not currently implemented
- **ASCII**: Architecture supports but not currently implemented

*Future versions may include support for additional file formats based on user requirements.*

## Performance

Perth C++ is optimized for high-performance applications:

- **Vectorized Operations**: Efficient processing of large coordinate arrays
- **Memory Management**: Smart caching and memory-efficient algorithms
- **Parallel Processing**: Multi-threaded evaluation for large datasets
- **Optimized Interpolation**: Fast bilinear interpolation with quality assessment

Typical performance: >10⁶ predictions per second on modern hardware.

## Differences from Original Fortran perth5

This C++ implementation maintains compatibility with the original Fortran `perth5` for NetCDF-based workflows while providing:

1. **Modern Language Features**: C++20 with type safety and memory management
2. **Python Integration**: Native Python bindings via nanobind
3. **Enhanced Performance**: Vectorized operations and parallel processing
4. **Improved API**: Object-oriented design with clear interfaces
5. **Better Memory Management**: Automatic resource management
6. **Extended Precision**: Support for both float32 and float64 precision
7. **NetCDF Focus**: Optimized specifically for NetCDF tide model processing

### Migration from perth5

Users of the original Fortran `perth5` can easily migrate:

```python
# Original perth5 workflow concept
# call perth5(dlat, dlon, time, tide, isdata)

# Perth C++ equivalent
tide, tide_lp, quality = predictor.evaluate(lon, lat, time)
is_valid = quality > 0  # equivalent to isdata
```

## Development and Testing

### Building from Source

```bash
# Clone with submodules
git clone --recursive https://github.com/fbriol/perth5_cxx.git
cd perth5_cxx

# Build in development mode
pip install -e .[testing]

# Run tests
pytest tests/
```

### Running Tests

The test suite requires tide model data files. Set the `SAD` environment variable to point to your tide model directory:

```bash
export SAD=/path/to/tide/models
pytest tests/
```

### C++ Development

```bash
# Build C++ library and tests
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Run C++ tests
ctest
```

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Ensure all tests pass
5. Submit a pull request

## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.

## Citation

If you use Perth C++ in your research, please cite:

```
Perth C++ Ocean Tide Prediction Library
Frédéric Briol, based on perth5 by Richard Ray
https://github.com/fbriol/perth5_cxx
```

Original Fortran implementation:
```
Ray, R. D. (2024). perth5: A Fortran Module for Ocean Tide Prediction.
Goddard Space Flight Center. https://codeberg.org/rray/perth5
```

## Acknowledgments

- **Richard Ray**: Original author of the Fortran `perth5` library
- **CNES**: Funding for SWOT mission applications
- **NASA Goddard Space Flight Center**: Supporting tide model development
- **Contributors**: All developers who have contributed to this project

## Support

For questions, bug reports, or feature requests:

- **Issues**: [GitHub Issues](https://github.com/fbriol/perth5_cxx/issues)
- **Documentation**: [Wiki](https://github.com/fbriol/perth5_cxx/wiki)
- **Email**: fbriol@gmail.
