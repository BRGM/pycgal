import numpy as np

# We need to import Point_3 to display them cf (*) below
from pycgal.Epick import Point_3, Line_3, Ray_3
from pycgal.Surface_mesh import Surface_mesh
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
