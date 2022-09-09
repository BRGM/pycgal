import numpy as np

from pycgal.Epick import Point_3, Vector_3
from pycgal.Surface_mesh import Surface_mesh
from pycgal.utils import (
    surface_mesh_to_vtp as to_vtp,
    surface_mesh_edges_to_vtu as edges_to_vtu,
)


def test_surface_mesh_to_vtp(simple_mesh):

    mesh = Surface_mesh(
        simple_mesh.all_vertices, [simple_mesh.square, simple_mesh.triangles]
    )
    vprop, created = mesh.add_vertex_property("v:count", dtype="i")
    assert created
    for k, v in enumerate(mesh.vertices()):
        vprop[v] = k
    fprop, created = mesh.add_face_property("f:count", dtype="i")
    assert created
    for k, f in enumerate(mesh.faces()):
        fprop[f] = k
    eprop, created = mesh.add_edge_property("e:count", dtype="i")
    assert created
    for k, e in enumerate(mesh.edges()):
        eprop[e] = k
    edges_to_vtu(mesh, "simple_mesh_edges")
