import sys
import shutil

try:
    from skbuild import setup
except ImportError:
    print("scikit-build is required to build from source!", file=sys.stderr)
    print("Install it running: python -m pip install scikit-build")
    sys.exit(1)

setup(
    name="pycgal",
    version="0.2.0",
    description="minimal CGAL wrap with pybind11",
    author="brgm",
    license="GPL v.3",
    url="https://gitlab.brgm.fr/brgm/modelisation-geologique/pycgal",
    packages=["pycgal"],
    install_requires=["numpy"],
)
