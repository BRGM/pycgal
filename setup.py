try:
    from skbuild import setup
except ImportError:
    import sys

    print("scikit-build is required to build from source!", file=sys.stderr)
    print("Install it running: python -m pip install scikit-build")
    sys.exit(1)


setup(
    name="pycgal",
    setup_requires=["setuptools_scm"],
    use_scm_version={"write_to": "pycgal/version.py"},
    description="minimal CGAL wrap with pybind11",
    author="brgm",
    license="GPLv3",
    url="https://gitlab.brgm.fr/brgm/modelisation-geologique/pycgal",
    packages=["pycgal"],
    install_requires=["numpy >= 1.17"],
    classifiers=[
        "Development Status :: 4 - Beta",
        "License :: OSI Approved :: GNU General Public License v3 (GPLv3)",
        "Natural Language :: English",
        "Operating System :: Microsoft :: Windows",
        "Operating System :: POSIX :: Linux",
        "Programming Language :: Python",
        "Programming Language :: C++",
        "Topic :: Scientific/Engineering",
    ],
)
