import pytest

import numpy as np

from pycgal.Epick import Surface_mesh
from pycgal.Epick.PMP import isotropic_remeshing, triangulate_faces, is_triangle_mesh
from pycgal.test.data import all_vertices, triangles, square_vertices, square


def test_remesh():

    mesh = Surface_mesh(all_vertices, [square, triangles])
    print(
        f"Hybrid mesh with {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    )
    assert not is_triangle_mesh(mesh)
    with pytest.raises(RuntimeError):
        isotropic_remeshing(mesh, 0.2)
    triangulate_faces(mesh)
    assert is_triangle_mesh(mesh)
    isotropic_remeshing(mesh, 0.2)
    print(
        f"... after remesh: {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    )


if __name__ == "__main__":
    test_remesh()
