from pycgal.Polygon_mesh_processing import bbox
from pycgal.Surface_mesh import Surface_mesh


def test_mesh_bounding_box(simple_mesh):
    for k, (vertices, faces) in enumerate(
        [
            (simple_mesh.all_vertices, simple_mesh.triangles),
            (simple_mesh.square_vertices, simple_mesh.square),
            (simple_mesh.all_vertices, [simple_mesh.square, simple_mesh.triangles]),
        ]
    ):
        mesh = Surface_mesh(vertices, faces)
        print(bbox(mesh))
