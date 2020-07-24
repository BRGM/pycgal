import pytest

import numpy as np

# We need to import Point_3 to display them cf (*) below
from pycgal.Epick import Surface_mesh, Point_3
from pycgal.test.data import all_vertices, triangles, square_vertices, square


def test_meshes():

    for vertices, faces in [
        (all_vertices, triangles),
        (square_vertices, square),
        (all_vertices, [square, triangles]),
    ]:
        print()
        mesh = Surface_mesh(vertices, faces)
        print(
            f"mesh with {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
        )
        print(f"Vertices:")
        for v in mesh.vertices():
            # (*) pycgal.Epick.Point_3 i.e. Surface_mesh::Point must be available here
            print(f"{v} at {mesh.point(v)}")
        print(mesh.as_arrays())


if __name__ == "__main__":
    test_meshes()
