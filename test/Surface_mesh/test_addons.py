from pycgal.Epick import Point_3
from pycgal.Surface_mesh import Surface_mesh, locate


def test_locate(simple_mesh):

    vertices, faces = simple_mesh.all_vertices, simple_mesh.triangles
    mesh = Surface_mesh(vertices, faces)
    points = [Point_3(*row) for row in vertices]
    v = locate(mesh, points)
    assert all([mesh.point(v[k]) == Pk for k, Pk in enumerate(points)])
