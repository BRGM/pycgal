from pathlib import Path
import numpy as np
from pycgal.Polygon_mesh_processing import (
    isotropic_remeshing,
    split_long_edges,
    triangulate_faces,
)
from pycgal.Epick import Point_3
from pycgal.Surface_mesh import Surface_mesh
from pycgal.utils import (
    surface_mesh_to_vtp as to_vtp,
    surface_mesh_edges_to_vtu as edges_to_vtu,
)


#   3 -   -   -   -   -   -   - 2
#   |                           |
#                   4
#   |               |           |
#                   5 -  - 7
#   |               |           |
#                   6
#   |                           |
#   0 -   -   -   -   -   -   - 1


# Surface mesh
mesh = Surface_mesh()
vertices = np.array(
    [
        (0, 0, 0),
        (7, 0, 0),
        (7, 4, 0),
        (0, 4, 0),
        (4, 3, 0),
        (4, 2, 0),
        (4, 1, 0),
        (6, 2, 0),
    ]
)
vertices = [mesh.add_vertex(Point_3(*P)) for P in vertices]
faces = [
    (0, 1, 6),
    (1, 2, 7),
    (2, 3, 4),
    (0, 6, 5, 4, 3),
    (2, 4, 5, 7),
    (1, 7, 5, 6),
]
for face in faces:
    mesh.add_face([vertices[vert] for vert in face])


# faults ids
constraints_ids = {
    (vertices[4], vertices[5]): 1,
    (vertices[5], vertices[6]): 1,
    (vertices[5], vertices[7]): 2,
}

constrained_edges = [mesh.edge(*extremities) for extremities in constraints_ids.keys()]

edge_is_constrained, created = mesh.add_edge_property(
    "e:is_constrained", dtype="b", value=False
)
assert created
for e in constrained_edges:
    edge_is_constrained[e] = True


# Constrained vertices
vertex_is_constrained, created = mesh.add_vertex_property(
    "v:is_corner", dtype="b", value=False
)
assert created
vertex_is_constrained.set(True)


# Triangulation
triangulate_faces(mesh)
target_edge_size = 0.2
split_long_edges(
    mesh, (4 / 3) * target_edge_size, edge_is_constrained_map=edge_is_constrained
)
isotropic_remeshing(
    mesh,
    target_edge_size,
    edge_is_constrained_map=edge_is_constrained,
    vertex_is_constrained_map=vertex_is_constrained,
)

polylines = mesh.split_edge_constraints_into_polylines(edge_is_constrained)

fault_id, created = mesh.add_edge_property("e:fault_id", dtype="i", value=0)
assert created

for p in polylines:
    try:
        cid = constraints_ids[(p[0], p[-1])]
    except KeyError:
        cid = constraints_ids[(p[-1], p[0])]
    for v0, v1 in zip(p[:-1], p[1:]):
        fault_id[mesh.edge(v0, v1)] = cid

output_dir = Path("./2-faults")
output_dir.mkdir(exist_ok=True)
to_vtp(mesh, f"{output_dir}/2D_polygon")
edges_to_vtu(mesh, f"{output_dir}/2D_polygon_edges")
