import numpy as np

from pycgal.Epick import Point_3
from pycgal.Surface_mesh import Surface_mesh
from pycgal.Polygon_mesh_processing import triangulate_faces

# from pycgal.Polygon_mesh_processing import corefine
from pycgal.Mesh_3 import Polyhedral_mesh_domain_with_features_3 as Domain
from pycgal.Mesh_3 import Mesh_criteria_3, make_mesh_3
from pycgal.utils import c3t3_to_vtu
from pycgal.Surface_soup import Surface_soup
from pycgal.utils import surface_mesh_to_vtp

# a cube
vertices = np.array(
    [
        (-1, -1, -1),
        (1, -1, -1),
        (1, 1, -1),
        (-1, 1, -1),
        (-1, -1, 1),
        (1, -1, 1),
        (1, 1, 1),
        (-1, 1, 1),
    ],
    dtype=np.float64,
)
faces = np.array(
    [(0, 3, 2, 1), (0, 4, 7, 3), (2, 3, 7, 6), (1, 2, 6, 5), (0, 1, 5, 4), (4, 5, 6, 7)]
)

# bounding domain
cube = Surface_mesh(vertices, faces)
triangulate_faces(cube)

# intersecting triangles
tm1 = Surface_mesh(
    [
        (-0.5, 0.0, -0.5),
        (0.5, 0.0, -0.5),
        (0.0, 0.0, 0.5),
    ],
    [[0, 1, 2]],
)
tm2 = Surface_mesh(
    [
        (0.0, -0.5, -0.5),
        (0.0, 0.5, -0.5),
        (0.0, 0.0, 0.5),
    ],
    [[0, 1, 2]],
)
soup = Surface_soup([tm1, tm2])

interior = Surface_mesh()
for mesh in soup.meshes:
    interior += mesh

# internal meshes, bounding domain
domain = Domain(interior, cube)
domain.detect_features()
criteria = Mesh_criteria_3(edge_size=0.2, cell_size=0.2, facet_distance=0.01)
c3t3 = make_mesh_3(domain, criteria)
c3t3_to_vtu(c3t3, "triangles", with_curve_index=True, with_subdomain_index=True)
