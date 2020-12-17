from pycgal.Polygon_mesh_processing import border_halfedges
from pycgal.Polygon_mesh_processing import extract_boundary_cycles
from pycgal.Polygon_mesh_processing import extract_zmap_corners_and_borders
from pycgal.Surface_mesh import Edges
from pycgal.Surface_mesh import Faces
from pycgal.Surface_mesh import Surface_mesh


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


def test_extract_boundary_cycles(simple_mesh):

    mesh = Surface_mesh(simple_mesh.all_vertices, simple_mesh.triangles)
    cycles = extract_boundary_cycles(mesh)
    for seed in cycles:
        L = 1
        h0 = mesh.next(seed)
        while h0 != seed:
            L += 1
            h0 = mesh.next(h0)
        print("Length:", L)


def test_extract_zmap_corners_and_borders(simple_mesh):

    mesh = Surface_mesh(simple_mesh.all_vertices, simple_mesh.triangles)
    corners, borders = extract_zmap_corners_and_borders(mesh, 0)
    assert len(corners) == 0
    assert len(borders) == 2
    corners, borders = extract_zmap_corners_and_borders(mesh)
    assert len(corners) == 8
    assert len(borders) == 8
