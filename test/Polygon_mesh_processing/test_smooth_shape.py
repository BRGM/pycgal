from pycgal.Polygon_mesh_processing import smooth_shape
from pycgal.Surface_mesh import Surface_mesh


def test_smooth_shape(simple_mesh):

    mesh = Surface_mesh(simple_mesh.all_vertices, simple_mesh.triangles)
    smooth_shape(mesh, 1e-6)
