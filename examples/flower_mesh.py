import numpy as np
import pycgal.Mesh_2 as Mesh_2
from pycgal.Epick import Point_2
from pycgal.Surface_mesh import make_mesh
from pycgal.utils import (
    surface_mesh_to_vtp as to_vtp,
    surface_mesh_edges_to_vtu as edges_to_vtu,
)

cdt = Mesh_2.Constrained_Delaunay_triangulation_2_with_intersections()

# bounding box
xmin, xmax = -1.0, 1.0
ymin, ymax = -1.0, 1.0
vA = cdt.insert(Point_2(xmin, ymin))
vB = cdt.insert(Point_2(xmax, ymin))
vC = cdt.insert(Point_2(xmax, ymax))
vD = cdt.insert(Point_2(xmin, ymax))

# bounding box must me set as constraint
for v1, v2 in [(vA, vB), (vB, vC), (vC, vD), (vD, vA)]:
    cdt.insert_constraint(v1, v2)

# additional constraint
[pa, pb, pc, pd, pe] = [
    Point_2(-0.5, -0.5),
    Point_2(0.5, 0.5),
    Point_2(-0.5, 0.5),
    Point_2(0.5, -0.5),
    Point_2(0.0, 0.0),
]
va = cdt.insert(pa)
vb = cdt.insert(pb)
vc = cdt.insert(pc)
vd = cdt.insert(pd)
cdt.insert_constraint(va, vb)  # constrained segment
cdt.insert_constraint(vc, vd)  # constrained segment

criteria = Mesh_2.Delaunay_mesh_adaptative_size_criteria_2(S=0.25)
Mesh_2.refine_Delaunay_mesh_2(cdt, criteria)

# convert to Surface_mesh
pts = cdt.points()
assert len(pts) == cdt.number_of_vertices()
vertices, faces, constraints = cdt.as_arrays_with_constraints()
pts3D = np.zeros((vertices.shape[0], 3), dtype="d")
pts3D[:, :2] = vertices
mesh, vmap = make_mesh(pts3D, faces, with_vertices_map=True)

# Constrained edges
edge_is_constrained, ok = mesh.add_edge_property("e:is_constrained", dtype="b")
assert ok
for v1, v2 in constraints:
    e = mesh.edge(vmap[v1], vmap[v2])
    # filter border edges
    if not mesh.is_border(e):
        edge_is_constrained[e] = True


mesh.pack_cells_along_edges(edge_is_constrained, "packs")

to_vtp(mesh, "flower_mesh")
edges_to_vtu(mesh, "flower_edges")
