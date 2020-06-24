import sys
import shutil

try:
    from skbuild import setup
except ImportError:
    print("scikit-build is required to build from source!", file=sys.stderr)
    print("Install it running: python -m pip install scikit-build")
    sys.exit(1)

from pathlib import Path


def _add_directory(path):
    if not path.exists():
        path.mkdir(parents=True)
    assert path.is_dir()


with Path("./PackagesList.txt").open() as f:
    pycgal_packages = []
    package_init_template = Path("src/template/__init__.py.template")
    assert package_init_template.is_file()
    for line in f:
        name = line.strip()
        if len(name) > 0:
            package_path = Path(f"pycgal/{name}")
            _add_directory(package_path)
            pycgal_packages.append(f"pycgal.{name}")
            package_init = package_path / "__init__.py"
            if not package_init.exists():
                shutil.copyfile(package_init_template, package_init)
            assert package_init.is_file()
            implementation_path = package_path / "_impl"
            _add_directory(implementation_path)
            pycgal_packages.append(f"pycgal.{name}._impl")
            implementation_init = package_path / "__init__.py"
            if not implementation_init.exists():
                implementation_init.touch()
            assert implementation_init.is_file()

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
