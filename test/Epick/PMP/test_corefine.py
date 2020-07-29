from pycgal.Epick import Surface_mesh
from pycgal.Epick.PMP import corefine
from pycgal.test.data import square_vertices

import numpy as np


def test_corefine():

    print()
    tm1 = Surface_mesh(square_vertices, np.array([[0, 1, 2]]))
    tm2 = Surface_mesh(square_vertices, np.array([[0, 1, 3]]))
    print(
        f"TM1 with {tm1.number_of_vertices()} vertices and {tm1.number_of_faces()} faces\n"
        + f"TM2 with {tm2.number_of_vertices()} vertices and {tm2.number_of_faces()} faces"
    )
    corefine(tm1, tm2)
    print(
        f"... after corefine:\n"
        + f"TM1 with {tm1.number_of_vertices()} vertices and {tm1.number_of_faces()} faces\n"
        + f"TM2 with {tm2.number_of_vertices()} vertices and {tm2.number_of_faces()} faces"
    )
    assert tm1.number_of_faces() == 2
    assert tm1.number_of_faces() == 2


if __name__ == "__main__":
    test_corefine()
