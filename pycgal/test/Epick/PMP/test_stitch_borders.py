from pycgal.Epick import Surface_mesh
from pycgal.Epick.PMP import stitch_borders
from pycgal.test.data import all_vertices, triangles, square_vertices, square


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
    stitch_borders(mesh)
    print(
        f"... after stiching borders: {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    )
    assert mesh.number_of_vertices() == all_vertices.shape[0]


if __name__ == "__main__":
    test_extend_mesh()
