from pycgal.Epick import Surface_mesh, Point_3, Vector_3, Plane_3
from pycgal.Epick.PMP import clip, triangulate_faces
from pycgal.test.data import all_vertices, triangles, square


def test_clip():
    mesh = Surface_mesh(all_vertices, triangles)
    triangulate_faces(mesh)
    plane = Plane_3(Point_3(0, 0, 0), Vector_3(1, 1, 0))
    clip(mesh, plane)
    print(mesh.as_arrays())


if __name__ == "__main__":
    test_clip()
