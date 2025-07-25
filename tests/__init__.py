import os
from perth import Constituent


def fetch_got_files(sad: str) -> dict[Constituent, str]:
    """Load the ocean tide model from the specified SAD directory."""
    if not os.path.isdir(sad):
        raise ValueError(
            f"The specified SAD directory '{sad}' does not exist."
        )
    return {
        Constituent.Q1: os.path.join(sad, "GOT5.5", "q1.nc"),
        Constituent.O1: os.path.join(sad, "GOT5.5", "o1.nc"),
        Constituent.P1: os.path.join(sad, "GOT5.5", "p1.nc"),
        Constituent.S1: os.path.join(sad, "GOT5.5", "s1.nc"),
        Constituent.K1: os.path.join(sad, "GOT5.5", "k1.nc"),
        Constituent.N2: os.path.join(sad, "GOT5.5", "n2.nc"),
        Constituent.M2: os.path.join(sad, "GOT5.5", "m2.nc"),
        Constituent.S2: os.path.join(sad, "GOT5.5", "s2.nc"),
        Constituent.K2: os.path.join(sad, "GOT5.5", "k2.nc"),
        Constituent.M4: os.path.join(sad, "GOT5.5", "m4.nc"),
        Constituent.MS4: os.path.join(sad, "GOT5.5", "ms4.nc"),
        Constituent._2N2: os.path.join(sad, "GOT5.5", "2n2.nc"),
        Constituent.MU2: os.path.join(sad, "GOT5.5", "mu2.nc"),
        Constituent.J1: os.path.join(sad, "GOT5.5", "j1.nc"),
        Constituent.SIG1: os.path.join(sad, "GOT5.5", "sig1.nc"),
        Constituent.OO1: os.path.join(sad, "GOT5.5", "oo1.nc"),
    }


def fetch_re14_files(sad: str) -> dict[Constituent, str]:
    """Load the ocean tide model from the specified SAD directory."""
    if not os.path.isdir(sad):
        raise ValueError(
            f"The specified SAD directory '{sad}' does not exist."
        )
    return {
        Constituent.MF: os.path.join(sad, "RE14", "mf.nc"),
        Constituent.MM: os.path.join(sad, "RE14", "mm.nc"),
        Constituent.MT: os.path.join(sad, "RE14", "mt.nc"),
        Constituent.NODE: os.path.join(sad, "RE14", "node.nc"),
        Constituent.SA: os.path.join(sad, "RE14", "sa.nc"),
        Constituent.SSA: os.path.join(sad, "RE14", "ssa.nc"),
    }
