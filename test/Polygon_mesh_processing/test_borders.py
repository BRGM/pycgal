from pycgal.Surface_mesh import Surface_mesh, Edges, Faces
from pycgal.Polygon_mesh_processing import border_halfedges


def test_border_halfedges(simple_mesh):

    print()
    mesh = Surface_mesh(simple_mesh.square_vertices, simple_mesh.square)
    print(
        f"Mesh with {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    )
    borders = border_halfedges(mesh)
    assert borders.size == 4
    print(f"Border halfedges:", [h for h in borders])
    mesh = Surface_mesh(simple_mesh.all_vertices, simple_mesh.triangles)
    print(
        f"Mesh with {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    )
    borders = border_halfedges(mesh)
    assert borders.size == 12
    print(f"Border halfedges:", [h for h in borders])
    edges = Edges(mesh, borders)
    print(f"Border edges:", [e for e in edges])
    faces = Faces(mesh, borders, opposite_faces=True)
    faces.remove_duplicates()
    print(f"Border faces:", [f for f in faces])
