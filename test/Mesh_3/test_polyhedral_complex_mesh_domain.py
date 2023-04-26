import numpy as np

from pycgal.Epick import Point_3
from pycgal.Surface_mesh import Surface_mesh
from pycgal.Polygon_mesh_processing import triangulate_faces
from pycgal.Mesh_3 import Polyhedral_complex_mesh_domain_3 as Domain
from pycgal.Mesh_3 import Mesh_criteria_3, make_mesh_3


def test_make_simple_complex_mesh(unit_cube):

    cube = Surface_mesh(*unit_cube)
    triangulate_faces(cube)
    domain = Domain(cube)
    domain.detect_features()
    criteria = Mesh_criteria_3(cell_size=0.1)
    c3t3 = make_mesh_3(domain, criteria)
    vertices, corners, edges, triangles, tets = c3t3.as_arrays()
