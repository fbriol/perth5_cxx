import os
from perth import Constituent


def fetch_tide_model_files(sad: str) -> dict[Constituent, str]:
    """Load the ocean tide model from the specified SAD directory."""
    if not os.path.isdir(sad):
        raise ValueError(
            f"The specified SAD directory '{sad}' does not exist."
        )
    return {
        Constituent.Q1: os.path.join(sad, "q1.nc"),
        Constituent.O1: os.path.join(sad, "o1.nc"),
        Constituent.P1: os.path.join(sad, "p1.nc"),
        Constituent.S1: os.path.join(sad, "s1.nc"),
        Constituent.K1: os.path.join(sad, "k1.nc"),
        Constituent.N2: os.path.join(sad, "n2.nc"),
        Constituent.M2: os.path.join(sad, "m2.nc"),
        Constituent.S2: os.path.join(sad, "s2.nc"),
        Constituent.K2: os.path.join(sad, "k2.nc"),
        Constituent.M4: os.path.join(sad, "m4.nc"),
        Constituent.MS4: os.path.join(sad, "ms4.nc"),
        Constituent._2N2: os.path.join(sad, "2n2.nc"),
        Constituent.MU2: os.path.join(sad, "mu2.nc"),
        Constituent.J1: os.path.join(sad, "j1.nc"),
        Constituent.SIG1: os.path.join(sad, "sig1.nc"),
        Constituent.OO1: os.path.join(sad, "oo1.nc"),
    }
