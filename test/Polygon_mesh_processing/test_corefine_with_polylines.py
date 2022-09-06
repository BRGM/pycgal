import numpy as np
from pycgal.Polygon_mesh_processing import corefine
from pycgal.Surface_mesh import Surface_mesh


def _add_pid_map(tm, pid_name="e:polyline_id"):
    pid, created = tm.add_edge_property(pid_name, dtype="i", value=0)
    assert created
    for k, e in enumerate(tm.edges()):
        pid[e] = k + 1
    return pid


def _add_pid_maps(tm1, tm2, pid_name="e:polyline_id"):
    pid1 = _add_pid_map(tm1)
    pid2 = _add_pid_map(tm2)
    return pid1, pid2, pid_name


def test_corefine_with_polylines():

    tm1 = Surface_mesh([(-1, 1, 0), (1, 1, 0), (0, -1, 0)], [(0, 1, 2)])
    tm2 = Surface_mesh([(0, 0, -1), (0, 0, 1), (0, 2, 0)], [(0, 1, 2)])

    pid1, pid2, pid_name = _add_pid_maps(tm1, tm2)

    pid = pid1.copy_as_array()
    assert np.sum(pid == 1) == 1
    assert np.sum(pid == 0) == 0
    pid = pid2.copy_as_array()
    assert np.sum(pid == 1) == 1
    assert np.sum(pid == 0) == 0

    def dump():
        print(
            f"TM1 has {tm1.number_of_edges()} edges "
            f"TM2 has {tm2.number_of_edges()} edges"
        )
        print(pid1.copy_as_array())
        print(pid2.copy_as_array())

    dump()

    polylines, vertices = corefine(
        tm1,
        tm2,
        return_new_polylines=True,
        return_intersection_vertices=True,
        polyline_id_map=pid_name,
    )

    dump()

    pid = pid1.copy_as_array()
    assert np.sum(pid == 1) == 2
    assert np.sum(pid == 0) == 4
    pid = pid2.copy_as_array()
    assert np.sum(pid == 1) == 2
    assert np.sum(pid == 0) == 4

    print("Polylines:", polylines)
    print("Intersection vertices:", vertices)


def test_corefine_intersecting_edge():

    tm1 = Surface_mesh([(0, 0, 0), (2, 0, 0), (1, 1, 0)], [(0, 1, 2)])
    tm2 = Surface_mesh([(1, 0, -1), (1, 0, 1), (1, -1, 0)], [(0, 1, 2)])

    pid1, pid2, pid_name = _add_pid_maps(tm1, tm2)

    polylines, vertices = corefine(
        tm1,
        tm2,
        return_new_polylines=True,
        return_intersection_vertices=True,
        polyline_id_map=pid_name,
    )

    print("Polylines:", polylines)
    print("Intersection vertices:", vertices)
