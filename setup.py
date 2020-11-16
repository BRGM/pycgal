import sys
import shutil

try:
    from skbuild import setup
except ImportError:
    print("scikit-build is required to build from source!", file=sys.stderr)
    print("Install it running: python -m pip install scikit-build")
    sys.exit(1)

# FIXME: For some strange reason when packages is specified in setup.cfg
#        compiled modules are not included in the generated wheel.
#        Is the content of the package directory parsed
#        after compilation when passed as an argument here?
setup(
    packages=["pycgal",]
)
