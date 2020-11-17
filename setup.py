import re
from pathlib import Path

try:
    from skbuild import setup
except ImportError:
    print("scikit-build is required to build from source!", file=sys.stderr)
    print("Install it running: python -m pip install scikit-build")
    sys.exit(1)


def _find_version(name):
    package_directory = Path(name)
    assert package_directory.is_dir(), f"Could not find package directory: {name}"
    init_file = package_directory / "__init__.py"
    assert init_file.is_file(), f"Could not find package __init__.py"
    for name, value in re.findall(
        r"""__([a-z]+)__ = "([^"]+)""", init_file.read_text(encoding="utf8")
    ):
        if name == "version":
            return value


name = "pycgal"
version = _find_version(name)

setup(
    name=name,
    version=version,
    description="minimal CGAL wrap with pybind11",
    author="brgm",
    license="GPL v.3",
    url="https://gitlab.brgm.fr/brgm/modelisation-geologique/pycgal",
    packages=[name],
    install_requires=["numpy >= 1.17"],
)
