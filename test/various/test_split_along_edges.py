import numpy as np
import pycgal
from pycgal.Epick import Point_2, Point_3, barycenter
import pycgal.Mesh_2 as Mesh_2
from pycgal.Surface_mesh import make_mesh, halfedges_around_source
from pycgal.Polygon_mesh_processing import split_along_edges
from pycgal.utils import (
    surface_mesh_to_vtp as to_vtp,
    surface_mesh_edges_to_vtu as edges_to_vtu,
)

assert pycgal.__version__ >= "0.3.15a4"

# will *cut* a mesh with thickness epsilon
def test_finite_fault(epsilon=1e-6, remesh_size_bound=0.1):

    cdt = Mesh_2.Constrained_Delaunay_triangulation_2_with_intersections()

    # bounding box
    vA = cdt.insert(Point_2(-1, -1))
    vB = cdt.insert(Point_2(1, -1))
    vC = cdt.insert(Point_2(1, 1))
    vD = cdt.insert(Point_2(-1, 1))

    # bounding box must me set as constraint
    for v1, v2 in [(vA, vB), (vB, vC), (vC, vD), (vD, vA)]:
        cdt.insert_constraint(v1, v2)

    # additional constraint
    va = cdt.insert(Point_2(-0.5, -0.5))
    vb = cdt.insert(Point_2(0.5, 0.5))
    vc = cdt.insert(Point_2(-0.5, 0.5))
    vd = cdt.insert(Point_2(0.5, -0.5))
    cdt.insert_constraint(va, vb)  # constrained segment
    cdt.insert_constraint(vc, vd)  # constrained segment

    # remesh the triangulation
    print(f"Number of vertices: {cdt.number_of_vertices()}")
    criteria = Mesh_2.Delaunay_mesh_adaptative_size_criteria_2(S=remesh_size_bound)
    Mesh_2.refine_Delaunay_mesh_2(cdt, criteria)
    print(f"Number of vertices: {cdt.number_of_vertices()}")

    # convert to Surface_mesh
    pts = cdt.points()
    assert len(pts) == cdt.number_of_vertices()
    vertices, faces, constraints = cdt.as_arrays_with_constraints()
    # convert to 3D Surface_mesh
    pts3D = np.zeros((vertices.shape[0], 3), dtype="d")
    pts3D[:, :2] = vertices
    mesh, vmap = make_mesh(pts3D, faces, with_vertices_map=True)

    edge_is_constrained, ok = mesh.add_edge_property("e:edge_is_constrained", dtype="b")
    assert ok
    for v1, v2 in constraints:
        e = mesh.edge(vmap[v1], vmap[v2])
        # filter border edges
        if not mesh.is_border(e):
            edge_is_constrained[e] = True

    # Split along edges
    [vertices_twins_map, edges_twins_map] = split_along_edges(
        mesh,
        edge_is_constrained_map=edge_is_constrained,
        return_vertex_twin=True,
        return_edge_twin=True,
    )

    # Check elements twins
    edge_twin, ok = mesh.add_edge_property("e:edge_twin", dtype="i", value=0)
    assert ok
    for key, value in edges_twins_map.items():
        edge_twin[key] = 1
        edge_twin[value] = 2
    vertex_twin, ok = mesh.add_vertex_property("v:vertex_twin", dtype="i", value=0)
    assert ok
    for key, value in vertices_twins_map.items():
        vertex_twin[key] = 1
        vertex_twin[value] = 2

    # look for pair of consecutive halfedges around vertices to be moved
    constrained_vertices = []
    for e in mesh.edges():
        if edge_is_constrained[e]:
            for i in [0, 1]:
                v = mesh.vertex(e, i)
                if v not in constrained_vertices:
                    constrained_vertices.append(v)

    # look for pair of consecutive halfedges around vertices to be moved
    constraint_contexts = []
    for v in constrained_vertices:
        h0 = h1 = None
        for h in halfedges_around_source(v, mesh):
            if edge_is_constrained[mesh.edge(h)]:
                if h0 is None:
                    h0 = h
                    continue
                assert h1 is None
                h1 = h
                break
        if h1 == mesh.next_around_source(h0):
            h0, h1 = h1, h0
        assert h0 is not None
        assert h1 is not None
        assert h0 != h1
        assert edge_is_constrained[mesh.edge(h0)]
        assert edge_is_constrained[mesh.edge(h1)]
        assert h1 != mesh.next_around_source(h0)
        assert mesh.source(h0) == mesh.source(h1) == v
        A = mesh.point(mesh.target(h0))
        B = mesh.point(mesh.target(h1))
        if A != B:
            constraint_contexts.append((v, h0, h1))
        else:
            print("Extremity at:", mesh.point(v))

    # for k, v in enumerate(constrained_vertices):
    for v, h0, h1 in constraint_contexts:
        targets = []
        h = mesh.next_around_source(h0)
        while h != h1:
            targets.append(mesh.point(mesh.target(h)))
            h = mesh.next_around_source(h)
        n = len(targets)
        assert n > 0
        # TODO: wrap barycenter of a list argument
        alpha = epsilon / n
        P = barycenter(mesh.point(v), 1 - epsilon, targets[0], alpha)
        for Q in targets[1:]:
            P = barycenter(P, 1, Q, alpha)
        mesh[v] = P

    # return a discontinuous z on the fault
    def dummy(P):
        x = P.x
        y = P.y
        z = (np.sign(x - y) + 0.5 * np.sign(x + y)) * max(
            0, 0.5**2 - (x**2 + y**2)
        )
        return Point_3(x, y, z)

    for v in mesh.vertices():
        mesh[v] = dummy(mesh[v])

    to_vtp(mesh, "result")
    edges_to_vtu(mesh, "result_edges")


if __name__ == "__main__":
    test_finite_fault()
