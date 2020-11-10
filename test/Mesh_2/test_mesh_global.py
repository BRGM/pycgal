def test_mesh_global():
    from pycgal.Epick import Point_2 as Point
    import pycgal.Mesh_2 as Mesh_2

    cdt = Mesh_2.Constrained_Delaunay_triangulation_2()
    va = cdt.insert(Point(-4, 0))
    vb = cdt.insert(Point(0, -1))
    vc = cdt.insert(Point(4, 0))
    vd = cdt.insert(Point(0, 1))
    cdt.insert(Point(2, 0.6))
    cdt.insert_constraint(va, vb)
    cdt.insert_constraint(vb, vc)
    cdt.insert_constraint(vc, vd)
    cdt.insert_constraint(vd, va)
    print(f"Number of vertices: {cdt.number_of_vertices()}")
    print("Meshing the triangulation...")
    criteria = Mesh_2.Delaunay_mesh_adaptative_size_criteria_2(S=0.5)
    Mesh_2.refine_Delaunay_mesh_2(cdt, criteria)
    mesher = Mesh_2.Delaunay_mesher_2(cdt)
    print(f"Number of vertices: {cdt.number_of_vertices()}")
    print(cdt.as_arrays())
    print(Mesh_2.as_arrays(cdt))
