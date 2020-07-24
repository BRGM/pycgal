import pytest

import numpy as np

from pycgal.Epick import Surface_mesh
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
            print(f"{v} at {mesh.point(v)}")
        print(mesh.as_arrays())


def test_remesh():

    print()
    mesh = Surface_mesh(all_vertices, [square, triangles])
    print(
        f"Hybrid mesh with {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    )
    # with pytest.raises(RuntimeError):
    #    isotropic_remeshing(mesh, 0.2)
    # assert not is_triangle_mesh(mesh)
    # triangulate_faces(mesh)
    # assert is_triangle_mesh(mesh)
    # isotropic_remeshing(mesh, 0.2)
    # print(
    #    f"... after remesh: {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    # )


def test_extend_mesh():

    print()
    mesh = Surface_mesh(square_vertices, square)
    print(
        f"Mesh with {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    )
    mesh.extend(all_vertices, [triangles])
    print(
        f"... after extension: {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    )
    assert (
        mesh.number_of_vertices() == square_vertices.shape[0] + all_vertices.shape[0]
    )  # vertices are duplicated
    assert mesh.number_of_faces() == 5
    # stitch_borders(mesh)
    # print(
    #    f"... after stiching borders: {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    # )
    # assert mesh.number_of_vertices() == all_vertices.shape[0]


if __name__ == "__main__":
    test_meshes()
    test_remesh()
    test_extend_mesh()
    test_foo()
