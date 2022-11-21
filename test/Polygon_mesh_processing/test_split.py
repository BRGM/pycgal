import numpy as np
from pycgal.Polygon_mesh_processing import split, split_along_edges
from pycgal.Polygon_mesh_processing import connected_components
from pycgal.Surface_mesh import Surface_mesh
from pycgal.Epick import Point_3 as Point


# This test https://github.com/CGAL/cgal/issues/5867
def test_split():
    tm1 = Surface_mesh(
        np.array([(0, 0, 0), (1, 0, 0), (1, 1, 0), (0, 1, 0)], dtype=np.double),
        np.array([(0, 1, 2), (0, 2, 3)]),
    )
    tm2 = Surface_mesh(
        np.array([(0.5, 0, 0), (0.5, 1, 0), (0.5, 1, -1)], dtype=np.double),
        np.array([(0, 1, 2)]),
    )
    split(tm2, tm1)


def test_split_along_edges():
    tm = Surface_mesh()
    v = [tm.add_vertex(Point(*P)) for P in [(0, 0, 0), (1, 0, 0), (1, 1, 0), (0, 1, 0)]]
    tm.add_face([v[i] for i in (0, 1, 2)])
    tm.add_face([v[i] for i in (0, 2, 3)])
    component_id, ok = tm.add_face_property("f:component", dtype="i")
    assert ok
    nb_comps = connected_components(tm, component_id)
    assert nb_comps == 1
    e = tm.edge(v[0], v[2])  # diagonal
    assert tm.is_valid(e)
    print(tm.number_of_vertices())
    split_along_edges(tm, constrained_edges=[e])
    print(tm.number_of_vertices())
    nb_comps = connected_components(tm, component_id)
    assert nb_comps == 2
