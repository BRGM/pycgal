import numpy as np

from pycgal.Epick import Point_3, squared_distance, midpoint, barycenter
from pycgal.Surface_mesh import Surface_mesh, locate

# Euler operations on Surface_mesh objects are directly available from Surface_mesh package
from pycgal.Surface_mesh import (
    fill_hole,
    collapse_edge,
    split_edge,
    join_vertex,
    does_satisfy_link_condition,
)
from pycgal.Polygon_mesh_processing import triangulate_faces


def _build_mesh(vertices, faces):
    mesh = Surface_mesh(vertices, faces)
    assert mesh.number_of_faces() == 2
    # locate inner edge
    A, B = Point_3(-1, 0, 0), Point_3(1, 0, 0)
    v0, v1 = locate(mesh, [Point_3(-1, 0, 0), Point_3(1, 0, 0)])
    if mesh[v0] != A:
        v0, v1 = v1, v0
    assert mesh[v0] == A
    assert mesh[v1] == B
    h = mesh.halfedge(v0, v1)
    h = split_edge(h, mesh)
    v2 = mesh.target(h)
    mesh[v2] = barycenter(A, 1 / 3.0, B)
    h = split_edge(h, mesh)
    v3 = mesh.target(h)
    mesh[v3] = barycenter(A, 2 / 3.0, B)
    triangulate_faces(mesh)
    assert mesh.number_of_faces() == 6
    return mesh, (v2, v3)


def test_add_and_collapse_edges(two_triangles):
    mesh, (v0, v1) = _build_mesh(two_triangles.vertices, two_triangles.faces)
    e = mesh.edge(v0, v1)
    assert does_satisfy_link_condition(e, mesh)
    # we do not control which of v0 and v1 disappear
    collapse_edge(e, mesh)


def test_add_and_join_vertex(two_triangles):
    mesh, (v0, v1) = _build_mesh(two_triangles.vertices, two_triangles.faces)
    h = mesh.halfedge(v0, v1)
    assert mesh.number_of_faces() == 6  # 6 faces
    # source vertex (v0) disappears but face connectivity is not updated
    join_vertex(h, mesh)
    assert mesh.number_of_faces() == 6  # still 6 faces
