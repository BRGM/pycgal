
pyCGAL
======

**python + CGAL = pyCGAL**

pyCGAL offers bindings powered by
`pybind11 <https://github.com/pybind/pybind11>`_
for a (very) small part of the
`CGAL <https://www.cgal.org/>`_  library.

These bindings were developed for convenience at
the `BRGM <https://www.brgm.fr/fr>`_ (French Geological Survey).

The CGAL library is developed in the framework of
the `CGAL project <https://www.cgal.org/project.html>`_ and is maintained
and distributed by `Geometry Factory <https://geometryfactory.com/>`_.


Bindings developed by `Geometry Factory <https://geometryfactory.com/>`_,
based on `SWIG <http://www.swig.org/>`_,
may be found at
`GitHub <https://github.com/CGAL/cgal-swig-bindings>`_
and `PyPi <https://test.pypi.org/project/cgal>`_.


Installation
------------

If you already registered the BRGM `nexus <https://nexus.brgm.fr/repository/pypi-all/simple>`_
as one of your ``pip`` repositories, simply type:

``pip install pycgal``

else:

``pip install pycgal -i https://nexus.brgm.fr/repository/pypi-all/simple``

Alternatively, one can build from sources using:

.. code-block:: sh

   git clone https://gitlab.brgm.fr/brgm/modelisation-geologique/pycgal
   cd pycgal
   # this requires CGAL and BOOST to be installed and available
   python setup.py install

The build step relies on `scikit-build <https://scikit-build.readthedocs.io/en/latest/>`__.
Useful command line options can be found in the
`scikit-build documentation <https://scikit-build.readthedocs.io/en/latest/usage.html#command-line-options>`__,
(*e.g.* the `--build-type` option,
the `-G` to select a generator such as  *Visual Studio 16 2019* on Windows
the `-D` flag to directly control the underlying cmake build).

How to use
----------

pyCGAL is not documented yet, but it is designed to be as close as possible
to the wrapped C++ algorithms so that the
`CGAL documentation <https://doc.cgal.org/latest/Manual/general_intro.html>`_
should be helpful.

Acknowledgement
---------------

These bindings are provided by the `BRGM <https://www.brgm.fr/fr>`_
(French Geological Survey).
They **do not** come as a part of the official CGAL library
and come **without** any warranty from BRGM.

Any contribution is welcomed.

License
-------

pyCGAL is free software: you can redistribute it and/or modify it under
the terms of the `GNU General Public License version 3 <https://www.gnu.org/licenses/gpl.html>`__.
