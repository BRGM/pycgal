import numpy as np
from pycgal.Polygon_mesh_processing import split
from pycgal.Surface_mesh import Surface_mesh


# This test https://github.com/CGAL/cgal/issues/5867
def test_split():
    tm1 = Surface_mesh(
        np.array([(0, 0, 0), (1, 0, 0), (1, 1, 0), (0, 1, 0)], dtype=np.double),
        np.array([(0, 1, 2), (0, 2, 3)]),
    )
    tm2 = Surface_mesh(
        np.array([(0.5, 0, 0), (0.5, 1, 0), (0.5, 1, -1)], dtype=np.double),
        np.array([(0, 1, 2)]),
    )
    split(tm2, tm1)
