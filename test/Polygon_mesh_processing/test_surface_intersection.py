import numpy as np
from pycgal.Polygon_mesh_processing import surface_intersection
from pycgal.Surface_mesh import Surface_mesh


def test_surface_intersection(simple_mesh):

    print()
    tm1 = Surface_mesh(simple_mesh.square_vertices, np.array([[0, 1, 2]]))
    tm2 = Surface_mesh(simple_mesh.square_vertices, np.array([[0, 1, 3]]))
    polylines = surface_intersection(tm1, tm2)
    print("Intersection polylines:")
    print(polylines)
