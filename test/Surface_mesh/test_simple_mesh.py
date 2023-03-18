import numpy as np

import pytest

from pycgal.Epick import Point_3, Vector_3
from pycgal.Surface_mesh import Surface_mesh, Vertices


def test_meshes(simple_mesh):

    for vertices, faces in [
        (simple_mesh.all_vertices, simple_mesh.triangles),
        (simple_mesh.square_vertices, simple_mesh.square),
        (simple_mesh.all_vertices, [simple_mesh.square, simple_mesh.triangles]),
    ]:
        mesh = Surface_mesh(vertices, faces)
        print(
            f"mesh with {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
        )
        print(f"Vertices:")
        for v in mesh.vertices():
            print(f"{v} at {mesh.point(v)}")
        print(mesh.as_arrays())
        print(mesh.as_lists())


def test_array_conversion(simple_mesh):
    mesh = Surface_mesh(
        simple_mesh.all_vertices, [simple_mesh.square, simple_mesh.triangles]
    )
    with pytest.raises(RuntimeError):
        mesh.as_arrays(throw_on_breaking_order=True)
    vertices, faces, indices = mesh.as_arrays(return_index=True)
    assert len(faces[0]) == len(indices[0]) == 4
    assert len(indices[1]) == 1
    assert indices[1][0] == 0


def test_addition(simple_mesh):
    data = simple_mesh
    square = Surface_mesh(data.square_vertices, data.square)
    triangles = Surface_mesh(data.all_vertices, data.triangles)
    whole = square + triangles
    assert square.number_of_vertices() == 4
    assert triangles.number_of_vertices() == 8
    assert whole.number_of_vertices() == 12
    triangles += square
    assert square.number_of_vertices() == 4
    assert triangles.number_of_vertices() == 12


def test_centroids(simple_mesh):

    for vertices, faces in [
        (simple_mesh.all_vertices, simple_mesh.triangles),
        (simple_mesh.square_vertices, simple_mesh.square),
    ]:
        mesh = Surface_mesh(vertices, faces)
        centroids = np.array(
            [np.mean(vertices[np.array(face_nodes)], axis=0) for face_nodes in faces]
        )
        assert np.allclose(mesh.centroids(), centroids)


def test_io(simple_mesh):
    for k, (vertices, faces) in enumerate(
        [
            (simple_mesh.all_vertices, simple_mesh.triangles),
            (simple_mesh.square_vertices, simple_mesh.square),
            (simple_mesh.all_vertices, [simple_mesh.square, simple_mesh.triangles]),
        ]
    ):
        mesh = Surface_mesh(vertices, faces)
        filename = f"mesh{k}.off"
        mesh.write_off(filename)
        mesh = Surface_mesh()
        mesh.read_off(filename)
        mesh = Surface_mesh.from_off(filename)


def test_move_points(squares_2x2):

    mesh = Surface_mesh(squares_2x2.vertices, squares_2x2.faces)
    assert all(mesh[v].z == 0 for v in mesh.vertices())
    for v in mesh.vertices():
        mesh[v] += Vector_3(0, 0, 3.14)
    assert all(mesh[v].z == 3.14 for v in mesh.vertices())


def test_access_points(squares_2x2):

    mesh = Surface_mesh(squares_2x2.vertices, squares_2x2.faces)
    points = mesh.points(Vertices.create(mesh.vertices()))
    assert points.shape == (9, 3)
    points = mesh.points(list(mesh.vertices()))
    assert points.shape == (9, 3)


def test_simple_square():
    add_corners = lambda mesh: [
        mesh.add_vertex(Point_3(x, y, 0)) for x, y in ((0, 0), (0, 1), (1, 1), (0, 1))
    ]
    mesh = Surface_mesh()
    corners = add_corners(mesh)
    mesh.add_face(corners)
    mesh = Surface_mesh()
    corners = add_corners(mesh)
    mesh.add_face(*corners)
    mesh = Surface_mesh()
    corners = add_corners(mesh)
    mesh.add_face(*corners[:3])


def test_dict_of_indices(squares_2x2):
    mesh = Surface_mesh(squares_2x2.vertices, squares_2x2.faces)
    d = {}
    for v in mesh.vertices():
        d[v] = str(v)
    for e in mesh.edges():
        d[e] = str(e)
    for f in mesh.faces():
        d[f] = str(f)
    for i, s in d.items():
        print(i, s)


def test_unit_cube(unit_cube):
    cube = Surface_mesh(*unit_cube)
    assert cube.number_of_vertices() == 8
    assert cube.number_of_faces() == 6
