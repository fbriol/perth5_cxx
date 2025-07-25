import numpy
import pytest
import perth

from . import fetch_tide_model_files


def test_ocean_tide(sad: str):
    tide_model_files = fetch_tide_model_files(sad)
    assert len(tide_model_files) > 0

    model = perth.load_model(tide_model_files)
    handler = perth.Perth(model)

    lon = numpy.array([-7.6880002021789551])
    lat = numpy.array([59.194999694824219])
    time = numpy.array(["1983-01-01T00:00:00"], dtype="datetime64[ns]")

    result = handler.evaluate(lon, lat, time, 0, perth.LINEAR_ADMITTANCE)
    assert pytest.approx(result[0], abs=1e-6) == -0.9295959
    assert pytest.approx(result[1], abs=1e-6) == -0.00049025
    assert result[2] == 4
