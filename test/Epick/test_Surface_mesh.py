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
        mesh = Surface_mesh(vertices, faces)
        print(
            f"mesh with {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
        )
        print(f"Vertices:")
        for v in mesh.vertices():
            # (*) pycgal.Epick.Point_3 i.e. Surface_mesh::Point must be available here
            print(f"{v} at {mesh.point(v)}")
        print(mesh.as_arrays())


def test_centroids():

    for vertices, faces in [
        (all_vertices, triangles),
        (square_vertices, square),
    ]:
        mesh = Surface_mesh(vertices, faces)
        centroids = np.array(
            [np.mean(vertices[np.array(face_nodes)], axis=0) for face_nodes in faces]
        )
        assert np.allclose(mesh.centroids(), centroids)


def test_io():
    for k, (vertices, faces) in enumerate(
        [
            (all_vertices, triangles),
            (square_vertices, square),
            (all_vertices, [square, triangles]),
        ]
    ):
        mesh = Surface_mesh(vertices, faces)
        filename = f"mesh{k}.off"
        mesh.write_off(filename)
        mesh = Surface_mesh()
        mesh.read_off(filename)
        mesh = Surface_mesh.from_off(filename)


if __name__ == "__main__":
    test_meshes()
    test_centroids()
    test_io()
