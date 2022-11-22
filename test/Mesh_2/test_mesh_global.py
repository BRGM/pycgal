import pytest


def test_mesh_global():
    import pycgal.Mesh_2 as Mesh_2
    from pycgal.Epick import Point_2 as Point

    cdt = Mesh_2.Constrained_Delaunay_triangulation_2()
    va = cdt.insert(Point(-4, 0))
    vb = cdt.insert(Point(0, -1))
    vc = cdt.insert(Point(4, 0))
    vd = cdt.insert(Point(0, 1))
    cdt.insert(Point(2, 0.6))
    # NB: bounding box must me set as constraint
    cdt.insert_constraint(va, vb)
    cdt.insert_constraint(vb, vc)
    cdt.insert_constraint(vc, vd)
    cdt.insert_constraint(vd, va)
    print(f"Number of vertices: {cdt.number_of_vertices()}")
    print("Meshing the triangulation...")
    criteria = Mesh_2.Delaunay_mesh_adaptative_size_criteria_2(S=0.5)
    Mesh_2.refine_Delaunay_mesh_2(cdt, criteria)
    Mesh_2.Delaunay_mesher_2(cdt)
    print(f"Number of vertices: {cdt.number_of_vertices()}")
    print(cdt.as_arrays())


def test_mesh_with_intersecting_constraints():
    import pycgal.Mesh_2 as Mesh_2
    from pycgal.Epick import Point_2 as Point

    def add_constraints(cdt):
        assert cdt.number_of_vertices() == 0
        assert cdt.number_of_faces() == 0
        va = cdt.insert(Point(-1, -1))
        vb = cdt.insert(Point(1, -1))
        vc = cdt.insert(Point(1, 1))
        vd = cdt.insert(Point(-1, 1))
        cdt.insert_constraint(va, vc)
        cdt.insert_constraint(vb, vd)

    cdt = Mesh_2.Constrained_Delaunay_triangulation_2()
    with pytest.raises(RuntimeError):
        add_constraints(cdt)

    cdt = Mesh_2.Constrained_Delaunay_triangulation_2_with_intersections()
    add_constraints(cdt)
    assert cdt.number_of_faces() == 4
