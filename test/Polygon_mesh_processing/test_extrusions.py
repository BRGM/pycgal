from pycgal.Epick import Point_3
from pycgal.Epick import Vector_3
from pycgal.Epick import Plane_3
from pycgal.Polygon_mesh_processing import extrude_mesh
from pycgal.Surface_mesh import Surface_mesh


def test_extrusions(squares_2x2):

    mesh = Surface_mesh(squares_2x2.vertices, squares_2x2.faces)
    extruded = extrude_mesh(mesh, Vector_3(0.5, 0, 1.0))
    extruded = extrude_mesh(mesh, Vector_3(0, 0.5, -1.0), Vector_3(0.5, 0, 1.0))
    bottom = lambda P: P
    top = lambda P: Point_3(P.x, P.y, 1.0)
    extruded = extrude_mesh(mesh, bottom, top)
    slope = Plane_3(Point_3(0, 0, 1), Point_3(1, 0, 1.2), Point_3(0, 1, 1))
    extruded = extrude_mesh(mesh, bottom, slope.projection)
    print(extruded.as_arrays())
