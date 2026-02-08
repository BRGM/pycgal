from pycgal.Polygon_mesh_processing import (
    isotropic_remeshing,
    split_long_edges,
    triangulate_faces,
)
from pycgal.Mesh_3 import Polyhedral_complex_mesh_domain_3 as Domain
from pycgal.Mesh_3 import Mesh_criteria_3, make_mesh_3
from pycgal.Surface_mesh import Surface_mesh
from pycgal.Surface_soup import Surface_soup
from pycgal.utils import (
    c3t3_to_vtu,
    surface_mesh_to_vtp as to_vtp,
    # surface_mesh_edges_to_vtu as edges_to_vtu,
)


def tag_and_remesh(mesh, tag_edges=False, target_edge_size=10):
    corners, created = mesh.add_vertex_property("v:is_corner", dtype="b", value=False)
    assert created
    corners.set(True)
    constraints = None
    if tag_edges:
        constraints, created = bounds.add_edge_property(
            "e:is_constrained", dtype="b", value=False
        )
        assert created
        constraints.set(True)
    triangulate_faces(mesh)
    split_long_edges(
        mesh, (4 / 3) * target_edge_size, edge_is_constrained_map=constraints
    )
    isotropic_remeshing(
        mesh,
        target_edge_size,
        edge_is_constrained_map=constraints,
        vertex_is_constrained_map=corners,
    )
    return constraints


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
constraints = tag_and_remesh(bounds, tag_edges=True)

# edges_to_vtu(bounds, "all_edges")

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
tag_and_remesh(vrect)

# an horizontal (non-traversing) rectangle
# generate bug
# hrect = Surface_mesh(
#     [
#         (-200, -200, 0),
#         (80, -200, 0),
#         (30, 200, 0),
#         (-200, 200, 0),
#     ],
#     [[0, 1, 2, 3]],
# )
hrect = Surface_mesh(
    [
        (-20, -30, 0),
        (70, -30, 0),
        (70, 80, 0),
        (-20, 80, 0),
    ],
    [[0, 1, 2, 3]],
)
tag_and_remesh(hrect)

to_vtp(hrect, "hrect_before_clip")

soup = Surface_soup((bounds, constraints), [vrect, hrect])

soup_meshes = list(soup.meshes)

to_vtp(soup_meshes[-1], "hrect_after_clip")

# for i, mesh in enumerate(soup_meshes):
#    to_vtp(mesh, f"mesh_{i:04d}")

domain = Domain(soup_meshes[0], soup_meshes[1:])
domain.add_features(soup.collect_polylines())
criteria = Mesh_criteria_3(edge_size=10, cell_size=10, facet_distance=0.01)
c3t3 = make_mesh_3(domain, criteria)
c3t3_to_vtu(
    c3t3,
    "two_clipped_faults",
    with_curve_index=True,
    with_facet_index=True,
    with_subdomain_index=True,
)

print("done")
