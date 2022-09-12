def test_connected_components_of_constrained_triangulation():
    import numpy as np
    from pycgal.Epick import Point_2 as Point
    import pycgal.Mesh_2 as Mesh_2
    import pycgal.Surface_mesh as Surface_mesh
    from pycgal.Polygon_mesh_processing import connected_components

    cdt = Mesh_2.Constrained_Delaunay_triangulation_2_with_intersections()
    va = cdt.insert(Point(-1, -1))
    vb = cdt.insert(Point(1, -1))
    vc = cdt.insert(Point(1, 1))
    vd = cdt.insert(Point(-1, 1))
    cdt.insert_constraint(va, vc)
    cdt.insert_constraint(vb, vd)
    pts = cdt.points()
    assert len(pts) == cdt.number_of_vertices()
    vertices, faces, constraints = cdt.as_arrays_with_constraints()
    # convert to 3D Surface_mesh and use PMP::connected_components
    pts3D = np.zeros((vertices.shape[0], 3), dtype="d")
    pts3D[:, :2] = vertices
    mesh, vmap = Surface_mesh.make_mesh(pts3D, faces, with_vertices_map=True)
    component_id, ok = mesh.add_face_property("f:component", dtype="i")
    assert ok
    edge_is_constrained, ok = mesh.add_edge_property("e:edge_is_constrained", dtype="b")
    assert ok
    for v1, v2 in constraints:
        e = mesh.edge(vmap[v1], vmap[v2])
        edge_is_constrained[e] = True
    nb_comps = connected_components(
        mesh, component_id, edge_is_constrained_map=edge_is_constrained
    )
    assert nb_comps == 4
    assert np.unique(component_id).shape == (nb_comps,)
    assert mesh.is_border(mesh.edge(vmap[0], vmap[1]))
    assert mesh.is_border(mesh.edge(vmap[1], vmap[2]))
    assert mesh.is_border(mesh.edge(vmap[2], vmap[3]))
    assert mesh.is_border(mesh.edge(vmap[3], vmap[0]))
