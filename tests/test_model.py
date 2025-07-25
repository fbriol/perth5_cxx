import perth
import perth._core

from . import fetch_got_files


#: Interpolation in the original Perth model on float32, here we use float64
#: during the interpolation process.
TOLERANCE = 1e-4


def test_model(sad: str):
    tide_model_files = fetch_got_files(sad)
    assert len(tide_model_files) > 0

    model = perth.load_model(tide_model_files)
    assert isinstance(model, perth._core.TidalModelFloat32)
    acc = model.accelerator(0)
    assert isinstance(acc, perth._core.Accelerator)
    tide_table = perth._core.make_tide_table(model.identifiers())
    quality = model.interpolate(
        -7.6880002021789551,
        59.194999694824219,
        tide_table,
        acc,
    )
    assert quality is perth.Quality.INTERPOLATED
    expected = {
        perth.Constituent.Q1: complex(2.04458141, -2.37769175),
        perth.Constituent.O1: complex(7.72207355, 1.22571337),
        perth.Constituent.P1: complex(-2.00809360, 1.64180589),
        perth.Constituent.S1: complex(0.478574932, 0.490766048),
        perth.Constituent.K1: complex(-7.94872284, 5.25266790),
        perth.Constituent.N2: complex(-18.6384964, 3.00534558),
        perth.Constituent.M2: complex(-90.5211105, -20.6030121),
        perth.Constituent.S2: complex(-24.5810661, -25.6641655),
        perth.Constituent.K2: complex(-7.38324118, -7.09723043),
        perth.Constituent.M4: complex(0.255432695, 0.141946957),
        perth.Constituent.MS4: complex(-5.95567748e-02, -0.140465707),
        perth.Constituent._2N2: complex(-2.23751235, 1.32460070),
        perth.Constituent.MU2: complex(-2.65443683, 2.40790391),
        perth.Constituent.J1: complex(-0.330671191, 0.154265821),
        perth.Constituent.SIG1: complex(2.93300748e-02, -0.700001657),
        perth.Constituent.OO1: complex(-0.218446687, -0.177276790),
    }
    for constituent, value in acc.values:
        assert constituent in expected
        expected_value = expected[constituent] * 1e-2  # cm to m
        assert abs(value - expected_value) < TOLERANCE, (
            f"Value for {constituent} does not match expected value."
        )
