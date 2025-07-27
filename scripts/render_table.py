from perth import _core
import numpy
import pathlib

ROOT = pathlib.Path(__file__).parent.parent

XDO = "RSTUVWXYZABCDEFGHIJKLMNOP"

GREEK_LETTERS = {
    "Alpha": "\\alpha",
    "Beta": "\\beta",
    "Gamma": "\\gamma",
    "Delta": "\\delta",
    "Psi": "\\psi",
    "Phi": "\\phi",
    "Theta": "\\theta",
    "Chi": "\\chi",
    "Pi": "\\pi",
    "Mu": "\\mu",
    "Nu": "\\nu",
    "Lambda": "\\lambda",
    "Eps": "\\epsilon",
    "Eta": "\\eta",
    "Sigma": "\\sigma",
    "Ups": "\\upsilon",
    "Rho": "\\rho",
    "Tau": "\\tau",
}


def _pretty_name(name: str) -> str:
    """
    Convert the greek letter in the name to its LaTeX representation.
    If the name does not contain a greek letter, return it unchanged.
    """
    for greek, latex in GREEK_LETTERS.items():
        if greek in name:
            value = name[-1]
            return f"{name} (${latex} {value}$)"
    return name


def to_xdo(numbers: numpy.ndarray):
    if numbers[-1] == 3:  # noqa: PLR2004
        numbers[-1] = -1
    xdo = "".join([XDO[item + 8] for item in numbers])
    return f"{xdo[:1]} {xdo[1:4]} {xdo[4:]}"


def create_constituent_representation() -> str:
    """ """
    table = _core.assemble_constituent_table()
    lines = [
        "| Name | Speed (deg/h) | XDO |",
        "| --- | --- | --- |",
    ]

    data = {}
    for constituent in table:
        component = table[constituent]
        name = _pretty_name(_core.constituent_to_name(constituent))
        frequency = _core.tidal_frequency(component.doodson_number[:6])
        xdo = to_xdo(component.doodson_number)
        data[constituent.name] = {
            "speed": frequency,
            "name": name,
            "xdo": xdo,
        }

    lines.extend(
        f"| {values['name']} | {values['speed']:.7f}  | {values['xdo']} |"
        for values in sorted(data.values(), key=lambda item: item["speed"])
    )
    return "\n".join(lines)


def main():
    """
    Main function to run the script.
    """
    with open(ROOT / "CONSTITUENTS.md", "w") as f:
        f.write("""## Tidal Constituents\n\n""")
        f.write(create_constituent_representation())
        f.write("\n")


if __name__ == "__main__":
    main()
