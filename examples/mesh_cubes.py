import numpy as np

# from pycgal.Epick import Point_3
from pycgal.Surface_mesh import Surface_mesh
from pycgal.Polygon_mesh_processing import triangulate_faces

# from pycgal.Polygon_mesh_processing import corefine
from pycgal.Mesh_3 import Polyhedral_mesh_domain_with_features_3 as Domain
from pycgal.Mesh_3 import Mesh_criteria_3, make_mesh_3
from pycgal.utils import c3t3_to_vtu

# a unit cube
vertices = np.array(
    [
        (0, 0, 0),
        (1, 0, 0),
        (1, 1, 0),
        (0, 1, 0),
        (0, 0, 1),
        (1, 0, 1),
        (1, 1, 1),
        (0, 1, 1),
    ],
    dtype=np.float64,
)
faces = np.array(
    [(0, 3, 2, 1), (0, 4, 7, 3), (2, 3, 7, 6), (1, 2, 6, 5), (0, 1, 5, 4), (4, 5, 6, 7)]
)

# bounding domain
cube = Surface_mesh(vertices, faces)
triangulate_faces(cube)

# make a smaller cube
vertices *= 0.3
vertices_sc1 = vertices + np.array([0.1, 0.1, 0.6], dtype=np.float64)
sc1 = Surface_mesh(vertices_sc1, faces)
triangulate_faces(sc1)

# translate the smaller cube
vertices_sc2 = vertices + np.array([0.3, 0.3, 0.4], dtype=np.float64)
sc2 = Surface_mesh(vertices_sc2, faces)
triangulate_faces(sc2)

# corefine(sc1, sc2)
smaller_cubes = sc1 + sc2

# internal meshes, bounding domain
domain = Domain(smaller_cubes, cube)
domain.detect_features()
criteria = Mesh_criteria_3(edge_size=0.05, cell_size=0.1, facet_distance=0.01)
c3t3 = make_mesh_3(domain, criteria)
c3t3_to_vtu(
    c3t3,
    "cubes",
    with_curve_index=True,
    with_subdomain_index=True,
    with_connected_components=True,
)
