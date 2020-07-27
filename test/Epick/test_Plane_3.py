import pytest

import numpy as np

# We need to import Point_3 to display them cf (*) below
from pycgal.Epick import Plane_3, Point_3, Vector_3
from pycgal.test.data import all_vertices, triangles, square_vertices, square


def test_planes():

    P1 = Plane_3(Point_3(0, 0, 0), Point_3(1, 0, 0), Point_3(0, 1, 0))
    P2 = Plane_3(Point_3(0, 0, 0), Vector_3(0, 0, 1))
    assert P1 == P2
    P3 = Plane_3(Point_3(0, 0, 0), Vector_3(1, 0, 0))
    assert P1 != P3


if __name__ == "__main__":
    test_planes()
