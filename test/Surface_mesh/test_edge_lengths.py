from collections import defaultdict
from math import sqrt

from pycgal.Epick import squared_distance
from pycgal.Surface_mesh import Surface_mesh


def test_edge_lengths(simple_mesh):

    mesh = Surface_mesh(simple_mesh.all_vertices, simple_mesh.triangles)
    d = defaultdict(lambda: 0)
    for e in mesh.edges():
        A = mesh.point(mesh.vertex(e, 0))
        B = mesh.point(mesh.vertex(e, 1))
        AB = sqrt(squared_distance(A, B))
        d[AB] += 1
    for el, n in d.items():
        print(f"{n} edges have length {el}")
