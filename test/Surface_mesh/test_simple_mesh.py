import numpy as np

# We need to import Point_3 to display them cf (*) below
from pycgal.Epick import Point_3, Vector_3
from pycgal.Surface_mesh import Surface_mesh


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
            # (*) pycgal.Epick.Point_3 i.e. Surface_mesh::Point must be available here
            print(f"{v} at {mesh.point(v)}")
        print(mesh.as_arrays())
        print(mesh.as_lists())


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
