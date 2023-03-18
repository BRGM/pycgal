import numpy as np

from pycgal.Epick import Point_3
from pycgal.Surface_mesh import Surface_mesh
from pycgal.Mesh_3 import Polyhedron


def test_mesh_polyhedron_3(simple_mesh):

    for vertices, faces in [
        (simple_mesh.all_vertices, simple_mesh.triangles),
        (simple_mesh.square_vertices, simple_mesh.square),
        (simple_mesh.all_vertices, [simple_mesh.square, simple_mesh.triangles]),
    ]:
        mesh = Surface_mesh(vertices, faces)
        polyhedron = Polyhedron(mesh)
        assert mesh.number_of_vertices() == polyhedron.number_of_vertices()
        assert mesh.number_of_vertices() == polyhedron.size_of_vertices()
        assert mesh.number_of_faces() == polyhedron.number_of_facets()
        assert mesh.number_of_faces() == polyhedron.size_of_facets()
