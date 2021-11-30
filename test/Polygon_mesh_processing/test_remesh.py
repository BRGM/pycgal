import pytest
import numpy as np
from pycgal.Epick import Point_3  # to use return values in python
from pycgal.Polygon_mesh_processing import is_triangle_mesh
from pycgal.Polygon_mesh_processing import isotropic_remeshing
from pycgal.Polygon_mesh_processing import split_long_edges
from pycgal.Polygon_mesh_processing import triangulate_faces
from pycgal.Surface_mesh import Surface_mesh, Faces


def test_isotropic_remeshing(simple_mesh):

    mesh = Surface_mesh(
        simple_mesh.all_vertices, [simple_mesh.square, simple_mesh.triangles]
    )
    print(
        f"Hybrid mesh with {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    )
    assert not is_triangle_mesh(mesh)
    with pytest.raises(RuntimeError):
        isotropic_remeshing(mesh, 0.2)
    triangulate_faces(mesh)
    assert is_triangle_mesh(mesh)
    isotropic_remeshing(mesh, 0.2)
    print(
        f"... after remesh: {mesh.number_of_vertices()} vertices and {mesh.number_of_faces()} faces"
    )


def test_split_long_edges(simple_mesh):

    mesh = Surface_mesh(simple_mesh.square_vertices, simple_mesh.square)
    constraints, created = mesh.add_edge_property(
        "e:is_constrained", dtype="b", value=False
    )
    assert created
    constraints.set(True)
    assert np.sum(constraints.copy_as_array()) == 4
    triangulate_faces(mesh)
    assert np.sum(constraints.copy_as_array()) == 4
    print("Number of edges before split:", mesh.number_of_edges())
    split_long_edges(mesh, 0.5, edge_is_constrained_map=constraints)
    print("Number of edges after split:", mesh.number_of_edges())
    print(
        "Number of constrained edges after split:", np.sum(constraints.copy_as_array())
    )


def test_triangulate_some_faces(squares_2x2):

    mesh = Surface_mesh(squares_2x2.vertices, squares_2x2.faces)
    centroids = [(f, mesh.centroid(f)) for f in mesh.faces()]
    selection = [f for f, P in centroids if P.x > 0]
    assert len(selection) == 2
    some_faces = Faces()
    some_faces.extend(selection)
    assert mesh.number_of_faces() == 4
    triangulate_faces(mesh, some_faces)
    assert mesh.number_of_faces() == 6
    triangulate_faces(mesh)
    assert mesh.number_of_faces() == 8
