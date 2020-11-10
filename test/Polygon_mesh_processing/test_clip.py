from pycgal.Surface_mesh import Surface_mesh
from pycgal.Epick import Point_3, Vector_3, Plane_3
from pycgal.Polygon_mesh_processing import clip, triangulate_faces


def test_clip(simple_mesh):
    mesh = Surface_mesh(simple_mesh.all_vertices, simple_mesh.triangles)
    triangulate_faces(mesh)
    plane = Plane_3(Point_3(0, 0, 0), Vector_3(1, 1, 0))
    clip(mesh, plane)
    print(mesh.as_arrays())
