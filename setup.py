import sys

try:
    from skbuild import setup
except ImportError:
    print("scikit-build is required to build from source!", file=sys.stderr)
    print("Install it running: python -m pip install scikit-build")
    sys.exit(1)

from pathlib import Path

with Path("./PackagesList.txt").open() as f:
    pycgal_packages = []
    for line in f:
        name = line.strip()
        if len(name) > 0:
            package_path = Path(f"pycgal/{name}")
            if not package_path.exists():
                package_path.mkdir(parents=True)
            assert package_path.is_dir()
            pycgal_packages.append(f"pycgal.{name}")
            package_init = package_path / "__init__.py"
            if not package_init.exists():
                with package_init.open("w") as _:
                    pass
            assert package_init.is_file()

setup(
    name="pyCGAL",
    version="0.0.1",
    description="minimal CGAL wrap with pybind11",
    author="brgm",
    license="GPL v.3",
    packages=["pycgal",] + pycgal_packages,
    # FIXME: it might be more robust to use git
    #        to copy tracked files to default _skbuild directory
    cmake_install_dir="pycgal",
    install_requires=["numpy"],
)
