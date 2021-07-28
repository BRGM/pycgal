try:
    from skbuild import setup
except ImportError:
    print("scikit-build is required to build from source!", file=sys.stderr)
    print("Install it running: python -m pip install scikit-build")
    sys.exit(1)


setup(
    name="pycgal",
    setup_requires=["setuptools_scm"],
    use_scm_version={"write_to": "pycgal/version.py"},
    description="minimal CGAL wrap with pybind11",
    author="brgm",
    license="GPL v.3",
    url="https://gitlab.brgm.fr/brgm/modelisation-geologique/pycgal",
    packages=["pycgal"],
    install_requires=["numpy >= 1.17"],
)
