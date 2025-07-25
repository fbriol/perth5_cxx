import perth
import perth._core

from . import fetch_tide_model_files


def test_model(sad: str):
    tide_model_files = fetch_tide_model_files(sad)
    assert len(tide_model_files) > 0

    model = perth.load_model(tide_model_files)
    assert isinstance(model, perth._core.TidalModelFloat32)
