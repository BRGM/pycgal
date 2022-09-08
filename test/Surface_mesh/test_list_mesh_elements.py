import numpy as np
from pycgal.Surface_mesh import Surface_mesh, Vertices


def test_list_mesh_elements():
    mesh = Surface_mesh(
        [(-1, 0, 0), (1, 0, 0), (0, 1, 0), (2, 1, 0)], [(0, 1, 2), (2, 1, 3)]
    )
    assert all([v == w for v, w in zip(mesh.vertices(), list(mesh.vertices()))])
    print(list(mesh.vertices()))
    print(list(mesh.halfedges()))
    print(list(mesh.edges()))
    print(list(mesh.faces()))
    assert len(mesh.vertices()) == 4
    print(mesh.vertices().as_array())
    print(np.array(Vertices(mesh.vertices())))
    assert np.unique(mesh.vertices().as_array()).shape == (4,)
    assert len(mesh.halfedges()) == 10
    assert np.unique(mesh.halfedges().as_array()).shape == (10,)
    assert len(mesh.edges()) == 5
    assert np.unique(mesh.edges().as_array()).shape == (5,)
    assert len(mesh.faces()) == 2
    assert np.unique(mesh.faces().as_array()).shape == (2,)
