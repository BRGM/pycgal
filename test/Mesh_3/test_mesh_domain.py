import numpy as np

from pycgal.Epick import Point_3
from pycgal.Surface_mesh import Surface_mesh
from pycgal.Polygon_mesh_processing import triangulate_faces
from pycgal.Mesh_3 import Polyhedral_mesh_domain_with_features_3 as Domain
from pycgal.Mesh_3 import Mesh_criteria_3, make_mesh_3


def test_bounded_domain(unit_cube):

    cube = Surface_mesh(*unit_cube)
    triangulate_faces(cube)
    Domain(cube)
    vertices, faces = unit_cube
    vertices = np.array(vertices) * 0.25
    vertices += np.array([0.5, 0.5, 0.5])
    # expecting a list of list of homogeneours faces when vertices is a numpy array
    smaller_cube = Surface_mesh(vertices, [faces])
    Domain(smaller_cube, cube)


def test_make_simple_mesh(unit_cube):

    cube = Surface_mesh(*unit_cube)
    triangulate_faces(cube)
    domain = Domain(cube)
    domain.detect_features()
    criteria = Mesh_criteria_3(cell_size=0.1)
    c3t3 = make_mesh_3(domain, criteria)
    vertices, corners, edges, triangles, tets = c3t3.as_arrays()
