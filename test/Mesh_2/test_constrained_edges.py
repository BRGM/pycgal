def test_constrained_edges():
    import pycgal.Mesh_2 as Mesh_2
    from pycgal.Epick import Point_2 as Point

    cdt = Mesh_2.Constrained_Delaunay_triangulation_2_with_intersections()
    va = cdt.insert(Point(-1, -1))
    vb = cdt.insert(Point(1, -1))
    vc = cdt.insert(Point(1, 1))
    vd = cdt.insert(Point(-1, 1))
    cdt.insert_constraint(va, vc)
    cdt.insert_constraint(vb, vd)
    pts = cdt.points()
    assert len(pts) == cdt.number_of_vertices()
    for P in pts:
        print(P)
    print(cdt.as_arrays_with_constraints())
