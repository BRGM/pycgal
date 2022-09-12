import numpy as np
import pytest

from pycgal.Surface_mesh import Surface_mesh, halfedges_around_face


def test_halfedges_around_face(simple_mesh):
    mesh = Surface_mesh(
        simple_mesh.all_vertices, [simple_mesh.square, simple_mesh.triangles]
    )

    for f in mesh.faces():
        l1 = list(halfedges_around_face(mesh.halfedge(f), mesh))
        assert len(l1) == mesh.degree(f)
        l2 = []
        for h in halfedges_around_face(mesh.halfedge(f), mesh):
            assert h in l1
            l2.append(h)
        assert len(l1) == len(l2)
