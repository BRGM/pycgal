from pycgal.Epick import Line_3, Point_3, Segment_3
from pycgal.Surface_mesh import Surface_mesh, add_center_vertex
from pycgal.Surface_mesh import Tree


def test_surface_mesh_tree(simple_mesh):

    mesh = Surface_mesh(simple_mesh.all_vertices, simple_mesh.triangles)
    tree = Tree(mesh)
    line = Line_3(Point_3(0, 0, -1), Point_3(0, 0, 1))
    intersection = tree.any_intersection(line)
    assert not intersection
    line = Line_3(Point_3(-1.1, 0, -1), Point_3(-1.1, 0, 1))
    intersection = tree.any_intersection(line)
    assert intersection
    P, f = intersection
    assert P.x == -1.1 and P.y == 0 and P.z == 0
    line = Line_3(Point_3(-1, 0, 0), Point_3(1, 0, 0))
    intersections = tree.all_intersections(line)
    assert intersections and len(intersections) == 2


def test_triangulate_segment_intersection(simple_mesh):

    mesh = Surface_mesh(simple_mesh.all_vertices, simple_mesh.triangles)
    tree = Tree(mesh)
    S = Segment_3(Point_3(-1.1, 0, -1), Point_3(-1.1, 0, 1))
    intersection = tree.any_intersection(S)
    assert intersection
    P, f = intersection
    h = mesh.halfedge(f)
    assert not mesh.is_border(h)
    # WARNING: here we should test that P is not intersecting a face edge
    h = add_center_vertex(h, mesh)
    mesh[mesh.target(h)] = P
