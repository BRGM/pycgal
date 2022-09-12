import numpy as np
from pycgal.Polygon_mesh_processing import corefine, isotropic_remeshing
from pycgal.Surface_mesh import Surface_mesh


def _make_a_disc(
    center, normal, radius, target_edge_length=None, number_of_pieces=None
):
    from math import pi, cos, sin, sqrt
    from pycgal.Epick import Point_3, Vector_3, Plane_3

    assert target_edge_length or number_of_pieces
    assert not (target_edge_length and number_of_pieces)
    center = Point_3(*center)
    normal = Vector_3(*normal)
    P = Plane_3(center, normal)
    u = P.base1()
    u2 = u.squared_length()
    v = P.base2()
    v2 = v.squared_length()
    assert radius > 0
    if number_of_pieces:
        number_of_pieces = int(number_of_pieces)
        assert number_of_pieces > 0
        target_edge_length = 2 * pi * radius / number_of_pieces
    else:
        assert target_edge_length > 0
        number_of_pieces = int(2 * pi * radius // target_edge_length) + 1
        number_of_pieces = max(number_of_pieces, 3)
    circumference = []
    for theta in np.linspace(0, 2 * pi, number_of_pieces + 1)[:-1]:
        circumference.append(
            center
            + (radius * cos(theta) / sqrt(u2)) * u
            + (radius * sin(theta) / sqrt(v2)) * v
        )
    vertices = [center] + circumference
    assert len(circumference) == number_of_pieces
    triangles = [(0, i, i + 1) for i in range(1, number_of_pieces)]
    triangles.append((0, number_of_pieces, 1))
    mesh = Surface_mesh(vertices, triangles)
    isotropic_remeshing(mesh, target_edge_length, do_project=True)
    return mesh


def test_corefine_discs():

    disc1 = _make_a_disc(
        center=(0, 0, 0), normal=(0, 0, 1), radius=1, target_edge_length=0.2
    )
    disc2 = _make_a_disc(
        center=(0, 0, 0), normal=(0, 1, 1), radius=2, number_of_pieces=12
    )
    result = corefine(
        disc1, disc2, return_new_polylines=True, return_intersection_vertices=True
    )
    assert len(result) == 2
    polylines, vertices = result
    assert len(polylines) == 1
