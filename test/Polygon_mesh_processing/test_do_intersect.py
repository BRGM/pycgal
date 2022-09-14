import numpy as np
from pycgal.Polygon_mesh_processing import do_intersect
from pycgal.Surface_mesh import Surface_mesh


def test_do_intersect():

    tm1 = Surface_mesh([(-1, 0, 0), (1, 0, 0), (0, 2, 0)], [(0, 1, 2)])
    tm2 = Surface_mesh([(-1, 1, -1), (1, 1, -1), (0, 1, 1)], [(0, 1, 2)])
    tm3 = Surface_mesh([(-1, 0, 1), (1, 0, 1), (0, 2, 1)], [(0, 1, 2)])

    assert do_intersect(tm1, tm2)
    assert not do_intersect(tm1, tm3)
