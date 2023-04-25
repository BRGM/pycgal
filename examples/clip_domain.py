import numpy as np
from pycgal.Polygon_mesh_processing import (
    isotropic_remeshing,
    split_long_edges,
    triangulate_faces,
)
from pycgal.Mesh_3 import Polyhedral_mesh_domain_with_features_3 as Domain
from pycgal.Mesh_3 import Mesh_criteria_3, make_mesh_3
from pycgal.Surface_mesh import Surface_mesh
from pycgal.Surface_soup import Surface_soup
from pycgal.utils import (
    c3t3_to_vtu,
    surface_mesh_to_vtp as to_vtp,
    surface_mesh_edges_to_vtu as edges_to_vtu,
)

# describe polygon with upward normal
polygon = [(0, 0), (100, -20), (110, 70), (-10, 60)]
zbottom, ztop = -90, 30


top = [(x, y, ztop) for x, y in polygon]
bottom = [(x, y, zbottom) for x, y in polygon]

n = len(polygon)
vertices = top + bottom
top_face = list(range(n))
# reverse order to have outward orientation
bottom_face = list(range(n, 2 * n))[::-1]
all_faces = [top_face, bottom_face]
for i in range(n):
    all_faces.append([(i + 1) % n, i, i + n, (i + 1) % n + n])

bounds = Surface_mesh(vertices, all_faces)

constraints, created = bounds.add_edge_property(
    "e:is_constrained", dtype="b", value=False
)
assert created
constraints.set(True)
corners, created = bounds.add_vertex_property("v:is_corner", dtype="b", value=False)
assert created
corners.set(True)

target_edge_size = 10

triangulate_faces(bounds)
split_long_edges(
    bounds, (4 / 3) * target_edge_size, edge_is_constrained_map=constraints
)

isotropic_remeshing(
    bounds,
    target_edge_size,
    edge_is_constrained_map=constraints,
    vertex_is_constrained_map=corners,
)

edges_to_vtu(bounds, "all_edges")

# a vertical rectangle
vrect = Surface_mesh(
    [
        (50, -100, -100),
        (50, 100, -100),
        (50, 100, 100),
        (50, -100, 100),
    ],
    [[0, 1, 2, 3]],
)
corners, created = vrect.add_vertex_property("v:is_corner", dtype="b", value=False)
assert created
corners.set(True)
triangulate_faces(vrect)
split_long_edges(vrect, (4 / 3) * target_edge_size)
isotropic_remeshing(
    vrect,
    target_edge_size,
    vertex_is_constrained_map=corners,
)

soup = Surface_soup([(bounds, constraints), vrect], use_first_as_clipper=True)

soup_meshes = list(soup.meshes)

for i, mesh in enumerate(soup_meshes):
    to_vtp(mesh, f"mesh_{i:04d}")

domain = Domain(soup_meshes[1], soup_meshes[0])
domain.add_features(soup.collect_intersections())
criteria = Mesh_criteria_3(edge_size=10, cell_size=10, facet_distance=0.01)
c3t3 = make_mesh_3(domain, criteria)
c3t3_to_vtu(
    c3t3,
    "a_clipped_fault",
    with_curve_index=True,
    with_facet_index=True,
    with_subdomain_index=True,
)
