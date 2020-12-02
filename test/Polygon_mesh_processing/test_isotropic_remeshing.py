import pytest
from pycgal.Polygon_mesh_processing import is_triangle_mesh
from pycgal.Polygon_mesh_processing import isotropic_remeshing
from pycgal.Polygon_mesh_processing import triangulate_faces
from pycgal.Surface_mesh import Surface_mesh


def test_remesh(simple_mesh):

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
