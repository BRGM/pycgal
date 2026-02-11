from pycgal.Polygon_mesh_processing import stitch_borders
from pycgal.Surface_mesh import Surface_mesh


def test_extend_mesh(simple_mesh):

    print()
    mesh = Surface_mesh(simple_mesh.square_vertices, simple_mesh.square)
    print(
        f"Mesh with {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    )
    mesh.extend(simple_mesh.all_vertices, [simple_mesh.triangles])
    print(
        f"... after extension: {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    )
    assert (
        mesh.number_of_vertices()
        == simple_mesh.square_vertices.shape[0] + simple_mesh.all_vertices.shape[0]
    )  # vertices are duplicated
    assert mesh.number_of_faces() == 5
    stitch_borders(mesh)
    print(
        f"... after stiching borders: {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    )
    assert mesh.number_of_vertices() == simple_mesh.all_vertices.shape[0]
